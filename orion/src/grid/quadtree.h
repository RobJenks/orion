#pragma once

#include <assert.h>
#include <array>
#include <vector>
#include <algorithm>
#include "../math/vec2.h"

namespace Orion
{
	template <typename T>
	class Quadtree
	{
	public:
		typedef size_t NodeIndex;
		static const NodeIndex NO_NODE = std::numeric_limits<NodeIndex>::max();
		enum class ChildNode { TopRight = 0, BottomRight = 1, BottomLeft = 2, TopLeft = 3 };

		static const size_t MAX_NODE_ITEMS = 16U;		// Maximum number of items which can be accepted by a node before it attempts to subdivide
		static const int MIN_NODE_SIZE = 4;				// Minimum size of nodes in any dimension, beyond which they will never subdivide, even if over item limit


		// Quadtree node
		class Node
		{
		public:
			Node(NodeIndex id, NodeIndex parent, Vec2<int> minBounds, Vec2<int> maxBounds);

			NodeIndex getId() const;
			NodeIndex getParent() const;

			Vec2<int> getMinBounds() const;
			Vec2<int> getMaxBounds() const;
			Vec2<int> getCentrePoint() const;
			bool canSubdivide() const;

			bool isRoot() const;
			bool hasParent() const;
			bool isBranch() const;
			bool isLeaf() const;
			bool hasChildren() const;
			const std::array<NodeIndex, 4> & getChildren() const;
			void setChildren(std::array<NodeIndex, 4> children);

			bool canAcceptItems() const;
			void addItemDirect(T item);
			const std::vector<T> & getItems() const;
			void getItems(std::vector<T>& items) const;
			void setItems(std::vector<T>&& items);
			bool removeItemDirect(T item);
			void clearItemsDirect();

			bool		containsPoint(Vec2<int> point) const;
			bool		containsRegion(Vec2<int> minPoint, Vec2<int> maxPoint) const;
			ChildNode	getChildContainingPoint(Vec2<int> point) const;


		private:
			std::vector<T>						m_items;

			NodeIndex							m_id;
			NodeIndex							m_parent;
			std::array<typename NodeIndex, 4>	m_children;

			Vec2<int>							m_min_bounds;
			Vec2<int>							m_max_bounds;
			Vec2<int>							m_centre;
		};


	public:

		Quadtree(Vec2<int> minBounds, Vec2<int> maxBounds);

		NodeIndex	addItem(T item);
		NodeIndex	addItem(NodeIndex index, T item);

		bool		removeItem(T item);
		bool		removeItem(NodeIndex index, T item);

		void		clearItems();

		void		findItems(Vec2<int> minPos, Vec2<int> maxPos, std::vector<T> & outItems) const;

		void		subdivide(NodeIndex index);

		void		itemMoved(T item, Vec2<int> oldPosition);

	private:

		NodeIndex	newNode(NodeIndex parent, Vec2<int> minBounds, Vec2<int> maxBounds);

		void		getItemsRecursive(NodeIndex index, std::vector<T>& items) const;
		NodeIndex   getLeafNodeContainingPoint(Vec2<int> pos) const;

		void		deleteSingleNode(NodeIndex index);
		void		deleteSubtree(NodeIndex index);
		void		attemptToCollapseNode(NodeIndex index);

		NodeIndex	getRoot() const;

	private:

		std::vector<Node>			m_nodes;
		std::vector<NodeIndex>		m_free_nodes;

	};


	/* ********************************************************************* */
	/* Quadtree implementation												 */
	/* ********************************************************************* */

	template <typename T>
	Quadtree<T>::Quadtree(Vec2<int> minBounds, Vec2<int> maxBounds)
		:
		m_nodes(),
		m_free_nodes()
	{
		newNode(NO_NODE, minBounds, maxBounds);
	}

	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::newNode(NodeIndex parent, Vec2<int> minBounds, Vec2<int> maxBounds)
	{
		// Reuse a node from the free list if available
		if (!m_free_nodes.empty())
		{
			Quadtree<T>::NodeIndex index = m_free_nodes.back();
			m_free_nodes.pop_back();
			m_nodes[index] = Node(index, parent, minBounds, maxBounds);

			return index;
		}

		// Allocate a new node
		Quadtree<T>::NodeIndex index = m_nodes.size();
		m_nodes.push_back(Node(index, parent, minBounds, maxBounds));

		return index;
	}

