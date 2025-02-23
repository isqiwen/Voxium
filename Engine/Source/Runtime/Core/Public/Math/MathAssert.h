#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <functional>
#include <string>
#include <thread>

#include "Math/Vector.h"
#include "Math/Vector3.h"
#include "Math/Vector3F.h"
#include "Math/VectorF.h"

namespace Voxium::Core
{
    inline int MainThreadID = 0;

#ifdef DEBUG

    // Only in debug builds.
    inline void Assert(bool v) { assert(v); }

    inline void AssertMainThread()
    {
        std::hash<std::thread::id> hasher;
        int                        currentID = static_cast<int>(hasher(std::this_thread::get_id()));
        Assert(MainThreadID == currentID);
    }

    inline void AssertLower(const std::string& s)
    {
        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
        Assert(s == lower);
    }

    inline void AssertFalse() { Assert(false); }

    inline void AssertFinite(double v) { Assert(!IsNaNorInfinity(v)); }

    inline void AssertFinite(float v) { Assert(!IsNaNorInfinity(v)); }

    inline void AssertFinite(const Vector& v) { Assert(!IsNaNorInfinity(v)); }

    inline void AssertFinite(const Vector3& v) { Assert(!IsNaNorInfinity(v)); }

    inline void AssertFinite(const Vector3F& v) { Assert(!IsNaNorInfinity(v)); }
#else
    // In non-debug builds, these functions do nothing.
    inline void Assert(bool) {}
    inline void AssertMainThread() {}
    inline void AssertLower(const std::string&) {}
    inline void AssertFalse() {}
    inline void AssertFinite(double) {}
    inline void AssertFinite(float) {}
    inline void AssertFinite(const Vector&) {}
    inline void AssertFinite(const Vector3&) {}
    inline void AssertFinite(const Vector3F&) {}
#endif

    // Floating point checks.
    inline bool IsNaNorInfinity(double v) { return std::isnan(v) || std::isinf(v); }

    inline bool IsNaNorInfinity(float v) { return std::isnan(v) || std::isinf(v); }

    inline bool IsNaNorInfinity(const Vector& v) { return IsNaNorInfinity(v.X) || IsNaNorInfinity(v.Y); }

    inline bool IsNaNorInfinity(const VectorF& v) { return IsNaNorInfinity(v.X) || IsNaNorInfinity(v.Y); }

    inline bool IsNaNorInfinity(const Vector3& v) { return IsNaNorInfinity(v.X) || IsNaNorInfinity(v.Y) || IsNaNorInfinity(v.Z); }

    inline bool IsNaNorInfinity(const Vector3F& v) { return IsNaNorInfinity(v.X) || IsNaNorInfinity(v.Y) || IsNaNorInfinity(v.Z); }

    inline bool ValidVector3(const Vector3& v) { return IsNaNorInfinity(v.X) && IsNaNorInfinity(v.Y) && IsNaNorInfinity(v.Z); }

    inline bool ValidVector3(const Vector3F& v) { return IsNaNorInfinity(v.X) && IsNaNorInfinity(v.Y) && IsNaNorInfinity(v.Z); }

} // namespace Voxium::Core
