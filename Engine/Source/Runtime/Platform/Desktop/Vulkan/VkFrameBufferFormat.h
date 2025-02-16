#pragma once

#include <vulkan.h>

#include "Platform/Render/IFrameBufferFormat.h"
#include "Platform/Render/IRenderContext.h"

#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class FrameBufferFormat final : public Voxium::Platform::Render::IFrameBufferFormat
    {
    public:
        FrameBufferFormat(std::shared_ptr<VulkanContext>                                                 context,
                          std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor>         attachments,
                          const std::vector<Voxium::Platform::Render::FrameBufferRenderStageDescriptor>& renderStages);
        FrameBufferFormat(std::shared_ptr<VulkanContext>                                         context,
                          vk::UniqueRenderPass                                                   renderPass,
                          std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor> attachments);

        [[nodiscard]] const vk::RenderPass& GetPass() const { return *renderPass_; }

        [[nodiscard]] uint32_t GetAttachmentCount() const override
        {
            return static_cast<uint32_t>(attachments_.size());
        }

        [[nodiscard]] const std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor>& GetAttachments() const
        {
            return attachments_;
        }

    private:
        std::shared_ptr<VulkanContext>                       context_;
        vk::UniqueRenderPass                                 renderPass_;
        std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor> attachments_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
