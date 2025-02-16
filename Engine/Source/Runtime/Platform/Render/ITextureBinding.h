#pragma once

#include <memory>

#include "IResource.h"
#include "ITexture.h"
#include "ITextureSampler.h"

namespace Voxium::Platform::Render
{
    class ITextureBinding : public IResource, public std::enable_shared_from_this<ITextureBinding>
    {
    public:
        ITextureBinding()                                            = default;
        ~ITextureBinding() override                                  = default;
        ITextureBinding(const ITextureBinding& other)                = delete;
        ITextureBinding(ITextureBinding&& other) noexcept            = delete;
        ITextureBinding& operator=(const ITextureBinding& other)     = delete;
        ITextureBinding& operator=(ITextureBinding&& other) noexcept = delete;

        virtual void Update(std::shared_ptr<ITextureSampler> sampler, std::shared_ptr<ITexture> texture) = 0;
    };
} // namespace Voxium::Platform::Render
