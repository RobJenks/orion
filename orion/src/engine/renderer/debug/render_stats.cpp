#include <numeric>

#include "render_stats.h"

namespace Orion
{
    const double RenderStats::FPS_CALC_INTERVAL_MS = 1000.0f;

    RenderStats::RenderStats()
        :
        m_fps(0.0),
        m_fpsSampleIndex(0),
        m_timeToNextFpsCalc(FPS_CALC_INTERVAL_MS),
		m_fpsSamples({ 0 })
    {
    }

    void RenderStats::frame(const bgfx::Stats * stats)
    {
        // Calculate FPS
        const double cpuTimerFreqToMs = 1000.0 / stats->cpuTimerFreq;
        const double frameMs = double(stats->cpuTimeFrame) * cpuTimerFreqToMs;

        recordFpsSample(frameMs);

		if ((m_timeToNextFpsCalc -= frameMs) <= 0)
		{
			m_fps = calculateFps();
			m_timeToNextFpsCalc += FPS_CALC_INTERVAL_MS;
		}
    }

    void RenderStats::recordFpsSample(double frameMs)
    {
		m_fpsSamples[m_fpsSampleIndex] = frameMs;
		if ((++m_fpsSampleIndex) == RenderStats::FPS_CALC_SAMPLE_COUNT)
		{
			m_fpsSampleIndex = 0;
		}
    }

	double RenderStats::calculateFps() const
	{
		return 1000.0 /
			(std::accumulate(m_fpsSamples.cbegin(), m_fpsSamples.cend(), 0.0) / double(FPS_CALC_SAMPLE_COUNT));
	}

	double RenderStats::getFps() const
	{
		return m_fps;
	}
}
