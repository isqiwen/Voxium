#pragma once

#include "Platform/Render/ITexture.h"

#include "VkContext.h"
#include "VkUtil.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class StaticTexture : public ITexture
    {
    public:
        StaticTexture(std::shared_ptr<VulkanContext> context,
                      const uint32_t                 width,
                      const uint32_t                 height,
                      const Voxium::Platform::Render::TextureFormat    format,
                      const std::vector<uint8_t>&    data);

        [[nodiscard]] uint32_t GetWidth() override { return width_; }
        [[nodiscard]] uint32_t GetHeight() override { return height_; }

        [[nodiscard]] virtual vk::ImageView& Get() override { return *imageView_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        uint32_t                       width_, height_;
        std::unique_ptr<VulkanImage>   image_;
        vk::UniqueImageView            imageView_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
