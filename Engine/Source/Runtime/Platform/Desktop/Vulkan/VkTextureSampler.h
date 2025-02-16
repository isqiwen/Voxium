#pragma once

#include "Platform/Render/IRenderContext.h"

#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class TextureSampler final : public Voxium::Platform::Render::TextureSampler
    {
    public:
        TextureSampler(std::shared_ptr<VulkanContext> context,
                       Voxium::Platform::Render::TextureFiltering       minFiltering,
                       Voxium::Platform::Render::TextureFiltering       magFiltering,
                       Voxium::Platform::Render::TextureWrapping        wrapping,
                       Voxium::Platform::Render::TextureBorderColor     borderColor,
                       bool                           enableAnisotropy,
                       uint32_t                       anisotropyLevel);

        [[nodiscard]] vk::Sampler& get() { return *sampler_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        vk::UniqueSampler              sampler_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
