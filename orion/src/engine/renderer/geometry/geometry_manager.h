#pragma once

#include <unordered_map>
#include "../../../util/result_code.h"
#include "basic_mesh.h"
struct RendererInputState;

namespace Orion
{
	class GeometryManager
	{
	public:

		GeometryManager();

		ResultCode initialise();

		ResultCode beginFrame(const RendererInputState& state);
		ResultCode executeFrame(const RendererInputState& state);
		ResultCode endFrame(const RendererInputState& state);

		const BasicMesh getMesh(const std::string& name) const;

		void shutdown();

	private:

		ResultCode initialiseVertexDefinitions();
		ResultCode initialiseGeometryData();

		ResultCode loadInternalMeshes();
		ResultCode loadInternalMesh(const std::string& name);

		ResultCode storeMesh(const std::string& name, BasicMesh&& mesh);

		void shutdownGeometryData();

	private:

		std::unordered_map<std::string, BasicMesh> m_meshes;


	};
}
