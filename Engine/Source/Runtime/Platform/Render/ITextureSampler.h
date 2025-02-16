#pragma once

#include <memory>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class ITextureSampler : public IResource, public std::enable_shared_from_this<ITextureSampler>
    {
    public:
        ITextureSampler()                                            = default;
        ~ITextureSampler() override                                  = default;
        ITextureSampler(const ITextureSampler& other)                = delete;
        ITextureSampler(ITextureSampler&& other) noexcept            = delete;
        ITextureSampler& operator=(const ITextureSampler& other)     = delete;
        ITextureSampler& operator=(ITextureSampler&& other) noexcept = delete;
    };
} // namespace Voxium::Platform::Render
