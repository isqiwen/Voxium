#include "Math/Int2.h"

#include <cmath>
#include <format>

#include "Math/Hash.h"
#include "Math/Vector.h"

namespace Voxium::Core
{
    const int  Int2::INVALID_X = -1;
    const Int2 Int2::Zero      = Int2(0, 0);
    const Int2 Int2::Invalid   = Int2(INVALID_X, 0);

    std::size_t Int2::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y);
        return hash;
    }

    Vector Int2::ToVector() const { return Vector(static_cast<double>(X), static_cast<double>(Y)); }

    std::string Int2::ToString() const { return std::format("({},{})", X, Y); }

} // namespace Voxium::Core
