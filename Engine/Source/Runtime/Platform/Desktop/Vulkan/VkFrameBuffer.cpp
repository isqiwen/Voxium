#include "VkFrameBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    vk::ImageAspectFlags ToVulkanAspectFlags(const Voxium::Platform::Render::FrameBufferAttachmentType type)
    {
        switch (type)
        {
            case Voxium::Platform::Render::FrameBufferAttachmentType::COLOR:
                return vk::ImageAspectFlagBits::eColor;
            case Voxium::Platform::Render::FrameBufferAttachmentType::DEPTH_STENCIL:
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::ImageUsageFlagBits ToVulkanUsageFlags(const Voxium::Platform::Render::FrameBufferAttachmentType type)
    {
        switch (type)
        {
            case Voxium::Platform::Render::FrameBufferAttachmentType::COLOR:
                return vk::ImageUsageFlagBits::eColorAttachment;
            case Voxium::Platform::Render::FrameBufferAttachmentType::DEPTH_STENCIL:
                return vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }
        throw std::runtime_error("Invalid type.");
    }

    FrameBuffer::FrameBuffer(std::shared_ptr<VulkanContext>     context,
                             std::shared_ptr<IFrameBufferFormat> format,
                             const uint32_t                     width,
                             const uint32_t                     height,
                             const uint32_t                     stages) :
        context_(std::move(context)), format_(std::move(format)), width_(width), height_(height),
        shouldTransition_(true)
    {
        const auto& attachments = format_->GetAttachments();
        textures_.reserve(attachments.size());

        std::vector<std::vector<vk::ImageView>> FrameBufferViews;
        FrameBufferViews.resize(stages);

        for (const auto& attachment : attachments)
        {
            const auto usageFlags  = ToVulkanUsageFlags(attachment.type) | vk::ImageUsageFlagBits::eSampled;
            const auto vkFormat    = util::ToVulkanFormat(attachment.format);
            const auto aspectFlags = ToVulkanAspectFlags(attachment.type);

            std::vector<std::unique_ptr<VulkanImage>> images;
            std::vector<vk::UniqueImageView>          views;
            images.reserve(stages);
            views.reserve(stages);
            for (auto i = 0u; i < stages; ++i)
            {
                auto image = context_->CreateImage(
                    width, height, vkFormat, usageFlags, vk::MemoryPropertyFlagBits::eDeviceLocal);
                auto view = context_->CreateImageView(image->get(), vkFormat, aspectFlags);

                FrameBufferViews[i].push_back(*view);

                util::TransitionImageLayoutsImmediate(*context_->device_,
                                                      *context_->commandPool_,
                                                      context_->graphicsQueue_,
                                                      {{image->get(),
                                                        aspectFlags,
                                                        vk::ImageLayout::eUndefined,
                                                        attachment.type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR ?
                                                            vk::ImageLayout::eColorAttachmentOptimal :
                                                            vk::ImageLayout::eDepthStencilAttachmentOptimal}});

                images.push_back(std::move(image));
                views.push_back(std::move(view));
            }

            textures_.push_back(
                std::make_shared<FrameBufferTexture>(context_, std::move(images), std::move(views), width, height));
        }

        frameBuffers_ = context_->CreateFrameBuffers(format_->GetPass(), {width, height}, FrameBufferViews);
    }

    FrameBuffer::FrameBuffer(std::shared_ptr<VulkanContext>     context,
                             std::shared_ptr<IFrameBufferFormat> format,
                             const uint32_t                     width,
                             const uint32_t                     height,
                             std::vector<vk::UniqueImageView>   imageViews,
                             std::vector<vk::UniqueFrameBuffer> FrameBuffers) :
        context_(std::move(context)), format_(std::move(format)), width_(width), height_(height),
        frameBuffers_(std::move(FrameBuffers)), writeIndex_(static_cast<uint32_t>(imageViews.size() - 1)),
        shouldTransition_(false)
    {
        textures_.push_back(std::make_shared<FrameBufferTexture>(
            context_, std::vector<std::unique_ptr<VulkanImage>> {}, std::move(imageViews), width, height));
    }

    void FrameBuffer::Advance();
    {
        writeIndex_ = GetWriteIndex();
        for (auto& texture : textures_)
            texture->readIndex_ = (writeIndex_ + 1) % frameBuffers_.size();
    }

    void FrameBuffer::TransitionTexturesPost(const vk::CommandBuffer& cmd)
    {
        if (!shouldTransition_)
            return;

        const auto& attachments = format_->GetAttachments();

        auto i = 0u;
        for (auto& texture : textures_)
        {
            const auto& attachment = attachments[i];
            util::TransitionImageLayouts(cmd,
                                         {{texture->image_s[texture->readIndex_]->get(),
                                           ToVulkanAspectFlags(attachment.type),
                                           attachment.type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR ?
                                               vk::ImageLayout::eColorAttachmentOptimal :
                                               vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                           vk::ImageLayout::eShaderReadOnlyOptimal}});
            i++;
        }
    }
} // namespace Voxium::Platform::Desktop::Vulkan
