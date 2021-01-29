#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include "../util/debug.h"
#include "../math/vec2.h"

namespace Orion
{
	template <typename T, typename TCoord>
	class Quadtree
	{
	public:
		typedef size_t NodeIndex;
		static const NodeIndex NO_NODE = std::numeric_limits<NodeIndex>::max();
		enum class ChildNode { TopRight = 0, BottomRight = 1, BottomLeft = 2, TopLeft = 3 };

		static const size_t MAX_NODE_ITEMS = 16U;		// Maximum number of items which can be accepted by a node before it attempts to subdivide
		static const int MIN_NODE_SIZE = 4;				// Minimum size of nodes in any dimension, beyond which they will never subdivide, even if over item limit

		// Position-wrapped object
		struct Item
		{
			T			 item;
			Vec2<TCoord> pos;

			Item(T && _item, Vec2<TCoord> && _pos) : item(_item), pos(_pos) {}
		};

		// Quadtree node
		class Node
		{
		public:
			Node(NodeIndex id, NodeIndex parent, Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds);

			NodeIndex getId() const;
			NodeIndex getParent() const;

			Vec2<TCoord> getMinBounds() const;
			Vec2<TCoord> getMaxBounds() const;
			Vec2<TCoord> getCentrePoint() const;
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

			bool		containsPoint(Vec2<TCoord> point) const;
			bool		containsRegion(Vec2<TCoord> minPoint, Vec2<TCoord> maxPoint) const;
			ChildNode	getChildContainingPoint(Vec2<TCoord> point) const;


		private:
			std::vector<T>						m_items;

			NodeIndex							m_id;
			NodeIndex							m_parent;
			std::array<typename NodeIndex, 4>	m_children;

			Vec2<TCoord>							m_min_bounds;
			Vec2<TCoord>							m_max_bounds;
			Vec2<TCoord>							m_centre;
		};


	public:

		Quadtree(Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds);

		NodeIndex	addItem(T item);
		NodeIndex	addItem(NodeIndex index, T item);

		NodeIndex	addItemAtPosition(T item, Vec2<TCoord> pos);
		NodeIndex	addItemAtPosition(NodeIndex index, T item, Vec2<TCoord> pos);

		bool		removeItem(T item);
		bool		removeItem(NodeIndex index, T item);

		void		clearItems();

		void		findItems(Vec2<TCoord> minPos, Vec2<TCoord> maxPos, std::vector<T> & outItems) const;

		T			getItemAtExact(Vec2<TCoord> pos) const;

		void		subdivide(NodeIndex index);

		void		itemMoved(T item, Vec2<TCoord> oldPosition);

	private:

		NodeIndex	newNode(NodeIndex parent, Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds);

		void		getItemsRecursive(NodeIndex index, std::vector<T>& items) const;
		NodeIndex   getLeafNodeContainingPoint(Vec2<TCoord> pos) const;

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

