#pragma once

#include <array>
#include "bgfx/bgfx.h"

namespace Orion
{
    class RenderStats
    {
    public:
        static const size_t FPS_CALC_SAMPLE_COUNT = 128;
        static const double FPS_CALC_SAMPLES[FPS_CALC_SAMPLE_COUNT];
        static const double FPS_CALC_INTERVAL_MS;


        RenderStats();

        void frame(const bgfx::Stats * stats);

        double getFps() const;

    private:

        void recordFpsSample(double frameMs);
        double calculateFps(void) const;

    private:
        double m_fps;												// Current sample-averaged FPS value
        double m_timeToNextFpsCalc;									// Time remaining (ms) until the next FPS calculation
        size_t m_fpsSampleIndex;									// Index of the next FPS sample to be collected
        std::array<double, FPS_CALC_SAMPLE_COUNT> m_fpsSamples;     // Samples collected for the next FPS calculation
        
        
    };
}
