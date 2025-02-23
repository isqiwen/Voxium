#include "Watch/TickWatch.h"

namespace Voxium::Core
{
    TickWatch::TickWatch(int maxValue) : max_(maxValue), maxFunc_(nullptr) { watch_.Start(); }

    TickWatch::TickWatch(std::function<int()> maxFunction) : max_(0), maxFunc_(std::move(maxFunction)) { watch_.Start(); }

    bool TickWatch::Allow()
    {
        if (!watch_.IsRunning())
            return false;

        // Determine the threshold: if maxFunc is set, use its value; otherwise use fixed max.
        int threshold = maxFunc_ ? maxFunc_() : max_;
        if (watch_.ElapsedMilliseconds() > threshold)
        {
            watch_.Restart();
            return true;
        }
        return false;
    }

    void TickWatch::Start() { watch_.Start(); }

    void TickWatch::Restart() { watch_.Restart(); }

    void TickWatch::Stop() { watch_.Stop(); }

    void TickWatch::Reset() { watch_.Reset(); }

    int TickWatch::Elapsed() const { return static_cast<int>(watch_.ElapsedMilliseconds()); }
} // namespace Voxium::Core
