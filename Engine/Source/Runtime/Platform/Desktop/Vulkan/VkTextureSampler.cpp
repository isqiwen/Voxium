#include "VkTextureSampler.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    vk::SamplerAddressMode ToVulkan(const Voxium::Platform::Render::TextureWrapping wrapping)
    {
        switch (wrapping)
        {
            case Voxium::Platform::Render::TextureWrapping::REPEAT:
                return vk::SamplerAddressMode::eRepeat;
            case Voxium::Platform::Render::TextureWrapping::MIRRORED_REPEAT:
                return vk::SamplerAddressMode::eMirroredRepeat;
            case Voxium::Platform::Render::TextureWrapping::CLAMP_TO_EDGE:
                return vk::SamplerAddressMode::eClampToEdge;
            case Voxium::Platform::Render::TextureWrapping::MIRRORED_CLAMP_TO_EDGE:
                return vk::SamplerAddressMode::eMirrorClampToEdge;
            case Voxium::Platform::Render::TextureWrapping::CLAMP_TO_BORDER:
                return vk::SamplerAddressMode::eClampToBorder;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::Filter ToVulkan(const Voxium::Platform::Render::TextureFiltering filtering)
    {
        switch (filtering)
        {
            case Voxium::Platform::Render::TextureFiltering::LINEAR:
                return vk::Filter::eLinear;
            case Voxium::Platform::Render::TextureFiltering::NEAREST:
                return vk::Filter::eNearest;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::BorderColor ToVulkan(const Voxium::Platform::Render::TextureBorderColor color)
    {
        switch (color)
        {
            case Voxium::Platform::Render::TextureBorderColor::TRANSPARENT_BLACK:
                return vk::BorderColor::eIntTransparentBlack;
            case Voxium::Platform::Render::TextureBorderColor::OPAQUE_BLACK:
                return vk::BorderColor::eIntOpaqueBlack;
            case Voxium::Platform::Render::TextureBorderColor::OPAQUE_WHITE:
                return vk::BorderColor::eIntOpaqueWhite;
        }
        throw std::runtime_error("Invalid type.");
    }

    TextureSampler::TextureSampler(std::shared_ptr<VulkanContext>   context,
                                   const Voxium::Platform::Render::TextureFiltering   minFiltering,
                                   const Voxium::Platform::Render::TextureFiltering   magFiltering,
                                   const Voxium::Platform::Render::TextureWrapping    wrapping,
                                   const Voxium::Platform::Render::TextureBorderColor borderColor,
                                   const bool                       enableAnisotropy,
                                   const uint32_t                   anisotropyLevel) : context_(std::move(context))
    {
        const auto addressMode = ToVulkan(wrapping);
        m_sampler              = context_->CreateTextureSampler(ToVulkan(minFiltering),
                                                    ToVulkan(magFiltering),
                                                    addressMode,
                                                    vk::SamplerMipmapMode::eNearest,
                                                    0.0f,
                                                    0.0f,
                                                    0.0f,
                                                    enableAnisotropy,
                                                    static_cast<float>(anisotropyLevel),
                                                    false,
                                                    vk::CompareOp::eAlways,
                                                    toVulkan(borderColor),
                                                    false);
    }
} // namespace Voxium::Platform::Desktop::Vulkan
