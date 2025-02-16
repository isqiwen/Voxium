#include "VkVertexBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    StaticVertexBuffer::StaticVertexBuffer(std::shared_ptr<VulkanContext> context,
                                           const std::vector<uint8_t>&    data,
                                           const uint32_t                 vertexSize) :
        context_(std::move(context)), vertexSize_(vertexSize)
    {
        buffer_ =
            context_->CreateBuffer(static_cast<uint32_t>(data.size()),
                                    vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                    vk::SharingMode::eExclusive,
                                    {});

        auto buf = context_->CreateCpuToGpuTransferBuffer(data.data(), static_cast<uint32_t>(data.size()));

        util::CopyBufferToBufferImmediate(*context_->m_device,
                                          *context_->m_commandPool,
                                          context_->m_graphicsQueue,
                                          buf->Buffer(),
                                          buffer_->Buffer(),
                                          static_cast<uint32_t>(data.size()));

        size_ = static_cast<uint32_t>(data.size() / vertexSize);
    }

    DynamicVertexBuffer::DynamicVertexBuffer(std::shared_ptr<VulkanContext> context,
                                             const std::vector<uint8_t>&    data,
                                             const uint32_t                 vertexSize,
                                             const uint32_t                 stages) :
        context_(std::move(context)), vertexSize_(vertexSize)
    {
        buffers_.resize(stages);
        sizes_.resize(stages);

        if (!data.empty())
            Write(data);
    }

    void DynamicVertexBuffer::Tick() { AdvanceIfNeeded(); }

    void DynamicVertexBuffer::Write(const std::vector<uint8_t>& data)
    {
        const auto writeIndex = GetWriteIndex();
        auto&      buffer     = buffers_[writeIndex];

        if (!buffer || buffer->size() < data.size())
        {
            buffer =
                context_->CreateBuffer(static_cast<uint32_t>(data.size()),
                                        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                        vk::SharingMode::eExclusive,
                                        {});
        }

        auto buf = context_->CreateCpuToGpuTransferBuffer(data.data(), static_cast<uint32_t>(data.size()));

        util::CopyBufferToBufferImmediate(*context_->m_device,
                                          *context_->m_commandPool,
                                          context_->m_graphicsQueue,
                                          buf->Buffer(),
                                          buffer->Buffer(),
                                          static_cast<uint32_t>(data.size()));

        sizes_[writeIndex] = static_cast<uint32_t>(data.size() / vertexSize_);

        advanceIndex_ = true;
    }

    vk::Buffer& DynamicVertexBuffer::get()
    {
        AdvanceIfNeeded();
        return buffers_[readIndex_]->Buffer();
    }

    uint32_t DynamicVertexBuffer::size()
    {
        AdvanceIfNeeded();
        return sizes_[readIndex_];
    }

    void DynamicVertexBuffer::AdvanceIfNeeded()
    {
        if (advanceIndex_)
        {
            advanceIndex_ = false;
            readIndex_    = GetWriteIndex();
        }
    }

    uint32_t DynamicVertexBuffer::GetWriteIndex() const { return (readIndex_ + 1) % buffers_.size(); }
} // namespace Voxium::Platform::Desktop::Vulkan
