#include "VkTexture.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    StaticTexture::StaticTexture(std::shared_ptr<VulkanContext> context,
                                 const uint32_t                 width,
                                 const uint32_t                 height,
                                 const Voxium::Platform::Render::TextureFormat    format,
                                 const std::vector<uint8_t>&    data) :
        context_(std::move(context)), width_(width), height_(height)
    {
        const auto fmt = ToVulkanFormat(format);
        image_        = context_->CreateImage(width,
                                         height,
                                         fmt,
                                         vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
                                         vk::MemoryPropertyFlagBits::eDeviceLocal);
        imageView_    = context_->CreateImageView(image_->Get(), fmt, vk::ImageAspectFlagBits::eColor);

        auto buf = context_->CreateCpuToGpuTransferBuffer(data.data(), static_cast<uint32_t>(data.size()));

        util::DirectExecuteCommands(
            *context_->device_, *context_->commandPool_, context_->graphicsQueue_, [&](vk::CommandBuffer& cmd) {
                util::TransitionImageLayouts(cmd,
                                             {{image_->Get(),
                                               vk::ImageAspectFlagBits::eColor,
                                               vk::ImageLayout::eUndefined,
                                               vk::ImageLayout::eTransferDstOptimal}});
                util::CopyBufferToImage(cmd, buf->Buffer(), image_->Get(), width, height);
                util::TransitionImageLayouts(cmd,
                                             {{image_->Get(),
                                               vk::ImageAspectFlagBits::eColor,
                                               vk::ImageLayout::eTransferDstOptimal,
                                               vk::ImageLayout::eShaderReadOnlyOptimal}});
            });
    }
} // namespace Voxium::Platform::Desktop::Vulkan
