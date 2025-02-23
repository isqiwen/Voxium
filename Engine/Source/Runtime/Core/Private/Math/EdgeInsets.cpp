#include "Math/EdgeInsets.h"

#include <format>

#include "Math/Hash.h"
#include "Math/MathUtility.h"

namespace Voxium::Core
{

    bool EdgeInsets::operator==(const EdgeInsets& other) const { return IsEqual(other); }
    bool EdgeInsets::operator!=(const EdgeInsets& other) const { return !IsEqual(other); }

    bool EdgeInsets::IsEqual(const EdgeInsets& other) const
    {
        return IsNearlyEqual(Left, other.Left) && IsNearlyEqual(Top, other.Top) && IsNearlyEqual(Right, other.Right) && IsNearlyEqual(Bottom, other.Bottom);
    }

    size_t EdgeInsets::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, Left, Right, Top, Bottom);
        return hash;
    }

    std::string EdgeInsets::ToString() const { return std::format("Top: {} Left: {} Bottom: {} Right: {}", Top, Left, Bottom, Right); }

} // namespace Voxium::Core
