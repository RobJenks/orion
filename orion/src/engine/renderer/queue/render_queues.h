#pragma once

#include "../queue/render_queue.h"
#include "../geometry/vertex_definitions.h"

namespace Orion
{
	class RenderQueues
	{
	public:

		RenderQueues();
		ResultCode initialise();

		const inline RenderQueue<VertexDefinitions::PosTexVertex>& rqTextured() { return m_textured; }

		void shutdown();

	private:
		RenderQueue<VertexDefinitions::PosTexVertex> m_textured;
	};
}