	template <typename T, typename TCoord>
	Quadtree<T, TCoord>::Quadtree(Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds)
		:
		m_nodes(),
		m_free_nodes()
	{
		ASS(minBounds < maxBounds, "Invalid bounds; min=" << minBounds << ", max=" << maxBounds);
		newNode(NO_NODE, minBounds, maxBounds);
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::newNode(NodeIndex parent, Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds)
	{
		// Reuse a node from the free list if available
		if (!m_free_nodes.empty())
		{
			Quadtree<T, TCoord>::NodeIndex index = m_free_nodes.back();
			m_free_nodes.pop_back();
			m_nodes[index] = Node(index, parent, minBounds, maxBounds);

			return index;
		}

		// Allocate a new node
		Quadtree<T, TCoord>::NodeIndex index = m_nodes.size();
		m_nodes.push_back(Node(index, parent, minBounds, maxBounds));

		return index;
	}

	// Deletes a single node without considering any relationships to other nodes in the tree
	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::deleteSingleNode(NodeIndex index)
	{
		m_free_nodes.push_back(index);
	}
	
	// Deletes a node and all child nodes below it.  Items are not reallocated and are simply discarded
	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::deleteSubtree(NodeIndex index)
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

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::attemptToCollapseNode(NodeIndex index)
	{
		Quadtree<T, TCoord>::Node & node = m_nodes[index];

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
	
	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::getRoot() const
	{
		return 0;	// Root node will always be at the first location
	}
	
	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::addItem(T item)
	{
		return addItemAtPosition(getRoot(), item, item.getPosition());
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::addItem(NodeIndex index, T item)
	{
		return addItemAtPosition(index, item, item.getPosition());
	}
	
	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::addItemAtPosition(T item, Vec2<TCoord> pos)
	{
		return addItemAtPosition(getRoot(), item, pos);
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::addItemAtPosition(NodeIndex index, T item, Vec2<TCoord> pos)
	{
		// If the item will not fit within this tree then quit immediately
		Node* node = &(m_nodes[index]);
		if (!node->containsPoint(pos)) return NO_NODE;

		// Locate the leaf node where this item should be added
		while (node->hasChildren())
		{
			const auto child = node->getChildContainingPoint(pos);
			index = node->getChildren()[static_cast<int>(child)];
			ASS(index != NO_NODE, "Missing child node: " << child);

			node = &(m_nodes[index]);
		}

		// If we are at the item threshold, we may need to try subdividing
		if (!node->canAcceptItems())
		{
			// Only subdivide if we are still permitted to do so
			if (node->canSubdivide())
			{
				// Subdivide and then call this method recursively; should trivially-succeed on new child, unless all items moved into the same child and another subdivision is needed
				subdivide(index);
				return addItem(index, item);	
			}
		}

		// We are within the threshold, or cannot subdivide any further, so add directly to this node
		node->addItemDirect(item);
		return index;
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::removeItem(T item)
	{
		return removeItem(getRoot(), item);
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::removeItem(NodeIndex index, T item)
	{
		// If the item would not fit within this tree then quit immediately
		Node *node = &(m_nodes[index]);
		const auto& pos = item.getPosition();
		if (!node->containsPoint(pos)) return false;

		// Locate the leaf node where this item should be added
		while (node->hasChildren())
		{
			index = node->getChildContainingPoint(pos);
			node = &(m_nodes[index]);
		}

		// Remove the item if it exists
		return node->removeItemDirect(item);
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::clearItems()
	{
		// We can just clear everything in all nodes, since it does not matter whether the node is freed or not
		for (auto & node : m_nodes)
		{
			node.clearItemsDirect();
		}
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::findItems(Vec2<TCoord> minPos, Vec2<TCoord> maxPos, std::vector<T>& outItems) const
	{
		std::vector<NodeIndex> search({ getRoot() });

		while (!search.empty())
		{
			auto index = search.back();
			search.pop_back();
			const auto & node = m_nodes[index];

			// Skip this node immediately if it contains no part of the target region
			if (!node.containsRegion(minPos, maxPos)) continue;

			// Recurse into all children if this node has them (eligibility will be checked when they are pulled from the search stack)
			if (node.hasChildren())
			{
				const auto& children = node.getChildren();
				search.insert(search.end(), children.begin(), children.end());
			}
			else
			{
				// Otherwise, if this is a leaf node, check all items for eligibility
				for (const T& item : node.getItems())
				{
					// Add all items which are within the target region
					const auto& pos = item.getPosition();
					if (pos.x >= minPos.x && pos.x < maxPos.x &&
						pos.y >= minPos.y && pos.y < maxPos.y)
					{
						outItems.push_back(item);
					}
				}
			}
		}
	}

	template<typename T, typename TCoord>
	inline T Quadtree<T, TCoord>::getItemAtExact(Vec2<TCoord> pos) const
	{
		std::vector<NodeIndex> search({ getRoot() });

		while (!search.empty())
		{
			auto index = search.back();
			search.pop_back();
			const auto& node = m_nodes[index];

			// Skip this node immediately if it contains no part of the target region
			if (!node.containsRegion(minPos, maxPos)) continue;

			// Recurse into all children if this node has them (eligibility will be checked when they are pulled from the search stack)
			if (node.hasChildren())
			{
				const auto& children = node.getChildren();
				search.insert(search.end(), children.begin(), children.end());
			}
			else
			{
				// Otherwise, if this is a leaf node, check all items for eligibility
				for (const T& item : node.getItems())
				{
					// Add the first item which has the exact required position
					if (item.getPosition() == pos) return item;
				}
			}
		}
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::getItemsRecursive(NodeIndex index, std::vector<T>& items) const
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

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::getLeafNodeContainingPoint(Vec2<TCoord> pos) const
	{
		auto index = getRoot();
		const auto *node = &(m_nodes[index]);

		if (!node->containsPoint(pos)) return NO_NODE;

		while (node->hasChildren())
		{
			index = node->getChildContainingPoint(pos);
			node = &(m_nodes[index]);
		}

		return index;
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::subdivide(NodeIndex index)
	{
		// Do not get a reference to the node here, beofre children are created, since we are about to modify the node collection and the reference may be invalidated
		ASS(!m_nodes[index].hasChildren(), "Attempted to subdivide a node (index: " << index << ") with existing children");

		// Create children
		const auto pMin = m_nodes[index].getMinBounds(), pCtr = m_nodes[index].getCentrePoint(), pMax = m_nodes[index].getMaxBounds();
		std::array<NodeIndex, 4> newChildren {
			newNode(index, pMin, pCtr),														// Bottom-left
			newNode(index, Vec2<TCoord>(pMin.x, pCtr.y), Vec2<TCoord>(pCtr.x, pMax.y)),		// Top-left
			newNode(index, pCtr, pMax),														// Top-right
			newNode(index, Vec2<TCoord>(pCtr.x, pMin.y), Vec2<TCoord>(pMax.x, pCtr.y))		// Bottom-right
		};
		m_nodes[index].setChildren(newChildren);

		// Add all item to the relevant child node
		auto& node = m_nodes[index];
		const auto& children = node.getChildren();
		for (auto item : node.getItems())
		{
			auto child = node.getChildContainingPoint(item.getPosition());
			m_nodes[children[static_cast<int>(child)]].addItemDirect(item);
		}

		// Clear all items from the current node, since we are no longer a leaf and have distributed all items
		node.clearItemsDirect();
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::itemMoved(T item, Vec2<TCoord> oldPosition)
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

	template <typename T, typename TCoord>
	Quadtree<T, TCoord>::Quadtree::Node::Node(NodeIndex id, NodeIndex parent, Vec2<TCoord> minBounds, Vec2<TCoord> maxBounds)
		:
		m_id(id),
		m_parent(parent),
		m_children({NO_NODE, NO_NODE, NO_NODE, NO_NODE}),
		m_min_bounds(minBounds),
		m_max_bounds(maxBounds),
		m_centre((minBounds + maxBounds) / Vec2<TCoord>{2, 2})
	{
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::Node::getId() const
	{
		return m_id;
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::NodeIndex Quadtree<T, TCoord>::Node::getParent() const
	{
		return m_parent;
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Quadtree<T, TCoord>::Node::getMinBounds() const
	{
		return m_min_bounds;
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Quadtree<T, TCoord>::Node::getMaxBounds() const
	{
		return m_max_bounds;
	}

	template <typename T, typename TCoord>
	Vec2<TCoord> Quadtree<T, TCoord>::Node::getCentrePoint() const
	{
		return m_centre;
	}
	
	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::canSubdivide() const
	{
		const auto threshold = MIN_NODE_SIZE * 2;
		return ((m_max_bounds.x - m_min_bounds.x) > threshold) &&
			   ((m_max_bounds.y - m_min_bounds.y) > threshold);
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::isRoot() const
	{
		return !hasParent();
	}
	
	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::hasParent() const
	{
		return (m_parent != NO_NODE);
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::isBranch() const
	{
		return hasChildren();
	}
	
	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::isLeaf() const
	{
		return !hasChildren();
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::hasChildren() const
	{
		return (m_children[0] != NO_NODE);
	}

	template <typename T, typename TCoord>
	const std::array<typename Quadtree<T, TCoord>::NodeIndex, 4>& Quadtree<T, TCoord>::Node::getChildren() const
	{
		return m_children;
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::Node::setChildren(std::array<NodeIndex, 4> children)
	{
		m_children = children;
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::canAcceptItems() const
	{
		return m_items.size() < MAX_NODE_ITEMS;
	}
	
	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::Node::addItemDirect(T item)
	{
		m_items.push_back(item);
	}

	template <typename T, typename TCoord>
	const std::vector<T>& Quadtree<T, TCoord>::Node::getItems() const
	{
		return m_items;
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::Node::getItems(std::vector<T>& items) const
	{
		m_items.insert(m_items.end(), items);
	}

	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::Node::setItems(std::vector<T>&& items)
	{
		m_items = items;
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::removeItemDirect(T item)
	{
		const auto it = std::find_if(m_items.begin(), m_items.end(), [item](const T& el) { return item == el; });
		if (it != m_items.end())
		{
			m_items.erase(it);
			return true;
		}

		return false;
	}
	
	template <typename T, typename TCoord>
	void Quadtree<T, TCoord>::Node::clearItemsDirect()
	{
		m_items.clear();
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::containsPoint(Vec2<TCoord> point) const
	{
		return point.x >= m_min_bounds.x && point.x < m_max_bounds.x &&
			point.y >= m_min_bounds.y && point.y < m_max_bounds.y;
	}

	template <typename T, typename TCoord>
	bool Quadtree<T, TCoord>::Node::containsRegion(Vec2<TCoord> minPoint, Vec2<TCoord> maxPoint) const
	{
		return !(								// Check FAILS if:
			minPoint.x >= m_max_bounds.x ||		//  1. region left is to the right of our right bound, or
			maxPoint.x < m_min_bounds.x ||		//  2. region right is to the left of our left bound, or
			minPoint.y >= m_max_bounds.y ||     //  3. region bottom is above our topmost bound, or
			maxPoint.y < m_min_bounds.y			//  4. region top is below our bottom-most bound
			);
	}

	template <typename T, typename TCoord>
	typename Quadtree<T, TCoord>::ChildNode Quadtree<T, TCoord>::Node::getChildContainingPoint(Vec2<TCoord> point) const
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