	// Deletes a single node without considering any relationships to other nodes in the tree
	template <typename T>
	void Quadtree<T>::deleteSingleNode(NodeIndex index)
	{
		m_free_nodes.push_back(index);
	}
	
	// Deletes a node and all child nodes below it.  Items are not reallocated and are simply discarded
	template <typename T>
	void Quadtree<T>::deleteSubtree(NodeIndex index)
	{
		const auto& node = m_nodes[index];
		if (node.hasChildren())
		{
			const auto& children = node.getChildren();
			deleteSubtree(children[0]);
			deleteSubtree(children[1]);
			deleteSubtree(children[2]);
			deleteSubtree(children[3]);
		}

		deleteSingleNode(index);
	}

	template <typename T>
	void Quadtree<T>::attemptToCollapseNode(NodeIndex index)
	{
		Quadtree<T>::Node & node = m_nodes[index];

		// Only need to clean up if we do have child nodes
		if (!node.hasChildren()) return;

		// Get all items below this node and see whether we are within the threshold to collapse
		std::vector<T> items;
		getItemsRecursive(index, items);
		if (items.size() > MAX_NODE_ITEMS) return;

		// We are able to collapse this node, so add all child items to this node and dispose of children
		node.setItems(std::move(items));

		const auto& children = node.getChildren();
		deleteSubtree(children[0]);
		deleteSubtree(children[1]);
		deleteSubtree(children[2]);
		deleteSubtree(children[3]);
	}
	
	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::getRoot() const
	{
		return 0;	// Root node will always be at the first location
	}
	
	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::addItem(T item)
	{
		return addItem(getRoot(), item);
	}

	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::addItem(NodeIndex index, T item)
	{
		// If the item will not fit within this tree then quit immediately
		Node& node = m_nodes[index];
		const auto& pos = item.getPosition();
		if (!node.containsPoint(pos)) return NO_NODE;

		// Locate the leaf node where this item should be added
		while (node.hasChildren())
		{
			index = node.getChildContainingPoint(pos);
			node = m_nodes[index];
		}

		// If we are at the item threshold, we may need to try subdividing
		if (!node.canAcceptItems())
		{
			// Only subdivide if we are still permitted to do so
			if (node.canSubdivide())
			{
				// Subdivide and then call this method recursively; should trivially-succeed on new child, unless all items moved into the same child and another subdivision is needed
				subdivide(index);
				return addItem(index, item);	
			}
		}

		// We are within the threshold, or cannot subdivide any further, so add directly to this node
		node.addItemDirect(item);
		return index;
	}

	template <typename T>
	bool Quadtree<T>::removeItem(T item)
	{
		return removeItem(getRoot(), item);
	}

	template <typename T>
	bool Quadtree<T>::removeItem(NodeIndex index, T item)
	{
		// If the item would not fit within this tree then quit immediately
		Node& node = m_nodes[index];
		const auto& pos = node.getPosition();
		if (!node.containsPoint(pos)) return false;

		// Locate the leaf node where this item should be added
		while (node.hasChildren())
		{
			index = node.getChildContainingPoint(pos);
			node = m_nodes[index];
		}

		// Remove the item if it exists
		return node.removeItemDirect(item);
	}

	template <typename T>
	void Quadtree<T>::clearItems()
	{
		// We can just clear everything in all nodes, since it does not matter whether the node is freed or not
		for (auto & node : m_nodes)
		{
			node.clearItemsDirect();
		}
	}

