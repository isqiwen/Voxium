#pragma once

#include "Platform/Render/IVertexBuffer.h"

#include "VkBuffer.h"
#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class VertexBuffer : public Voxium::Platform::Render::IVertexBuffer
    {
    public:
        [[nodiscard]] virtual vk::Buffer& Get()  = 0;
        [[nodiscard]] virtual uint32_t    Size() = 0;
    };

    class StaticVertexBuffer final : public VertexBuffer
    {
    public:
        StaticVertexBuffer(std::shared_ptr<VulkanContext> context,
                           const std::vector<uint8_t>&    data,
                           uint32_t                       vertexSize);

        [[nodiscard]] uint32_t GetVertexSize() override { return vertexSize_; }

        void Write(const std::vector<uint8_t>& data) override
        {
            throw std::runtime_error("Cannot write to a static vertex buffer.");
        }

        [[nodiscard]] vk::Buffer& Get() override { return buffer_->Buffer(); }

        [[nodiscard]] uint32_t Size() override { return size_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        std::unique_ptr<VulkanBuffer>  buffer_;
        uint32_t                       vertexSize_, size_;
    };

    class DynamicVertexBuffer final : public VertexBuffer
    {
    public:
        DynamicVertexBuffer(std::shared_ptr<VulkanContext> context,
                            const std::vector<uint8_t>&    data,
                            uint32_t                       vertexSize,
                            uint32_t                       stages);

        void Tick();

        [[nodiscard]] uint32_t GetVertexSize() override { return vertexSize_; }

        void Write(const std::vector<uint8_t>& data) override;

        [[nodiscard]] vk::Buffer& Get() override;

        [[nodiscard]] uint32_t Size() override;

    private:
        void     AdvanceIfNeeded();
        uint32_t GetWriteIndex() const;

        std::shared_ptr<VulkanContext>             context_;
        std::vector<std::unique_ptr<VulkanBuffer>> buffers_;
        std::vector<uint32_t>                      sizes_;
        uint32_t                                   vertexSize_;
        uint32_t                                   readIndex_    = 0;
        bool                                       advanceIndex_ = false;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
