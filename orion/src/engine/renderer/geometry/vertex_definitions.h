#pragma once

#include <stdint.h>
#include "bgfx_utils.h"

namespace Orion
{
	class VertexDefinitions
	{
	public:

		// Vertex with position and ABGR colour components
		struct PosColorVertex
		{
			float m_x;
			float m_y;
			float m_z;
			uint32_t m_abgr;

			static bgfx::VertexLayout ms_layout;
			static void init()
			{
				ms_layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.end();
			};
		};

		// Vertex with position and texcoord components
		struct PosTexVertex
		{
			float m_x;
			float m_y;
			float m_z;
			float m_u;
			float m_v;

			static bgfx::VertexLayout ms_layout;
			static void init()
			{
				ms_layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();
			};
		};

	};
}
