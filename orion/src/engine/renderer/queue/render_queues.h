#pragma once

#include "../queue/render_queue.h"
#include "../queue/render_instance.h"

namespace Orion
{
	class RenderQueues
	{
	public:

		RenderQueues();
		ResultCode initialise();

		const inline RenderQueue<InstanceData>& primary() const { return m_primary; }

		inline RenderQueue<InstanceData>& primary() { return m_primary; }

		void shutdown();

	private:
		RenderQueue<InstanceData> m_primary;
	};
}
