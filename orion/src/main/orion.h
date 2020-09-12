#pragma once

#include "common.h"
#include "../debug/render_stats.h"

namespace Orion
{
    class Orion : public entry::AppI
    {
    public:

        Orion(const char* _name, const char* _description, const char* _url);

        void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

        virtual int shutdown() override;

        bool update() override;


    private:

        entry::MouseState m_mouseState;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;

		RenderStats m_renderStats;
    };


    // Temporary
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
};
