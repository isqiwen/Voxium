#pragma once

#include <cstddef>
#include <functional>

namespace Voxium::Core
{

    constexpr std::size_t MagicNumber()
    {
        if constexpr (std::numeric_limits<std::size_t>::digits == 32)
        {
            return 0x9e3779b9u;
        }
        else
        {
            return 0x9e3779b97f4a7c15ULL;
        }
    }

    template<typename T>
    inline void HashCombine(std::size_t& seed, const T& value)
    {
        seed ^= std::hash<T> {}(value) + MagicNumber() + (seed << 6) + (seed >> 2);
    }

    template<typename T, typename... Args>
    void HashCombine(std::size_t& seed, const T& v, Args... rest)
    {
        HashCombine(seed, v);
        if constexpr (sizeof...(Args) > 1)
        {
            HashCombine(seed, rest...);
        }
    }

} // namespace Voxium::Core
