#include "bink_playback.h"

#include "intro_skip.h"

namespace shh
{
bool ShouldTraceBinkPlayback(std::string_view moviePath)
{
    return ShouldApplyMenuMovieWaitFix(moviePath);
}

bool ShouldAdvanceBinkFrame(bool frameRateUnlockEnabled, int binkWaitResult)
{
    return !frameRateUnlockEnabled || binkWaitResult == 0;
}

std::string_view GetBinkTrackedFunctionName(BinkTrackedFunction function)
{
    switch (function)
    {
    case BinkTrackedFunction::Wait:
        return "BinkWait";
    case BinkTrackedFunction::DoFrame:
        return "BinkDoFrame";
    case BinkTrackedFunction::NextFrame:
        return "BinkNextFrame";
    case BinkTrackedFunction::Pause:
        return "BinkPause";
    }

    return "BinkUnknown";
}
} // namespace shh
