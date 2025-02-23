#pragma once

#include <cmath>
#include <limits>
#include <numbers>

#include "CoreMacros.h"

namespace Voxium::Core
{
    constexpr float  VOXIUM_FLOAT_TOLERANCE  = std::numeric_limits<float>::epsilon();
    constexpr double VOXIUM_DOUBLE_TOLERANCE = std::numeric_limits<double>::epsilon();

    constexpr float  VOXIUM_FLOAT_PI  = std::numbers::pi_v<float>;
    constexpr double VOXIUM_DOUBLE_PI = std::numbers::pi_v<double>;

    constexpr int VOXIUM_INT_MAX = std::numeric_limits<int>::max();

    [[nodiscard]] CORE_API inline bool IsNearlyEqual(float A, float B, float errorTolerance = VOXIUM_FLOAT_TOLERANCE)
    {
        return std::abs(A - B) <= errorTolerance;
    }

    [[nodiscard]] CORE_API inline bool IsNearlyEqual(double A, double B, double errorTolerance = VOXIUM_DOUBLE_TOLERANCE)
    {
        return std::abs(A - B) <= errorTolerance;
    }

    [[nodiscard]] CORE_API inline bool IsNearlyZero(float value, float errorTolerance = VOXIUM_FLOAT_TOLERANCE) { return std::abs(value) <= errorTolerance; }

    [[nodiscard]] CORE_API inline bool IsNearlyZero(double value, double errorTolerance = VOXIUM_DOUBLE_TOLERANCE) { return std::abs(value) <= errorTolerance; }

    [[nodiscard]] CORE_API inline bool IsLessThan(float A, float B, float tolerance = VOXIUM_FLOAT_TOLERANCE) { return A < B - tolerance; }

    [[nodiscard]] CORE_API inline bool IsLessThan(double A, double B, double tolerance = VOXIUM_DOUBLE_TOLERANCE) { return A < B - tolerance; }

    [[nodiscard]] CORE_API inline bool IsGreaterThan(float A, float B, float tolerance = VOXIUM_FLOAT_TOLERANCE) { return A > B + tolerance; }

    [[nodiscard]] CORE_API inline bool IsGreaterThan(double A, double B, double tolerance = VOXIUM_DOUBLE_TOLERANCE) { return A > B + tolerance; }

    [[nodiscard]] CORE_API inline bool IsLessThanOrEqual(float A, float B, float tolerance = VOXIUM_FLOAT_TOLERANCE)
    {
        return A < B - tolerance || IsNearlyEqual(A, B, tolerance);
    }

    [[nodiscard]] CORE_API inline bool IsLessThanOrEqual(double A, double B, double tolerance = VOXIUM_DOUBLE_TOLERANCE)
    {
        return A < B - tolerance || IsNearlyEqual(A, B, tolerance);
    }

    [[nodiscard]] CORE_API inline bool IsGreaterThanOrEqual(float A, float B, float tolerance = VOXIUM_FLOAT_TOLERANCE)
    {
        return A > B + tolerance || IsNearlyEqual(A, B, tolerance);
    }

    [[nodiscard]] CORE_API inline bool IsGreaterThanOrEqual(double A, double B, double tolerance = VOXIUM_DOUBLE_TOLERANCE)
    {
        return A > B + tolerance || IsNearlyEqual(A, B, tolerance);
    }

} // namespace Voxium::Core
