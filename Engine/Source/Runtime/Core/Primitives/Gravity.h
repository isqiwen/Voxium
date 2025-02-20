#pragma once

namespace Voxium::Core
{

    enum class Gravity : unsigned int
    {
        None             = 0x0000,
        Left             = 0x0001,
        Right            = 0x0002,
        CenterHorizontal = 0x0004,
        Top              = 0x0008,
        Bottom           = 0x0010,
        CenterVertical   = 0x0020,
        Fill             = 0x1000,
        AspectFit        = 0x2000,
        AspectFill       = 0x3000,
        TopLeft          = Top | Left,
        TopRight         = Top | Right,
        TopCenter        = Top | CenterHorizontal,
        BottomLeft       = Bottom | Left,
        BottomRight      = Bottom | Right,
        BottomCenter     = Bottom | CenterHorizontal,
        LeftCenter       = Left | CenterVertical,
        RightCenter      = Right | CenterVertical,
        Center           = CenterVertical | CenterHorizontal,
        HorizontalMask   = Left | Right | CenterHorizontal,
        VerticalMask     = Top | Bottom | CenterVertical,
        ScaleMask        = 0xF000,
    };

    constexpr Gravity operator&(Gravity a, Gravity b)
    {
        return static_cast<Gravity>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    constexpr Gravity operator|(Gravity a, Gravity b)
    {
        return static_cast<Gravity>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }

} // namespace Voxium::Core
