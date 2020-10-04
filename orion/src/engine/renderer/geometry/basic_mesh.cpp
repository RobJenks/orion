#include "basic_mesh.h"

namespace Orion
{
	BasicMesh::BasicMesh()
		:
		BasicMesh(BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE)
	{
	}

	BasicMesh::BasicMesh(bgfx::VertexBufferHandle vb, bgfx::IndexBufferHandle ib)
		:
		vertex_buffer(vb),
		index_buffer(ib)
	{
	}

	bool BasicMesh::isValid()
	{
		return bgfx::isValid(vertex_buffer)
			&& bgfx::isValid(index_buffer);
	}

	void BasicMesh::destroy()
	{
		bgfx::destroy(vertex_buffer);
		bgfx::destroy(index_buffer);
	}
}
