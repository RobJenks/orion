#include <algorithm>
#include "vertex_definitions.h"
#include "vertex_definition_loader.h"
#include "../../../util/log.h"
#include "../core/renderer_input_state.h"

#include "geometry_manager.h"

// === Temporary ===
namespace Orion
{
    const VertexDefinitions::PosColorVertex s_cubeVertices[] =
    {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };
    const uint16_t s_cubeTriList[] =
    {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };
    const VertexDefinitions::PosTexVertex s_quadVertices[] =
    {
        {-1.0f,  1.0f,  1.0f, 0.0f, 1.0f },
        { 1.0f,  1.0f,  1.0f, 1.0f, 1.0f },
        {-1.0f, -1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, -1.0f,  1.0f, 1.0f, 0.0f }
    };
    const uint16_t s_quadTriList[] =
    {
        2, 1, 0, // 0, 1, 2,
        3, 1, 2  // 2, 1, 3
    };
}
// === Temporary ===

namespace Orion
{
	GeometryManager::GeometryManager()
		:
		m_meshes()
	{
	}

	ResultCode GeometryManager::initialise()
	{
		LOG_INFO("Initialise geometry manager");

		RETURN_ON_ERROR(initialiseVertexDefinitions());
		RETURN_ON_ERROR(initialiseGeometryData());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::initialiseVertexDefinitions()
	{
		LOG_INFO("Initialising vertex definitions");

		RETURN_ON_ERROR(VertexDefinitions::initialiseDefinitions());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::initialiseGeometryData()
	{
		LOG_INFO("Initialising geometry data");

		RETURN_ON_ERROR(loadInternalMeshes());

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::loadInternalMeshes()
	{
		ResultCode aggregate = ResultCodes::Success;

		std::vector<std::string> meshes { "cube", "quad" };
		for (const auto& mesh : meshes)
		{
			const auto result = loadInternalMesh(mesh);
			if (!ResultCodes::isSuccess(result) && ResultCodes::isSuccess(aggregate))
			{
				aggregate = result;
			}
		}

		if (!ResultCodes::isSuccess(aggregate))
		{
			LOG_WARN("At least one failure while loading internal mesh data (" << aggregate << ")");
		}

		return ResultCodes::Success;
	}

	ResultCode GeometryManager::loadInternalMesh(const std::string& name)
	{
		if (name.empty()) return ResultCodes::CannotLoadMeshWithInvalidEmptyName;

		LOG_INFO("Loading mesh data for \"" << name << "\"");

		bgfx::VertexBufferHandle vb;
		bgfx::IndexBufferHandle ib;

		if (name == "cube")
		{
			vb = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), VertexDefinitions::PosColorVertex::ms_layout);
			ib = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
		}
		else if (name == "quad")
		{
			vb = bgfx::createVertexBuffer(bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)), VertexDefinitions::PosTexVertex::ms_layout);
			ib = bgfx::createIndexBuffer(bgfx::makeRef(s_quadTriList, sizeof(s_quadTriList)));
		}
		else
		{
			return ResultCodes::CannotLoadMeshWithInvalidName;
		}

		return storeMesh(name, BasicMesh(vb, ib));
	}

	ResultCode GeometryManager::storeMesh(const std::string& name, BasicMesh&& mesh)
	{
		if (m_meshes.find(name) != m_meshes.end())
		{
			LOG_WARN("Cannot store mesh data; mesh already exists with name \"" << name << "\"");
			return ResultCodes::CannotStoreDuplicateMesh;
		}

		m_meshes[name] = mesh;
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

	const BasicMesh GeometryManager::getMesh(const std::string& name) const
	{
        return m_meshes.at(name);
	}

	void GeometryManager::shutdown()
	{
		LOG_INFO("Shutting down geometry manager");

		shutdownGeometryData();
	}

	void GeometryManager::shutdownGeometryData()
	{
		LOG_INFO("Releasing all geometry data");

		std::for_each(m_meshes.begin(), m_meshes.end(), [](auto& mesh) { mesh.second.destroy(); });
        m_meshes.clear();
	}
}
