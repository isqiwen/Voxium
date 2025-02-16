#pragma once

#include "Platform/Render/ICommandBuffer.h"

#include "VkFrameBufferFormat.h"
#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class CBCmd
    {
    public:
        CBCmd()                                  = default;
        virtual ~CBCmd()                         = default;
        CBCmd(const CBCmd& other)                = delete;
        CBCmd(CBCmd&& other) noexcept            = delete;
        CBCmd& operator=(const CBCmd& other)     = delete;
        CBCmd& operator=(CBCmd&& other) noexcept = delete;

        virtual void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) = 0;
    };

    class CBCmdBegin final : public CBCmd
    {
    public:
        explicit CBCmdBegin(std::shared_ptr<IFrameBufferFormat> format) : m_format(std::move(format)) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        std::shared_ptr<IFrameBufferFormat> m_format;
    };
    class CBCmdEnd final : public CBCmd
    {
    public:
        explicit CBCmdEnd() {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;
    };
    class CBCmdSetViewportScissor final : public CBCmd
    {
    public:
        explicit CBCmdSetViewportScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget) :
            m_renderTarget(std::move(renderTarget))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::IRenderTarget> m_renderTarget;
    };
    class CBCmdSetViewport final : public CBCmd
    {
    public:
        explicit CBCmdSetViewport(const platform::util::Extents2D extents) : m_extents(extents) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        platform::util::Extents2D m_extents;
    };
    class CBCmdSetScissor final : public CBCmd
    {
    public:
        explicit CBCmdSetScissor(const platform::util::Extents2D extents) : m_extents(extents) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        platform::util::Extents2D m_extents;
    };
    class CBCmdBindUniform final : public CBCmd
    {
    public:
        explicit CBCmdBindUniform(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                                  std::shared_ptr<Voxium::Platform::Render::UniformBinding> uniformBinding,
                                  const uint32_t                          binding) :
            m_pipeline(std::move(pipeline)), m_uniformBinding(std::move(uniformBinding)), binding_(binding)
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::RenderPipeline> m_pipeline;
        std::shared_ptr<Voxium::Platform::Render::UniformBinding> m_uniformBinding;
        uint32_t                                binding_;
    };
    class CBCmdBindTexture final : public CBCmd
    {
    public:
        explicit CBCmdBindTexture(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                                  std::shared_ptr<Voxium::Platform::Render::TextureBinding> binding) :
            m_pipeline(std::move(pipeline)), binding_(std::move(binding))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::RenderPipeline> m_pipeline;
        std::shared_ptr<Voxium::Platform::Render::TextureBinding> binding_;
    };
    class CBCmdDraw final : public CBCmd
    {
    public:
        explicit CBCmdDraw(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                           std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                           std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer) :
            m_pipeline(std::move(pipeline)), m_vertexBuffer(std::move(vertexBuffer)),
            m_instanceBuffer(std::move(instanceBuffer))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::Resource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::RenderPipeline> m_pipeline;
        std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   m_vertexBuffer;
        std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   m_instanceBuffer;
    };

    class CommandBuffer final : public Voxium::Platform::Render::CommandBuffer, public IScalableStagingResource
    {
    public:
        CommandBuffer(std::shared_ptr<VulkanContext> context, uint32_t stages);

        void tick();

        void reserve(uint32_t stages) override;

        void BeginRecording(const std::shared_ptr<Voxium::Platform::Render::FrameBufferFormat>& format) override;
        void SetViewportAndScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget) override;
        void SetViewport(platform::util::Extents2D extents) override;
        void SetScissor(platform::util::Extents2D extents) override;
        void BindUniform(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                         std::shared_ptr<Voxium::Platform::Render::UniformBinding> uniformBinding,
                         uint32_t                                binding) override;
        void BindTexture(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                         std::shared_ptr<Voxium::Platform::Render::TextureBinding> binding) override;
        void Draw(std::shared_ptr<Voxium::Platform::Render::RenderPipeline> pipeline,
                  std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                  std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer) override;
        void FinishRecording() override;

        [[nodiscard]] vk::CommandBuffer& get();

    private:
        std::shared_ptr<VulkanContext> context_;

        std::vector<vk::UniqueCommandBuffer>                m_commandBuffers;
        std::vector<std::vector<std::shared_ptr<Resource>>> m_resources;
        std::vector<std::unique_ptr<CBCmd>>                 m_commandQueue;
        uint32_t                                            readIndex_      = 0;
        uint32_t                                            leftoverWrites_ = 0;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
