#include "vertex_definitions.h"
#include "vertex_definition_loader.h"

#include "geometry_manager.h"


namespace Orion
{
	GeometryManager::GeometryManager()
	{
	}

	ResultCode GeometryManager::initialise()
	{
		RETURN_ON_ERROR(initialiseVertexDefinitions());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::initialiseVertexDefinitions()
	{
		RETURN_ON_ERROR(VertexDefinitions::initialiseDefinitions());

		return ResultCodes::Success;
	}

	void GeometryManager::shutdown()
	{
	}
}
