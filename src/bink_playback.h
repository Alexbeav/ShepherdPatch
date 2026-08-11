#pragma once

#include <string_view>

namespace shh
{
enum class BinkTrackedFunction
{
    Wait,
    DoFrame,
    NextFrame,
    Pause,
};

bool ShouldTraceBinkPlayback(std::string_view moviePath);
bool ShouldAdvanceBinkFrame(bool frameRateUnlockEnabled, int binkWaitResult);
std::string_view GetBinkTrackedFunctionName(BinkTrackedFunction function);
} // namespace shh