	template <typename T>
	void Quadtree<T>::findItems(Vec2<int> minPos, Vec2<int> maxPos, std::vector<T>& outItems) const
	{
		std::vector<NodeIndex> search({ getRoot() });

		while (!search.empty())
		{
			auto index = search.back();
			search.pop_back();
			auto& node = m_nodes[index];

			// Skip this node immediately if it contains no part of the target region
			if (!node.containsRegion(minPos, maxPos)) continue;

			// Recurse into all children if this node has them (eligibility will be checked when they are pulled from the search stack)
			if (node.hasChildren())
			{
				const auto& children = node.getChildren();
				search.push_back(children[0]);
				search.push_back(children[1]);
				search.push_back(children[2]);
				search.push_back(children[3]);
			}
			else
			{
				// Otherwise, if this is a leaf node, check all items for eligibility
				for (const T& item : node.getItems())
				{
					// Add all items which are within the target region
					const auto& pos = item.getPosition();
					if (pos.x >= node.getMinBounds().x && pos.x < node.getMaxBounds().x &&
						pos.y >= node.getMinBounds().y && pos.y < node.getMaxBounds().y)
					{
						outItems.push_back(item);
					}
				}
			}
		}
	}

	template <typename T>
	void Quadtree<T>::getItemsRecursive(NodeIndex index, std::vector<T>& items) const
	{
		const auto& node = m_nodes[index];
		if (node.hasChildren())
		{
			const auto& children = node.getChildren();
			getItemsRecursive(children[0], items);
			getItemsRecursive(children[1], items);
			getItemsRecursive(children[2], items);
			getItemsRecursive(children[3], items);
		}
		else
		{
			items.insert(items.cend(), node.getItems());
		}
	}

	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::getLeafNodeContainingPoint(Vec2<int> pos) const
	{
		auto index = getRoot();
		const auto& node = m_nodes[index];

		if (!node.containsPoint(pos)) return NO_NODE;

		while (node.hasChildren())
		{
			index = node.getChildContainingPoint(pos);
			node = m_nodes[index];
		}

		return index;
	}

	template <typename T>
	void Quadtree<T>::subdivide(NodeIndex index)
	{
		auto& node = m_nodes[index];
		assert(!node.hasChildren());

		// Create children
		node.setChildren({
			newNode(index, node.getMinBounds(), node.getCentrePoint()),																				// Bottom-left
			newNode(index, Vec2<int>{node.getMinBounds().x, node.getCentrePoint().y}, Vec2<int>{node.getCentrePoint().x, node.getMaxBounds().y}),	// Top-left
			newNode(index, node.getCentrePoint(), node.getMaxPoint()),																				// Top-right
			newNode(index, Vec2<int>{node.getCentrePoint().x, node.getMinBounds().y}, Vec2<int>{node.getMaxPoint().x, node.getCenterPoint().y})		// Bottom-right
		});

		// Add all item to the relevant child node
		const auto& children = node.getChildren();
		for (auto item : node.getItems())
		{
			auto child = node.getChildContainingPoint(item);
			m_nodes[children[child]].addItemDirect(item);
		}

		// Clear all items from the current node, since we are no longer a leaf and have distributed all items
		node.clearItemsDirect();
	}

	template <typename T>
	void Quadtree<T>::itemMoved(T item, Vec2<int> oldPosition)
	{
		// Find the node that this object previously existed in
		auto currentNode = getLeafNodeContainingPoint(oldPosition);
		if (currentNode == NO_NODE) return;

		// If this node still contains the new object position then we have nothing to do
		if (m_nodes[currentNode].containsPoint(item.getPosition())) return;

		// We need to move this object.  Remove from the current node and re-add it to the tree root
		m_nodes[currentNode].removeItemDirect(item);
		addItem(item);
	}

	

	/* ********************************************************************* */
	/* Quadtree::Node implementation										 */
	/* ********************************************************************* */

	template <typename T>
	Quadtree<T>::Quadtree::Node::Node(NodeIndex id, NodeIndex parent, Vec2<int> minBounds, Vec2<int> maxBounds)
		:
		m_id(id),
		m_parent(parent),
		m_children({NO_NODE, NO_NODE, NO_NODE, NO_NODE}),
		m_min_bounds(minBounds),
		m_max_bounds(maxBounds),
		m_centre((minBounds + maxBounds) / Vec2<int>{2, 2})
	{
	}

	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::Node::getId() const
	{
		return m_id;
	}

	template <typename T>
	typename Quadtree<T>::NodeIndex Quadtree<T>::Node::getParent() const
	{
		return m_parent;
	}

	template <typename T>
	Vec2<int> Quadtree<T>::Node::getMinBounds() const
	{
		return m_min_bounds;
	}

