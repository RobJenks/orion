#include "vertex_definition_loader.h"
#include "vertex_definitions.h"

#define DEFINE_LAYOUT(DEF) bgfx::VertexLayout VertexDefinitions::DEF::ms_layout
#define DEFINE_LAYOUT(DEF) bgfx::VertexLayout VertexDefinitions::DEF::ms_layout

namespace Orion
{
	DEFINE_LAYOUT(PosColorVertex);
	DEFINE_LAYOUT(PosTexVertex);

	ResultCode VertexDefinitions::initialiseDefinitions()
	{
		RETURN_ON_ERROR(VertexDefinitionLoader<VertexDefinitions::PosColorVertex>::load());
		RETURN_ON_ERROR(VertexDefinitionLoader<VertexDefinitions::PosTexVertex>::load());

		return ResultCodes::Success;
	}
}
