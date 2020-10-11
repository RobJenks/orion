#include "render_queues.h"

namespace Orion
{
	RenderQueues::RenderQueues()
		:
		m_primary("Primary")
	{
	}

	ResultCode RenderQueues::initialise()
	{
		LOG_INFO("Initialising render queues");

		// Initialise each render queue in turn
		ResultCode result = ResultCodes::Success;

		result = ResultCodes::aggregate(result, m_primary.initialise());

		return result;
	}

	void RenderQueues::shutdown()
	{
		LOG_INFO("Shutting down render queues");

		// Shutdown each render queue in turn
		m_primary.shutdown();
	}
}
