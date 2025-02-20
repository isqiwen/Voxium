// IVoxelMap.hpp
#pragma once

#include <array>
#include <cstdint>

namespace Voxium::Core {

    class IVoxelMap {
    public:
        const int MAX_KIND;

        std::array<uint32_t, 256> MagicVoxelColours;

        explicit IVoxelMap(int maxKind)
            : MAX_KIND(maxKind), MagicVoxelColours{} {
        }

        virtual ~IVoxelMap() = default;

        virtual bool BlockKindExists(int index) const = 0;

        virtual bool OutOfBounds(int x, int y, int z) const = 0;

        virtual void AddBlockFromMagicVoxel(int x, int y, int z, int index) = 0;
    };

} // namespace Voxium::Core