	template <typename T>
	Vec2<int> Quadtree<T>::Node::getMaxBounds() const
	{
		return m_max_bounds;
	}

	template <typename T>
	Vec2<int> Quadtree<T>::Node::getCentrePoint() const
	{
		return m_centre;
	}
	
	template <typename T>
	bool Quadtree<T>::Node::canSubdivide() const
	{
		const auto threshold = MIN_NODE_SIZE * 2;
		return ((m_max_bounds.x - m_min_bounds.x) > threshold) &&
			   ((m_max_bounds.y - m_min_bounds.y) > threshold);
	}

	template <typename T>
	bool Quadtree<T>::Node::isRoot() const
	{
		return !hasParent();
	}
	
	template <typename T>
	bool Quadtree<T>::Node::hasParent() const
	{
		return (m_parent != NO_NODE);
	}

	template <typename T>
	bool Quadtree<T>::Node::isBranch() const
	{
		return hasChildren();
	}
	
	template <typename T>
	bool Quadtree<T>::Node::isLeaf() const
	{
		return !hasChildren();
	}

	template <typename T>
	bool Quadtree<T>::Node::hasChildren() const
	{
		return (m_children[0] != NO_NODE);
	}

	template <typename T>
	const std::array<typename Quadtree<T>::NodeIndex, 4>& Quadtree<T>::Node::getChildren() const
	{
		return m_children;
	}

	template <typename T>
	void Quadtree<T>::Node::setChildren(std::array<NodeIndex, 4> children)
	{
		m_children = children;
	}

	template <typename T>
	bool Quadtree<T>::Node::canAcceptItems() const
	{
		return m_items.size() < MAX_NODE_ITEMS;
	}
	
	template <typename T>
	void Quadtree<T>::Node::addItemDirect(T item)
	{
		m_items.push_back(item);
	}

	template <typename T>
	const std::vector<T>& Quadtree<T>::Node::getItems() const
	{
		return m_items;
	}

	template <typename T>
	void Quadtree<T>::Node::getItems(std::vector<T>& items) const
	{
		m_items.insert(m_items.end(), items);
	}

	template <typename T>
	void Quadtree<T>::Node::setItems(std::vector<T>&& items)
	{
		m_items = items;
	}

	template <typename T>
	bool Quadtree<T>::Node::removeItemDirect(T item)
	{
		const auto it = std::find_if(m_items.begin(), m_items.end(), [item](const T& el) { return item == el; });
		if (it != m_items.end())
		{
			m_items.erase(it);
			return true;
		}

		return false;
	}
	
	template <typename T>
	void Quadtree<T>::Node::clearItemsDirect()
	{
		m_items.clear();
	}

	template <typename T>
	bool Quadtree<T>::Node::containsPoint(Vec2<int> point) const
	{
		return point.x >= m_min_bounds.x && point.x < m_max_bounds.x&&
			point.y >= m_min_bounds.y && point.y < m_max_bounds.y;
	}

	template <typename T>
	bool Quadtree<T>::Node::containsRegion(Vec2<int> minPoint, Vec2<int> maxPoint) const
	{
		return !(								// Check FAILS if:
			minPoint.x >= m_max_bounds.x ||		//  1. region left is to the right of our right bound, or
			maxPoint.x < m_min_bounds.x ||		//  2. region right is to the left of our left bound, or
			minPoint.y >= m_max_bounds.y ||     //  3. region bottom is above our topmost bound, or
			maxPoint.y < m_min_bounds.y			//  4. region top is below our bottom-most bound
			);
	}

	template <typename T>
	typename Quadtree<T>::ChildNode Quadtree<T>::Node::getChildContainingPoint(Vec2<int> point) const
	{
		if (point.x < m_centre.x)
		{
			if (point.y < m_centre.y)
			{
				return ChildNode::BottomLeft;
			}
			else // y >= centre.y
			{
				return ChildNode::TopLeft;
			}
		}
		else // x >= centre.x
		{
			if (point.y < m_centre.y)
			{
				return ChildNode::BottomRight;
			}
			else // y >= centre.y
			{
				return ChildNode::TopRight;
			}
		}
	}
}
