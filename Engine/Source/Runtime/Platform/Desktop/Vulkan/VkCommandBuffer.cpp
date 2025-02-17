#include "VkCommandBuffer.h"
#include "VkFrameBufferFormat.h"
#include "VkRenderPipeline.h"
#include "VkTextureBinding.h"
#include "VkUniformBinding.h"
#include "VkVertexBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    void CBCmdBegin::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        vk::CommandBufferInheritanceInfo inheritanceInfo {format_->GetPass()};
        cmd.begin(
            {vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eSimultaneousUse,
             &inheritanceInfo});
    }

    void CBCmdEnd::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        cmd.end();
    }

    void CBCmdSetViewportScissor::Record(vk::CommandBuffer&                              cmd,
                                         std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        auto& fb = renderTarget_->GetFrameBuffer();

        cmd.SetViewport(
            0, vk::Viewport {0, 0, static_cast<float>(fb.GetWidth()), static_cast<float>(fb.GetHeight()), 0.0f, 1.0f});
        cmd.SetScissor(0, vk::Rect2D {{0, 0}, {fb.GetWidth(), fb.GetHeight()}});
    }

    void CBCmdSetViewport::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        cmd.SetViewport(0,
                        vk::Viewport {static_cast<float>(extents_.x),
                                      static_cast<float>(extents_.y),
                                      static_cast<float>(extents_.width),
                                      static_cast<float>(extents_.height),
                                      0.0f,
                                      1.0f});
    }

    void CBCmdSetScissor::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        cmd.SetScissor(0,
                       vk::Rect2D {{static_cast<int32_t>(extents_.x), static_cast<int32_t>(extents_.y)},
                                   {extents_.width, extents_.height}});
    }

    void CBCmdBindUniform::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        auto pipeline = std::static_pointer_cast<RenderPipeline>(pipeline_);
        auto ub       = std::static_pointer_cast<UniformBinding>(uniformBinding_);

        cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                               pipeline->GetLayout(),
                               pipeline->GetLayoutOffset(ub->GetShader()) + ub->GetBinding(),
                               {ub->get()},
                               {binding_ * ub->getBindingSize()});

        resources.push_back(pipeline);
        resources.push_back(ub);
    }

    void CBCmdBindTexture::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        auto pipeline = std::static_pointer_cast<RenderPipeline>(pipeline_);
        auto tb       = std::static_pointer_cast<TextureBinding>(binding_);

        cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                               pipeline->GetLayout(),
                               pipeline->GetLayoutOffset(tb->GetShader()) + tb->GetBinding(),
                               {tb->Get()},
                               {});

        resources.push_back(pipeline);
        resources.push_back(tb);
    }

    void CBCmdDraw::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources)
    {
        auto p = std::static_pointer_cast<RenderPipeline>(pipeline_);
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, p->Get());

        auto vb = std::static_pointer_cast<VertexBuffer>(vertexBuffer_);
        if (instanceBuffer_)
        {
            auto ib = std::static_pointer_cast<VertexBuffer>(instanceBuffer_);
            cmd.bindVertexBuffers(0, {vb->Get(), ib->Get()}, {0, 0});
            cmd.Draw(vb->Size(), ib->Size(), 0, 0);
        }
        else
        {
            cmd.bindVertexBuffers(0, {vb->Get()}, {0});
            cmd.Draw(vb->Size(), 1, 0, 0);
        }

        resources.push_back(pipeline_);
        resources.push_back(vertexBuffer_);
        if (instanceBuffer_)
            resources.push_back(instanceBuffer_);
    }

    CommandBuffer::CommandBuffer(std::shared_ptr<VulkanContext> context, const uint32_t stages) :
        context_(std::move(context))
    {
        commandBuffers_ = context_->CreateCommandBuffers(stages, vk::CommandBufferLevel::eSecondary);
        resources_.reserve(stages);
        for (auto i = 0u; i < stages; ++i)
            resources_.emplace_back();
    }

    void CommandBuffer::tick()
    {
        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(commandBuffers_.size());

        if (leftoverWrites_ == 0)
        {
            readIndex_ = writeIndex;
            return;
        }

        auto& cmd       = *commandBuffers_[writeIndex];
        auto& resources = resources_[writeIndex];
        resources.clear();

        for (auto& cbCmd : commandQueue_)
            cbCmd->Record(cmd, resources);

        leftoverWrites_--;
        readIndex_ = writeIndex;
    }

    void CommandBuffer::reserve(const uint32_t stages)
    {
        const auto missing = stages - commandBuffers_.size();
        if (missing <= 0)
            return;

        auto missingBuffers =
            context_->CreateCommandBuffers(static_cast<uint32_t>(missing), vk::CommandBufferLevel::eSecondary);
        commandBuffers_.insert(commandBuffers_.begin(),
                                std::make_move_iterator(missingBuffers.begin()),
                                std::make_move_iterator(missingBuffers.end()));
    }

    void CommandBuffer::BeginRecording(const std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat>& format)
    {
        commandQueue_.clear();
        commandQueue_.push_back(std::make_unique<CBCmdBegin>(std::static_pointer_cast<FrameBufferFormat>(format)));
    }

    void CommandBuffer::SetViewportAndScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget)
    {
        commandQueue_.push_back(std::make_unique<CBCmdSetViewportScissor>(renderTarget));
    }

    void CommandBuffer::SetViewport(const Voxium::Platform::Utils::Extents2D extents)
    {
        commandQueue_.push_back(std::make_unique<CBCmdSetViewport>(extents));
    }

    void CommandBuffer::SetScissor(const Voxium::Platform::Utils::Extents2D extents)
    {
        commandQueue_.push_back(std::make_unique<CBCmdSetScissor>(extents));
    }

    void CommandBuffer::BindUniform(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                                    std::shared_ptr<Voxium::Platform::Render::IUniformBinding> uniformBinding,
                                    uint32_t                                binding)
    {
        commandQueue_.push_back(std::make_unique<CBCmdBindUniform>(pipeline, uniformBinding, binding));
    }

    void CommandBuffer::BindTexture(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                                    std::shared_ptr<Voxium::Platform::Render::ITextureBinding> binding)
    {
        commandQueue_.push_back(std::make_unique<CBCmdBindTexture>(pipeline, binding));
    }

    void CommandBuffer::Draw(const std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                             const std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                             const std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer)
    {
        commandQueue_.push_back(std::make_unique<CBCmdDraw>(pipeline, vertexBuffer, instanceBuffer));
    }

    void CommandBuffer::FinishRecording()
    {
        commandQueue_.push_back(std::make_unique<CBCmdEnd>());

        leftoverWrites_ = static_cast<uint32_t>(commandBuffers_.size());
    }

    vk::CommandBuffer& CommandBuffer::Get() { return *commandBuffers_[readIndex_]; }
} // namespace Voxium::Platform::Desktop::Vulkan
