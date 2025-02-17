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

        virtual void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) = 0;
    };

    class CBCmdBegin final : public CBCmd
    {
    public:
        explicit CBCmdBegin(std::shared_ptr<IFrameBufferFormat> format) : format_(std::move(format)) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        std::shared_ptr<IFrameBufferFormat> format_;
    };

    class CBCmdEnd final : public CBCmd
    {
    public:
        explicit CBCmdEnd() {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;
    };

    class CBCmdSetViewportScissor final : public CBCmd
    {
    public:
        explicit CBCmdSetViewportScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget) :
            renderTarget_(std::move(renderTarget))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget_;
    };

    class CBCmdSetViewport final : public CBCmd
    {
    public:
        explicit CBCmdSetViewport(const Voxium::Platform::Utils::Extents2D extents) : extents_(extents) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        Voxium::Platform::Utils::Extents2D extents_;
    };

    class CBCmdSetScissor final : public CBCmd
    {
    public:
        explicit CBCmdSetScissor(const Voxium::Platform::Utils::Extents2D extents) : extents_(extents) {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        Voxium::Platform::Utils::Extents2D extents_;
    };

    class CBCmdBindUniform final : public CBCmd
    {
    public:
        explicit CBCmdBindUniform(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                                  std::shared_ptr<Voxium::Platform::Render::IUniformBinding> uniformBinding,
                                  const uint32_t                          binding) :
            pipeline_(std::move(pipeline)), uniformBinding_(std::move(uniformBinding)), binding_(binding)
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline_;
        std::shared_ptr<Voxium::Platform::Render::IUniformBinding> uniformBinding_;
        uint32_t                                                   binding_;
    };

    class CBCmdBindTexture final : public CBCmd
    {
    public:
        explicit CBCmdBindTexture(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                                  std::shared_ptr<Voxium::Platform::Render::ITextureBinding> binding) :
            pipeline_(std::move(pipeline)), binding_(std::move(binding))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline_;
        std::shared_ptr<Voxium::Platform::Render::ITextureBinding> binding_;
    };

    class CBCmdDraw final : public CBCmd
    {
    public:
        explicit CBCmdDraw(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                           std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                           std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer) :
            pipeline_(std::move(pipeline)), vertexBuffer_(std::move(vertexBuffer)),
            instanceBuffer_(std::move(instanceBuffer))
        {}

        void Record(vk::CommandBuffer& cmd, std::vector<std::shared_ptr<Voxium::Platform::Render::IResource>>& resources) override;

    private:
        std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline_;
        std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer_;
        std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer_;
    };

    class CommandBuffer final : public Voxium::Platform::Render::ICommandBuffer, public IScalableStagingResource
    {
    public:
        CommandBuffer(std::shared_ptr<VulkanContext> context, uint32_t stages);

        void Tick();

        void Reserve(uint32_t stages) override;

        void BeginRecording(const std::shared_ptr<Voxium::Platform::Render::IFrameBufferFormat>& format) override;
        void SetViewportAndScissor(std::shared_ptr<Voxium::Platform::Render::IRenderTarget> renderTarget) override;
        void SetViewport(Voxium::Platform::Utils::Extents2D extents) override;
        void SetScissor(Voxium::Platform::Utils::Extents2D extents) override;
        void BindUniform(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                         std::shared_ptr<Voxium::Platform::Render::IUniformBinding> uniformBinding,
                         uint32_t                                binding) override;
        void BindTexture(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                         std::shared_ptr<Voxium::Platform::Render::ITextureBinding> binding) override;
        void Draw(std::shared_ptr<Voxium::Platform::Render::IRenderPipeline> pipeline,
                  std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   vertexBuffer,
                  std::shared_ptr<Voxium::Platform::Render::IVertexBuffer>   instanceBuffer) override;
        void FinishRecording() override;

        [[nodiscard]] vk::CommandBuffer& Get();

    private:
        std::shared_ptr<VulkanContext> context_;

        std::vector<vk::UniqueCommandBuffer>                 commandBuffers_;
        std::vector<std::vector<std::shared_ptr<IResource>>> resources_;
        std::vector<std::unique_ptr<CBCmd>>                  commandQueue_;
        uint32_t                                             readIndex_      = 0;
        uint32_t                                             leftoverWrites_ = 0;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
