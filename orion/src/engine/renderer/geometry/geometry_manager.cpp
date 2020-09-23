#include "vertex_definitions.h"
#include "vertex_definition_loader.h"
#include "../../../util/log.h"
#include "../core/renderer_input_state.h"

#include "geometry_manager.h"


namespace Orion
{
	GeometryManager::GeometryManager()
	{
	}

	ResultCode GeometryManager::initialise()
	{
		LOG_INFO("Initialise geometry manager");

		RETURN_ON_ERROR(initialiseVertexDefinitions());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::initialiseVertexDefinitions()
	{
		LOG_INFO("Initialising vertex definitions");

		RETURN_ON_ERROR(VertexDefinitions::initialiseDefinitions());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::beginFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode GeometryManager::executeFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	ResultCode GeometryManager::endFrame(const RendererInputState& state)
	{
		(void)state;
		return ResultCodes::Success;
	}

	void GeometryManager::shutdown()
	{
		LOG_INFO("Shutting down geometry manager");
	}
}
