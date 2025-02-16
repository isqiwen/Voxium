#include "VkRenderContext.h"

#include "../dt_render_surface.h"
#include "vk_command_buffer.h"
#include "vk_render_pipeline.h"
#include "VkShader.h"
#include "VkTextureBinding.h"
#include "vk_texture_sampler.h"
#include "vk_uniforbinding_.h"
#include "vk_uniforbuffer_.h"
#include "VkVertexBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    const vk::Fence NULL_FENCE = nullptr;

    void RenderQueue::clear() { m_queue.clear(); }

    void RenderQueue::Enqueue(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> target,
                              std::shared_ptr<ICommandBuffer>         commandBuffer)
    {
        m_queue.emplace_back(std::move(target), std::move(commandBuffer));
    }

    void RenderQueue::write(vk::CommandBuffer& cmd)
    {
        cmd.begin(vk::CommandBufferBeginInfo {vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        for (auto& [target, buffer] : m_queue)
        {
            auto&       FrameBuffer = reinterpret_cast<FrameBuffer&>(target->GetFrameBuffer());
            const auto& format      = reinterpret_cast<const FrameBufferFormat&>(FrameBuffer.GetFormat());

            std::vector<vk::ClearValue> clearValues;
            clearValues.reserve(format.GetAttachmentCount());
            for (const auto& attachment : format.GetAttachments())
            {
                if (attachment.type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR)
                    clearValues.push_back(vk::ClearColorValue {std::array<float, 4> {0.0f, 0.0f, 0.0f, 0.0f}});
                else
                    clearValues.push_back(vk::ClearDepthStencilValue {1.0f, 0});
            }

            cmd.beginRenderPass({format.GetPass(),
                                 FrameBuffer.getTarget(),
                                 {{0, 0}, {FrameBuffer.getWidth(), FrameBuffer.getHeight()}},
                                 clearValues},
                                vk::SubpassContents::eSecondaryCommandBuffers);

            cmd.executeCommands({buffer->get()});

            cmd.endRenderPass();

            FrameBuffer.TransitionTexturesPost(cmd);

            FrameBuffer.Advance();;
        }
        cmd.end();
    }

    RenderContext::RenderContext(RenderSurface&                   surface,
                                 std::shared_ptr<VulkanContext>&& context,
                                 vk::UniqueSurfaceKHR&&           vkSurface) :
        surface_(surface), context_(std::move(context)), m_vkSurface(std::move(vkSurface)), m_swapChainStages(0),
        m_maxFramesInFlight(0)
    {
        CreateSwapChain();
    }

    RenderContext::~RenderContext() { context_->WaitIdle(); }

    void RenderContext::CreateSwapChain()
    {
        const auto swapChainDesc = context_->GetSwapChainDescriptor(*m_vkSurface);

        m_swapChainStages        = swapChainDesc.GetOptimalImageCount();
        const auto surfaceFormat = swapChainDesc.GetOptimalFormat();
        const auto surfaceExtent = swapChainDesc.GetOptimalExtent(surface_.getWidth(), surface_.getHeight());

        m_swapChain = context_->CreateSwapChain(*m_vkSurface,
                                                 m_swapChainStages,
                                                 surfaceFormat,
                                                 swapChainDesc.GetOptimalPresentMode(),
                                                 surfaceExtent,
                                                 swapChainDesc.GetTransform(),
                                                 *m_swapChain);

        m_commandBuffer = context_->CreateCommandBuffer(m_swapChainStages, vk::CommandBufferLevel::ePrimary);
        m_renderQueues.clear();
        m_renderQueues.reserve(m_swapChainStages);
        for (auto i = 0u; i < m_swapChainStages; ++i)
            m_renderQueues.emplace_back();

        m_maxFramesInFlight       = m_swapChainStages - 1;
        image_AvailableSemaphore = context_->CreateSemaphore(m_maxFramesInFlight);
        m_renderFinishedSemaphore = context_->CreateSemaphore(m_maxFramesInFlight);
        m_inFlightFence           = context_->CreateFence(m_maxFramesInFlight, true);
        m_inFlightImages          = std::vector(m_swapChainStages, NULL_FENCE);
        // m_currentFrame = 0;

        auto renderPass = context_->CreateSimpleRenderPass({{surfaceFormat.format, vk::ImageLayout::ePresentSrcKHR}});
        surface_Format  = std::make_shared<FrameBufferFormat>(
            context_,
            std::move(renderPass),
            std::vector {Voxium::Platform::Render::FrameBufferAttachmentDescriptor {Voxium::Platform::Render::FrameBufferAttachmentType::COLOR,
                                                                  Voxium::Platform::Render::TextureFormat::B8G8R8A8_SRGB}});

        auto imageViews   = context_->CreateSwapChainViews(*m_swapChain, surfaceFormat.format);
        auto FrameBuffers = context_->CreateFrameBuffers(surface_Format->GetPass(), surfaceExtent, imageViews);
        m_FrameBuffer     = std::make_shared<FrameBuffer>(context_,
                                                      surface_Format,
                                                      surfaceExtent.width,
                                                      surfaceExtent.height,
                                                      std::move(imageViews),
                                                      std::move(FrameBuffers));

        // for (auto i = 0u; i < m_swapChainStages; ++i)
        // {
        // 	const auto& cmd = m_commandBuffer[i];
        // 	cmd.begin(vk::CommandBufferBeginInfo{});
        //
        // 	const std::vector<vk::ClearValue> clearValues = {
        // 		vk::ClearColorValue{ std::array<float, 4>{ 1.0f, 0.0f, 0.0f, 1.0f } },
        // 	};
        // 	const vk::RenderPassBeginInfo renderPassBeginInfo{
        // 		m_FrameBuffer->GetFormat().GetPass(),
        // 		(*m_FrameBuffer)[i],
        // 		{ { 0, 0 }, surfaceExtent },
        // 		static_cast<uint32_t>(clearValues.size()), clearValues.data()
        // 	};
        // 	cmd.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eSecondaryCommandBuffers);
        // 	cmd.endRenderPass();
        //
        // 	cmd.end();
        // }
    }

    void RenderContext::UpdateFirst()
    {
        context_->Wait(m_inFlightFence[m_currentFrame]);

        const auto acquireResult = context_->AcquireNextImage(*m_swapChain, image_AvailableSemaphore[m_currentFrame]);
        if (acquireResult.result == vk::Result::eErrorOutOfDateKHR || surface_.WasJustResized())
        {
            context_->WaitIdle();
            CreateSwapChain();
            return;
        }
        image_Index = acquireResult.value;

        auto& queue = m_renderQueues[image_Index];
        queue.clear();
    }

    void RenderContext::UpdateLast()
    {
        visitTicking();
        surface_.ResetResized();

        if (m_inFlightImages[image_Index] != NULL_FENCE)
            context_->Wait(m_inFlightImages[image_Index]);
        const auto inFlight = m_inFlightImages[image_Index] = m_inFlightFence[m_currentFrame];
        context_->reset(inFlight);

        auto& cb    = m_commandBuffer[image_Index];
        auto& queue = m_renderQueues[image_Index];
        queue.write(cb);

        context_->Submit(
            cb, image_AvailableSemaphore[m_currentFrame], m_renderFinishedSemaphore[m_currentFrame], inFlight);
        const auto presentResult =
            context_->present(m_renderFinishedSemaphore[m_currentFrame], *m_swapChain, image_Index);

        if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR ||
            surface_.WasJustResized())
        {
            context_->WaitIdle();
            CreateSwapChain();
        }

        m_currentFrame = (m_currentFrame + 1) % m_maxFramesInFlight;
    }

    std::shared_ptr<Voxium::Platform::Render::FrameBufferFormat>
    RenderContext::CreateFrameBufferFormat(const std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor>&  attachments,
                                           const std::vector<Voxium::Platform::Render::FrameBufferRenderStageDescriptor>& renderStages)
    {
        return std::make_shared<FrameBufferFormat>(context_, attachments, renderStages);
    }

    std::shared_ptr<Voxium::Platform::Render::FrameBuffer>
    RenderContext::CreateFrameBuffer(const std::shared_ptr<Voxium::Platform::Render::FrameBufferFormat>& format,
                                     const uint32_t                                    width,
                                     const uint32_t                                    height)
    {
        return std::make_shared<FrameBuffer>(
            context_, std::static_pointer_cast<FrameBufferFormat>(format), width, height, m_swapChainStages);
    }

    std::shared_ptr<Voxium::Platform::Render::Shader> RenderContext::CreateShader(const Voxium::Platform::Render::ShaderType                  type,
                                                                const std::vector<uint8_t>&               data,
                                                                const std::vector<Voxium::Platform::Render::ShaderBinding>& bindings)
    {
        return std::make_shared<Shader>(context_, type, data, bindings);
    }

    std::shared_ptr<Voxium::Platform::Render::RenderPipeline>
    RenderContext::CreatePipeline(const std::shared_ptr<Voxium::Platform::Render::FrameBufferFormat>&   format,
                                  const uint32_t                                      stage,
                                  const std::vector<std::shared_ptr<Voxium::Platform::Render::Shader>>& shaders,
                                  const Voxium::Platform::Render::VertexFormatDescriptor&               vertexFormat,
                                  const Voxium::Platform::Render::VertexFormatDescriptor&               instanceFormat,
                                  const Voxium::Platform::Render::RenderState                           state,
                                  const std::vector<Voxium::Platform::Render::BlendOptions>&            blendOptions)
    {
        std::vector<std::shared_ptr<IShader>> vkShaders;
        vkShaders.reserve(shaders.size());
        for (const auto& shader : shaders)
            vkShaders.push_back(std::static_pointer_cast<Shader>(shader));

        return std::make_shared<RenderPipeline>(context_,
                                                std::static_pointer_cast<FrameBufferFormat>(format),
                                                stage,
                                                vkShaders,
                                                vertexFormat,
                                                instanceFormat,
                                                state,
                                                blendOptions);
    }

    std::shared_ptr<Voxium::Platform::Render::UniformBinding>
    RenderContext::CreateUniformBinding(const std::shared_ptr<Voxium::Platform::Render::Shader>&        shader,
                                        const uint32_t                                binding,
                                        const std::shared_ptr<Voxium::Platform::Render::UniformBuffer>& uniformBuffer)
    {
        auto b = std::make_shared<UniformBinding>(
            context_, std::static_pointer_cast<Shader>(shader), binding, m_swapChainStages, uniformBuffer);
        if (uniformBuffer != nullptr)
            std::static_pointer_cast<UniformBuffer>(uniformBuffer)->RegisterUser(b);
        addTicking(b);
        return std::move(b);
    }

    std::shared_ptr<Voxium::Platform::Render::UniformBuffer> RenderContext::CreateUniformBuffer(const std::vector<uint8_t>& initialData)
    {
        auto ub = std::make_shared<UniformBuffer>(context_, m_swapChainStages, initialData);
        addTicking(ub);
        return std::move(ub);
    }

    std::shared_ptr<Voxium::Platform::Render::IVertexBuffer> RenderContext::CreateVertexBuffer(const std::vector<uint8_t>& initialData,
                                                                            const uint32_t              vertexSize,
                                                                            const bool                  writable)
    {
        if (writable)
        {
            auto vb = std::make_shared<DynamicVertexBuffer>(context_, initialData, vertexSize, m_swapChainStages);
            addTicking(vb);
            return std::move(vb);
        }

        return std::make_shared<StaticVertexBuffer>(context_, initialData, vertexSize);
    }

    std::shared_ptr<Voxium::Platform::Render::TextureBinding>
    RenderContext::CreateTextureBinding(const std::shared_ptr<Voxium::Platform::Render::Shader>&         shader,
                                        const uint32_t                                 binding,
                                        const std::shared_ptr<Voxium::Platform::Render::TextureSampler>& sampler,
                                        const std::shared_ptr<Voxium::Platform::Render::Texture>&        texture)
    {
        auto b = std::make_shared<TextureBinding>(
            context_, std::static_pointer_cast<Shader>(shader), binding, m_swapChainStages, sampler, texture);
        addTicking(b);
        return std::move(b);
    }

    std::shared_ptr<Voxium::Platform::Render::TextureSampler>
    RenderContext::CreateTextureSampler(const Voxium::Platform::Render::TextureFiltering   minFiltering,
                                        const Voxium::Platform::Render::TextureFiltering   magFiltering,
                                        const Voxium::Platform::Render::TextureWrapping    wrapping,
                                        const Voxium::Platform::Render::TextureBorderColor borderColor,
                                        const bool                       enableAnisotropy,
                                        const uint32_t                   anisotropyLevel)
    {
        return std::make_shared<TextureSampler>(
            context_, minFiltering, magFiltering, wrapping, borderColor, enableAnisotropy, anisotropyLevel);
    }

    std::shared_ptr<Voxium::Platform::Render::Texture>
    RenderContext::CreateTexture(const uint32_t width, const uint32_t height, const std::vector<uint8_t>& data)
    {
        return std::make_shared<StaticTexture>(context_, width, height, Voxium::Platform::Render::TextureFormat::B8G8R8A8_SRGB, data);
    }

    std::shared_ptr<Voxium::Platform::Render::CommandBuffer> RenderContext::CreateCommandBuffer()
    {
        auto cmd = std::make_shared<CommandBuffer>(context_, m_swapChainStages);
        addTicking(cmd);
        return std::move(cmd);
    }

    void RenderContext::Enqueue(const std::shared_ptr<Voxium::Platform::Render::IRenderTarget>& renderTarget,
                                const std::shared_ptr<Voxium::Platform::Render::CommandBuffer>& commandBuffer)
    {
        m_renderQueues[image_Index].Enqueue(renderTarget, std::static_pointer_cast<CommandBuffer>(commandBuffer));
    }

    void RenderContext::addTicking(std::weak_ptr<DynamicVertexBuffer> resource)
    {
        if (m_availableTickingVertexBufferSlots.empty())
            return m_tickingVertexBuffers.push_back(std::move(resource));

        const auto slot = m_availableTickingVertexBufferSlots.front();
        m_availableTickingVertexBufferSlots.pop();
        m_tickingVertexBuffers[slot] = std::move(resource);
    }

    void RenderContext::addTicking(std::weak_ptr<UniformBuffer> resource)
    {
        if (m_availableTickingUniformBufferSlots.empty())
            return m_tickingUniformBuffers.push_back(std::move(resource));

        const auto slot = m_availableTickingUniformBufferSlots.front();
        m_availableTickingUniformBufferSlots.pop();
        m_tickingUniformBuffers[slot] = std::move(resource);
    }

    void RenderContext::addTicking(std::weak_ptr<UniformBinding> resource)
    {
        if (m_availableTickingUniformBindingSlots.empty())
            return m_tickingUniformBindings.push_back(std::move(resource));

        const auto slot = m_availableTickingUniformBindingSlots.front();
        m_availableTickingUniformBindingSlots.pop();
        m_tickingUniformBindings[slot] = std::move(resource);
    }

    void RenderContext::addTicking(std::weak_ptr<TextureBinding> resource)
    {
        if (m_availableTickingTextureBindingSlots.empty())
            return m_tickingTextureBindings.push_back(std::move(resource));

        const auto slot = m_availableTickingTextureBindingSlots.front();
        m_availableTickingTextureBindingSlots.pop();
        m_tickingTextureBindings[slot] = std::move(resource);
    }

    void RenderContext::addTicking(std::weak_ptr<CommandBuffer> resource)
    {
        if (m_availableTickingCommandBufferSlots.empty())
            return m_tickingCommandBuffers.push_back(std::move(resource));

        const auto slot = m_availableTickingCommandBufferSlots.front();
        m_availableTickingCommandBufferSlots.pop();
        m_tickingCommandBuffers[slot] = std::move(resource);
    }

    void RenderContext::visitTicking()
    {
        uint32_t i = 0;
        for (auto& res : m_tickingVertexBuffers)
        {
            if (res.expired())
            {
                m_availableTickingVertexBufferSlots.emplace(i);
                i++;
                continue;
            }

            res.lock()->tick();
            i++;
        }

        i = 0;
        for (auto& res : m_tickingUniformBuffers)
        {
            if (res.expired())
            {
                m_availableTickingUniformBufferSlots.emplace(i);
                i++;
                continue;
            }

            res.lock()->tick();
            i++;
        }

        i = 0;
        for (auto& res : m_tickingUniformBindings)
        {
            if (res.expired())
            {
                m_availableTickingUniformBindingSlots.emplace(i);
                i++;
                continue;
            }

            res.lock()->tick();
            i++;
        }

        i = 0;
        for (auto& res : m_tickingTextureBindings)
        {
            if (res.expired())
            {
                m_availableTickingTextureBindingSlots.emplace(i);
                i++;
                continue;
            }

            res.lock()->tick();
            i++;
        }

        i = 0;
        for (auto& res : m_tickingCommandBuffers)
        {
            if (res.expired())
            {
                m_availableTickingCommandBufferSlots.emplace(i);
                i++;
                continue;
            }

            res.lock()->tick();
            i++;
        }
    }
} // namespace Voxium::Platform::Desktop::Vulkan
