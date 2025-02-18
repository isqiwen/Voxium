#pragma once

#include <queue>

#include "Platform/Render/IRenderSurface.h"

#include "VkRenderContext.h"
#include "VkFrameBuffer.h"
#include "VkFrameBufferFormat.h"
#include "VkCommandBuffer.h"
#include "VkContext.h"
#include "VkTextureBinding.h"
#include "VkUniformBinding.h"
#include "VkUniformBuffer.h"
#include "VkVertexBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class RenderQueue final
    {
    public:
        void clear();
        void Enqueue(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> target, std::shared_ptr<ICommandBuffer> commandBuffer);

        void write(vk::CommandBuffer& cmd);

    private:
        std::vector<std::tuple<std::shared_ptr<Voxium::Platform::Render::IRenderTarget>, std::shared_ptr<ICommandBuffer>>> m_queue;
    };

    class RenderContext final : public desktop::RenderContext
    {
    public:
        explicit RenderContext(RenderSurface&                   surface,
                               std::shared_ptr<VulkanContext>&& context,
                               vk::UniqueSurfaceKHR&&           vkSurface);
        ~RenderContext() override;
        RenderContext(const RenderContext& other)                = delete;
        RenderContext(RenderContext&& other) noexcept            = delete;
        RenderContext& operator=(const RenderContext& other)     = delete;
        RenderContext& operator=(RenderContext&& other) noexcept = delete;

        void CreateSwapChain();

        void UpdateFirst() override;
        void UpdateLast() override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat> GetSurfaceFormat() override { return surface_Format; }

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat>
        CreateFrameBufferFormat(const std::vector<Voxium::Platform::Render::FrameBufferAttachmentDescriptor>&  attachments,
                                const std::vector<Voxium::Platform::Render::FrameBufferRenderStageDescriptor>& renderStages) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::FrameBuffer>
        CreateFrameBuffer(const std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat>& format,
                          uint32_t                                          width,
                          uint32_t                                          height) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::Shader>
        CreateShader(Voxium::Platform::Render::ShaderType                        type,
                     const std::vector<uint8_t>&               data,
                     const std::vector<Voxium::Platform::Render::ShaderBinding>& bindings) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IRenderPipeline>
        CreatePipeline(const std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat>&   format,
                       uint32_t                                            stage,
                       const std::vector<std::shared_ptr<Voxium::Platform::Render::Shader>>& shaders,
                       const Voxium::Platform::Render::VertexFormatDescriptor&               vertexFormat,
                       const Voxium::Platform::Render::VertexFormatDescriptor&               instanceFormat,
                       Voxium::Platform::Render::RenderState                                 state,
                       const std::vector<Voxium::Platform::Render::BlendOptions>&            blendOptions) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IUniformBinding>
        CreateUniformBinding(const std::shared_ptr<Voxium::Platform::Render::Shader>&        shader,
                             uint32_t                                      binding,
                             const std::shared_ptr<Voxium::Platform::Render::UniformBuffer>& uniformBuffer) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::UniformBuffer>
        CreateUniformBuffer(const std::vector<uint8_t>& initialData) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>
        CreateVertexBuffer(const std::vector<uint8_t>& initialData, uint32_t vertexSize, bool writable) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::ITextureBinding>
        CreateTextureBinding(const std::shared_ptr<Voxium::Platform::Render::Shader>&         shader,
                             uint32_t                                       binding,
                             const std::shared_ptr<Voxium::Platform::Render::TextureSampler>& sampler,
                             const std::shared_ptr<Voxium::Platform::Render::Texture>&        texture) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::TextureSampler>
        CreateTextureSampler(Voxium::Platform::Render::TextureFiltering   minFiltering,
                             Voxium::Platform::Render::TextureFiltering   magFiltering,
                             Voxium::Platform::Render::TextureWrapping    wrapping,
                             Voxium::Platform::Render::TextureBorderColor borderColor,
                             bool                       enableAnisotropy,
                             uint32_t                   anisotropyLevel) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::Texture>
        CreateTexture(uint32_t width, uint32_t height, const std::vector<uint8_t>& data) override;

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::CommandBuffer> CreateCommandBuffer() override;

        void Enqueue(const std::shared_ptr<Voxium::Platform::Render::IRenderTarget>& renderTarget,
                     const std::shared_ptr<Voxium::Platform::Render::CommandBuffer>& commandBuffer) override;

        [[nodiscard]] Voxium::Platform::Render::FrameBuffer& GetFrameBuffer() override { return *m_FrameBuffer; }

    private:
        void addTicking(std::weak_ptr<DynamicVertexBuffer> resource);
        void addTicking(std::weak_ptr<UniformBuffer> resource);
        void addTicking(std::weak_ptr<UniformBinding> resource);
        void addTicking(std::weak_ptr<TextureBinding> resource);
        void addTicking(std::weak_ptr<CommandBuffer> resource);
        void visitTicking();

        RenderSurface&                       surface_;
        const std::shared_ptr<VulkanContext> context_;
        const vk::UniqueSurfaceKHR           m_vkSurface;

        vk::UniqueSwapchainKHR                   m_swapChain;
        uint32_t                                 m_swapChainStages;
        std::shared_ptr<IFrameBufferFormat>       surface_Format;
        std::shared_ptr<IFrameBuffer>             m_FrameBuffer;
        util::StagingResource<vk::CommandBuffer> m_commandBuffer;
        std::vector<RenderQueue>                 m_renderQueues;
        bool                                     resized_ = false;

        uint32_t                             m_maxFramesInFlight;
        util::StagingResource<vk::Semaphore> image_AvailableSemaphore;
        util::StagingResource<vk::Semaphore> m_renderFinishedSemaphore;
        util::StagingResource<vk::Fence>     m_inFlightFence;
        std::vector<vk::Fence>               m_inFlightImages;
        uint32_t                             m_currentFrame = 0;
        uint32_t                             image_Index   = 0;

        std::vector<std::weak_ptr<DynamicVertexBuffer>> m_tickingVertexBuffers;
        std::queue<uint32_t>                            m_availableTickingVertexBufferSlots;
        std::vector<std::weak_ptr<UniformBuffer>>       m_tickingUniformBuffers;
        std::queue<uint32_t>                            m_availableTickingUniformBufferSlots;
        std::vector<std::weak_ptr<UniformBinding>>      m_tickingUniformBindings;
        std::queue<uint32_t>                            m_availableTickingUniformBindingSlots;
        std::vector<std::weak_ptr<TextureBinding>>      m_tickingTextureBindings;
        std::queue<uint32_t>                            m_availableTickingTextureBindingSlots;
        std::vector<std::weak_ptr<CommandBuffer>>       m_tickingCommandBuffers;
        std::queue<uint32_t>                            m_availableTickingCommandBufferSlots;

        friend class RenderManager;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
