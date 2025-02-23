#pragma once

#include <string>

#include "CoreMacros.h"

#include "Math/Vector.h"
#include "Math/Vector3.h"
#include "Math/Vector3F.h"

namespace Voxium::Core
{

    //--------------------------------------------------------------------------------
    // Line2: Represents a 2D line defined by a start and end point (type: Vector)
    //--------------------------------------------------------------------------------
    class CORE_API Line2
    {
    public:
        Vector Start;
        Vector End;

        // Constructor
        constexpr Line2(const Vector& start, const Vector& end) : Start(start), End(end) {}

        // Returns the length of the line as the magnitude of (Start - End)
        inline double Length() const { return (Start - End).Magnitude(); }

        // Returns the maximum X coordinate between Start and End
        inline double MaxX() const { return std::max(Start.X, End.X); }

        // Returns the minimum X coordinate between Start and End
        inline double MinX() const { return std::min(Start.X, End.X); }

        // Returns the maximum Y coordinate between Start and End
        inline double MaxY() const { return std::max(Start.Y, End.Y); }

        // Returns the minimum Y coordinate between Start and End
        inline double MinY() const { return std::min(Start.Y, End.Y); }

        // Given an x coordinate, calculates the corresponding y coordinate on the line.
        // Here, slope m = (Start.X - End.X) / (Start.Y - End.Y), then y = m*x + Start.Y.
        // Note: This formulation is based on the C# code provided;
        // typically slope is computed as (y2-y1)/(x2-x1), but we follow the original code.
        constexpr double CalculateYForX(double x) const
        {
            double m = (Start.X - End.X) / (Start.Y - End.Y);
            return m * x + Start.Y;
        }
    };

    //--------------------------------------------------------------------------------
    // Line3: Represents a 3D line defined by a start and end point (type: Vector3)
    //--------------------------------------------------------------------------------
    class CORE_API Line3
    {
    public:
        Vector3 Start;
        Vector3 End;

        // Constructor
        constexpr Line3(const Vector3& start, const Vector3& end) : Start(start), End(end) {}

        // Returns the length of the 3D line as the magnitude of (Start - End)
        inline double Length() const { return (Start - End).Magnitude(); }
    };

    //--------------------------------------------------------------------------------
    // Line3F: Represents a 3D line defined by a start and end point (type: Vector3F)
    //--------------------------------------------------------------------------------
    class CORE_API Line3F
    {
    public:
        Vector3F Start;
        Vector3F End;

        // Constructor
        constexpr Line3F(const Vector3F& start, const Vector3F& end) : Start(start), End(end) {}

        // Returns the length of the 3D line as the magnitude of (Start - End)
        // (Assuming Vector3F::Magnitude() returns a float.)
        inline float Length() const { return (Start - End).Magnitude(); }
    };

} // namespace Voxium::Core
