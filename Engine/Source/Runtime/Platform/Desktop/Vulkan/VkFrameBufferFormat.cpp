#include "VkFrameBufferFormat.h"

#include <iterator>

namespace Voxium::Platform::Desktop::Vulkan
{
    vk::AttachmentDescription ToVulkan(const Voxium::Platform::Render::FrameBufferAttachmentDescriptor& attachment)
    {
        vk::ImageLayout layout = {};
        switch (attachment.type)
        {
            case Voxium::Platform::Render::FrameBufferAttachmentType::COLOR:
                layout = vk::ImageLayout::eColorAttachmentOptimal;
                break;
            case Voxium::Platform::Render::FrameBufferAttachmentType::DEPTH_STENCIL:
                layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
                break;
                // default:
                // 	throw std::runtime_error("Invalid type.");
        }
        return vk::AttachmentDescription {{},
                                          ToVulkanFormat(attachment.format),
                                          vk::SampleCountFlagBits::e1,
                                          vk::AttachmentLoadOp::eClear,
                                          vk::AttachmentStoreOp::eStore,
                                          vk::AttachmentLoadOp::eDontCare,
                                          vk::AttachmentStoreOp::eDontCare,
                                          vk::ImageLayout::eUndefined,
                                          layout};
    }

    FrameBufferFormat::FrameBufferFormat(std::shared_ptr<VulkanContext>                                                 context,
                                         std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor>         attachments,
                                         const std::vector<Voxium::Platform::Render::FrameBufferRenderStageDescriptor>& renderStages) :
        context_(std::move(context)), attachments_(std::move(attachments))
    {
        std::vector<vk::AttachmentDescription> attachmentDescriptions;
        std::vector<vk::AttachmentReference>   attachmentReferences;
        std::vector<uint32_t>                  allAttachments;
        attachmentDescriptions.reserve(attachments_.size());
        attachmentReferences.reserve(attachments_.size());
        for (const auto& attachment : attachments_)
        {
            const auto i           = static_cast<uint32_t>(attachmentDescriptions.size());
            const auto description = ToVulkan(attachment);
            attachmentDescriptions.push_back(description);
            attachmentReferences.emplace_back(i, description.finalLayout);
            allAttachments.push_back(i);
        }

        std::vector<vk::SubpassDescription>               subpassDescriptions;
        std::vector<vk::SubpassDependency>                subpassDependencies;
        std::vector<std::vector<vk::AttachmentReference>> colorAttachments;
        std::vector<std::vector<uint32_t>>                otherAttachments;
        colorAttachments.resize(renderStages.size());
        otherAttachments.resize(renderStages.size());

        auto stageID = 0u;
        for (const auto& renderStage : renderStages)
        {
            for (auto attachment : renderStage.colorAttachments)
                colorAttachments[stageID].push_back(attachmentReferences[attachment]);

            const auto* depthStencilAttachment = renderStage.depthStencilAttachment != UINT32_MAX ?
                                                     &attachmentReferences[renderStage.depthStencilAttachment] :
                                                     nullptr;

            auto& other = otherAttachments[stageID];
            std::set_difference(allAttachments.begin(),
                                allAttachments.end(),
                                renderStage.colorAttachments.begin(),
                                renderStage.colorAttachments.end(),
                                std::back_inserter(other));

            other.erase(std::remove(other.begin(), other.end(), renderStage.depthStencilAttachment), other.end());

            subpassDescriptions.push_back({{},
                                           vk::PipelineBindPoint::eGraphics,
                                           {},
                                           colorAttachments[stageID],
                                           {},
                                           depthStencilAttachment,
                                           otherAttachments[stageID]});

            subpassDependencies.push_back(
                {VK_SUBPASS_EXTERNAL,
                 stageID,
                 vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                 vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                 {},
                 vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite});
            for (auto dependency : renderStage.dependencies)
            {
                subpassDependencies.push_back({dependency,
                                               stageID,
                                               vk::PipelineStageFlagBits::eBottomOfPipe,
                                               vk::PipelineStageFlagBits::eTopOfPipe,
                                               {},
                                               {}});
            }

            stageID++;
        }

        renderPass_ = context_->device_->createRenderPassUnique(
            {{}, attachmentDescriptions, subpassDescriptions, subpassDependencies});
    }

    FrameBufferFormat::FrameBufferFormat(std::shared_ptr<VulkanContext>                       context,
                                         vk::UniqueRenderPass                                 renderPass,
                                         std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor> attachments) :
        context_(std::move(context)), renderPass_(std::move(renderPass)), attachments_(std::move(attachments))
    {}
} // namespace Voxium::Platform::Desktop::Vulkan
