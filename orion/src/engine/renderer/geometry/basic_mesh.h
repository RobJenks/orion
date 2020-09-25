#pragma once

#include "bgfx_utils.h"

namespace Orion
{
	struct BasicMesh
	{
	public:

		bgfx::VertexBufferHandle	vertex_buffer;
		bgfx::IndexBufferHandle		index_buffer;

	public:

		BasicMesh();
		BasicMesh(bgfx::VertexBufferHandle vb, bgfx::IndexBufferHandle ib);

		bool isValid();
		void destroy();

	};
}
