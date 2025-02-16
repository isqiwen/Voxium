#include "vk_FrameBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    vk::ImageAspectFlags toVulkanAspectFlags(const Voxium::Platform::Render::FrameBufferAttachmentType type)
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

    vk::ImageUsageFlagBits toVulkanUsageFlags(const Voxium::Platform::Render::FrameBufferAttachmentType type)
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
        context_(std::move(context)), m_format(std::move(format)), width_(width), height_(height),
        m_shouldTransition(true)
    {
        const auto& attachments = m_format->GetAttachments();
        m_textures.reserve(attachments.size());

        std::vector<std::vector<vk::ImageView>> FrameBufferViews;
        FrameBufferViews.resize(stages);

        for (const auto& attachment : attachments)
        {
            const auto usageFlags  = toVulkanUsageFlags(attachment.type) | vk::ImageUsageFlagBits::eSampled;
            const auto vkFormat    = util::ToVulkanFormat(attachment.format);
            const auto aspectFlags = toVulkanAspectFlags(attachment.type);

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

                util::TransitionImageLayoutsImmediate(*context_->m_device,
                                                      *context_->m_commandPool,
                                                      context_->m_graphicsQueue,
                                                      {{image->get(),
                                                        aspectFlags,
                                                        vk::ImageLayout::eUndefined,
                                                        attachment.type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR ?
                                                            vk::ImageLayout::eColorAttachmentOptimal :
                                                            vk::ImageLayout::eDepthStencilAttachmentOptimal}});

                images.push_back(std::move(image));
                views.push_back(std::move(view));
            }

            m_textures.push_back(
                std::make_shared<FrameBufferTexture>(context_, std::move(images), std::move(views), width, height));
        }

        m_FrameBuffers = context_->CreateFrameBuffers(m_format->GetPass(), {width, height}, FrameBufferViews);
    }

    FrameBuffer::FrameBuffer(std::shared_ptr<VulkanContext>     context,
                             std::shared_ptr<IFrameBufferFormat> format,
                             const uint32_t                     width,
                             const uint32_t                     height,
                             std::vector<vk::UniqueImageView>   imageViews,
                             std::vector<vk::UniqueFrameBuffer> FrameBuffers) :
        context_(std::move(context)), m_format(std::move(format)), width_(width), height_(height),
        m_FrameBuffers(std::move(FrameBuffers)), m_writeIndex(static_cast<uint32_t>(imageViews.size() - 1)),
        m_shouldTransition(false)
    {
        m_textures.push_back(std::make_shared<FrameBufferTexture>(
            context_, std::vector<std::unique_ptr<VulkanImage>> {}, std::move(imageViews), width, height));
    }

    void FrameBuffer::advance()
    {
        m_writeIndex = getWriteIndex();
        for (auto& texture : m_textures)
            texture->readIndex_ = (m_writeIndex + 1) % m_FrameBuffers.size();
    }

    void FrameBuffer::transitionTexturesPost(const vk::CommandBuffer& cmd)
    {
        if (!m_shouldTransition)
            return;

        const auto& attachments = m_format->GetAttachments();

        auto i = 0u;
        for (auto& texture : m_textures)
        {
            const auto& attachment = attachments[i];
            util::TransitionImageLayouts(cmd,
                                         {{texture->image_s[texture->readIndex_]->get(),
                                           toVulkanAspectFlags(attachment.type),
                                           attachment.type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR ?
                                               vk::ImageLayout::eColorAttachmentOptimal :
                                               vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                           vk::ImageLayout::eShaderReadOnlyOptimal}});
            i++;
        }
    }
} // namespace Voxium::Platform::Desktop::Vulkan
