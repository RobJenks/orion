#include <numeric>

#include "../../../util/log.h"
#include "../core/renderer_input_state.h"
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

	ResultCode RenderStats::initialise()
	{
		LOG_INFO("Initialise render stats");

		return ResultCodes::Success;
	}

	ResultCode RenderStats::beginFrame(const RendererInputState& state)
	{
        (void)state;    // Unused

        // Calculate all stats for the prior frame
        const auto stats = bgfx::getStats();

        // Calculate FPS
        const double cpuTimerFreqToMs = 1000.0 / stats->cpuTimerFreq;
        const double frameMs = double(stats->cpuTimeFrame) * cpuTimerFreqToMs;

        recordFpsSample(frameMs);

        if ((m_timeToNextFpsCalc -= frameMs) <= 0)
        {
            m_fps = calculateFps();
            m_timeToNextFpsCalc += FPS_CALC_INTERVAL_MS;
        }

        return ResultCodes::Success;
	}

	ResultCode RenderStats::executeFrame(const RendererInputState& state)
	{
        (void)state;    // Unused

        return ResultCodes::Success;
	}

    ResultCode RenderStats::endFrame(const RendererInputState& state)
    {
        (void)state;    // Unused

        return ResultCodes::Success;
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

	void RenderStats::shutdown()
	{
		LOG_INFO("Shutting down render stats");

	}
}
