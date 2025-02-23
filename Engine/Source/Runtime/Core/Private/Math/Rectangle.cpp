#include "Math/Rectangle.h"

#include <cmath>
#include <format>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Size.h"
#include "Math/Vector.h"

namespace Voxium::Core
{
    const Rectangle Rectangle::Empty = Rectangle();

    Rectangle operator*(const Rectangle& rect, double scale)
    {
        return Rectangle(rect.TopLeft() * scale, rect.Size() * scale);
    }

    Rectangle operator/(const Rectangle& rect, double scale)
    {
        return Rectangle(rect.TopLeft() / scale, rect.Size() / scale);
    }

    Rectangle::Rectangle(const Vector& topleft, const Voxium::Core::Size& size) :
        Left(topleft.X), Top(topleft.Y), Width(size.Width), Height(size.Height)
    {}

    // ===================================================================
    // Public Member Function Definitions
    // ===================================================================

    bool Rectangle::operator==(const Rectangle& other) const { return IsEqual(other); }
    bool Rectangle::operator!=(const Rectangle& other) const { return !IsEqual(other); }

    bool Rectangle::IsEqual(const Rectangle& other) const
    {
        return IsNearlyEqual(Left, other.Left) && IsNearlyEqual(Top, other.Top) && IsNearlyEqual(Width, other.Width) &&
               IsNearlyEqual(Height, other.Height);
    }

    Vector Rectangle::Center() const { return Vector(CenterX(), CenterY()); }

    Vector Rectangle::CenterSnapped() const { return Vector(std::floor(CenterX()), std::floor(CenterY())); }

    bool Rectangle::Contains(const Vector& point) const
    {
        return IsGreaterThanOrEqual(point.X, Left) && IsLessThan(point.X, Right()) &&
               IsGreaterThanOrEqual(point.Y, Top) && IsLessThan(point.Y, Bottom());
    }

    Vector Rectangle::TopLeft() const { return Vector(Left, Top); }

    void Rectangle::SetTopLeft(const Vector& v)
    {
        Left = v.X;
        Top  = v.Y;
    }

    Vector Rectangle::TopRight() const { return Vector(Right(), Top); }

    void Rectangle::SetTopRight(const Vector& v)
    {
        SetRight(v.X);
        Top = v.Y;
    }

    Vector Rectangle::BottomLeft() const { return Vector(Left, Bottom()); }

    Vector Rectangle::BottomRight() const { return Vector(Right(), Bottom()); }

    void Rectangle::SetBottomRight(const Vector& v)
    {
        Vector             newVector = v - TopLeft();
        Voxium::Core::Size newSize(newVector.X, newVector.Y);
        SetSize(newSize);
    }

    Voxium::Core::Size Rectangle::Size() const { return Voxium::Core::Size(Width, Height); }

    void Rectangle::SetSize(const Voxium::Core::Size& s)
    {
        Width  = s.Width;
        Height = s.Height;
    }

    Rectangle Rectangle::Clip(const Rectangle& outer) const
    {
        double newLeft   = std::max(Left, outer.Left);
        double newTop    = std::max(Top, outer.Top);
        double newRight  = std::min(Right(), outer.Right());
        double newBottom = std::min(Bottom(), outer.Bottom());
        return Rectangle(newLeft, newTop, newRight - newLeft, newBottom - newTop);
    }

    Rectangle Rectangle::ApplyGravity(const Voxium::Core::Size& size, Gravity align) const
    {
        Gravity scalePart = align & Gravity::ScaleMask;
        if (scalePart == Gravity::Fill)
        {
            return *this;
        }
        else if (scalePart == Gravity::AspectFill)
        {
            double             scale   = std::max(this->Size().Width / size.Width, this->Size().Height / size.Height);
            Voxium::Core::Size newSize = size * scale;
            Vector             newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
            return Rectangle(newTopLeft, newSize);
        }
        else if (scalePart == Gravity::AspectFit)
        {
            double             scale   = std::min(this->Size().Width / size.Width, this->Size().Height / size.Height);
            Voxium::Core::Size newSize = size * scale;
            Vector             newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
            return Rectangle(newTopLeft, newSize);
        }

        double  newLeft;
        Gravity horizontalPart = align & Gravity::HorizontalMask;
        if (horizontalPart == Gravity::Right)
        {
            newLeft = Right() - size.Width;
        }
        else if (horizontalPart == Gravity::CenterHorizontal)
        {
            newLeft = (Left + Right() - size.Width) / 2.0;
        }
        else
        {
            newLeft = Left;
        }

        double  newTop;
        Gravity verticalPart = align & Gravity::VerticalMask;
        if (verticalPart == Gravity::Bottom)
        {
            newTop = Bottom() - size.Height;
        }
        else if (verticalPart == Gravity::CenterVertical)
        {
            newTop = (Top + Bottom() - size.Height) / 2.0;
        }
        else
        {
            newTop = Top;
        }

        return Rectangle(newLeft, newTop, size.Width, size.Height);
    }

    size_t Rectangle::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, Left, Top, Width, Height);
        return hash;
    }

    std::string Rectangle::ToString() const
    {
        return std::format("[{:.2f},{:.2f} {:.2f}x{:.2f}]", Left, Top, Width, Height);
    }

} // namespace Voxium::Core
