#include "VkCommandBuffer.h"
#include "VkFrameBufferFormat.h"
#include "VkRenderPipeline.h"
#include "VkTextureBinding.h"
#include "VkUniformBinding.h"
#include "VkVertexBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    void CBCmdBegin::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        vk::CommandBufferInheritanceInfo inheritanceInfo {m_format->GetPass()};
        cmd.begin(
            {vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eSimultaneousUse,
             &inheritanceInfo});
    }

    void CBCmdEnd::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        cmd.end();
    }

    void CBCmdSetViewportScissor::Record(vk::CommandBuffer&                              cmd,
                                         std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        auto& fb = m_renderTarget->GetFrameBuffer();

        cmd.SetViewport(
            0, vk::Viewport {0, 0, static_cast<float>(fb.getWidth()), static_cast<float>(fb.getHeight()), 0.0f, 1.0f});
        cmd.SetScissor(0, vk::Rect2D {{0, 0}, {fb.getWidth(), fb.getHeight()}});
    }

    void CBCmdSetViewport::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        cmd.SetViewport(0,
                        vk::Viewport {static_cast<float>(m_extents.x),
                                      static_cast<float>(m_extents.y),
                                      static_cast<float>(m_extents.width),
                                      static_cast<float>(m_extents.height),
                                      0.0f,
                                      1.0f});
    }

    void CBCmdSetScissor::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        cmd.SetScissor(0,
                       vk::Rect2D {{static_cast<int32_t>(m_extents.x), static_cast<int32_t>(m_extents.y)},
                                   {m_extents.width, m_extents.height}});
    }

    void CBCmdBindUniform::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        auto pipeline = std::static_pointer_cast<RenderPipeline>(m_pipeline);
        auto ub       = std::static_pointer_cast<UniformBinding>(m_uniformBinding);

        cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                               pipeline->getLayout(),
                               pipeline->getLayoutOffset(ub->getShader()) + ub->getBinding(),
                               {ub->get()},
                               {binding_ * ub->getBindingSize()});

        resources.push_back(pipeline);
        resources.push_back(ub);
    }

    void CBCmdBindTexture::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        auto pipeline = std::static_pointer_cast<RenderPipeline>(m_pipeline);
        auto tb       = std::static_pointer_cast<TextureBinding>(binding_);

        cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                               pipeline->getLayout(),
                               pipeline->getLayoutOffset(tb->getShader()) + tb->getBinding(),
                               {tb->get()},
                               {});

        resources.push_back(pipeline);
        resources.push_back(tb);
    }

    void CBCmdDraw::Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources)
    {
        auto p = std::static_pointer_cast<RenderPipeline>(m_pipeline);
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, p->get());

        auto vb = std::static_pointer_cast<VertexBuffer>(m_vertexBuffer);
        if (instance_Buffer)
        {
            auto ib = std::static_pointer_cast<VertexBuffer>(instance_Buffer);
            cmd.bindVertexBuffers(0, {vb->get(), ib->get()}, {0, 0});
            cmd.Draw(vb->size(), ib->size(), 0, 0);
        }
        else
        {
            cmd.bindVertexBuffers(0, {vb->get()}, {0});
            cmd.Draw(vb->size(), 1, 0, 0);
        }

        resources.push_back(m_pipeline);
        resources.push_back(m_vertexBuffer);
        if (instance_Buffer)
            resources.push_back(instance_Buffer);
    }

    CommandBuffer::CommandBuffer(std::shared_ptr<VulkanContext> context, const uint32_t stages) :
        context_(std::move(context))
    {
        m_commandBuffers = context_->CreateCommandBuffers(stages, vk::CommandBufferLevel::eSecondary);
        m_resources.reserve(stages);
        for (auto i = 0u; i < stages; ++i)
            m_resources.emplace_back();
    }

    void CommandBuffer::tick()
    {
        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(m_commandBuffers.size());

        if (leftoverWrites_ == 0)
        {
            readIndex_ = writeIndex;
            return;
        }

        auto& cmd       = *m_commandBuffers[writeIndex];
        auto& resources = m_resources[writeIndex];
        resources.clear();

        for (auto& cbCmd : m_commandQueue)
            cbCmd->Record(cmd, resources);

        leftoverWrites_--;
        readIndex_ = writeIndex;
    }

    void CommandBuffer::reserve(const uint32_t stages)
    {
        const auto missing = stages - m_commandBuffers.size();
        if (missing <= 0)
            return;

        auto missingBuffers =
            context_->CreateCommandBuffers(static_cast<uint32_t>(missing), vk::CommandBufferLevel::eSecondary);
        m_commandBuffers.insert(m_commandBuffers.begin(),
                                std::make_move_iterator(missingBuffers.begin()),
                                std::make_move_iterator(missingBuffers.end()));
    }

    void CommandBuffer::BeginRecording(const std::shared_ptr<Voxium::Platform::Render::FrameBufferFormat>& format)
    {
        m_commandQueue.clear();
        m_commandQueue.push_back(std::make_unique<CBCmdBegin>(std::static_pointer_cast<FrameBufferFormat>(format)));
    }

    void CommandBuffer::SetViewportAndScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdSetViewportScissor>(renderTarget));
    }

    void CommandBuffer::SetViewport(const platform::util::Extents2D extents)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdSetViewport>(extents));
    }

    void CommandBuffer::SetScissor(const platform::util::Extents2D extents)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdSetScissor>(extents));
    }

    void CommandBuffer::BindUniform(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                                    std::shared_ptr<Voxium::Platform::Render::UniformBinding> uniformBinding,
                                    uint32_t                                binding)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdBindUniform>(pipeline, uniformBinding, binding));
    }

    void CommandBuffer::BindTexture(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                                    std::shared_ptr<Voxium::Platform::Render::TextureBinding> binding)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdBindTexture>(pipeline, binding));
    }

    void CommandBuffer::Draw(const std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                             const std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                             const std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer)
    {
        m_commandQueue.push_back(std::make_unique<CBCmdDraw>(pipeline, vertexBuffer, instanceBuffer));
    }

    void CommandBuffer::FinishRecording()
    {
        m_commandQueue.push_back(std::make_unique<CBCmdEnd>());

        leftoverWrites_ = static_cast<uint32_t>(m_commandBuffers.size());
    }

    vk::CommandBuffer& CommandBuffer::get() { return *m_commandBuffers[readIndex_]; }
} // namespace Voxium::Platform::Desktop::Vulkan
