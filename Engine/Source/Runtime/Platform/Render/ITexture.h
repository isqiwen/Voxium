#pragma once

#include <memory>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    enum class TextureFormat : uint8_t
    {
        R8G8B8A8_SRGB,
        B8G8R8A8_SRGB,
        R32G32B32A32S,
        D32SFLOAT_S8UINT = 0xFF
    };

    class ITexture : public IResource, public std::enable_shared_from_this<ITexture>
    {
    public:
        [[nodiscard]] virtual uint32_t GetWidth()  = 0;
        [[nodiscard]] virtual uint32_t GetHeight() = 0;
    };
} // namespace Voxium::Platform::Render
