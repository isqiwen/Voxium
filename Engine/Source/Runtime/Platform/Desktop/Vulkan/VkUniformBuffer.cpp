#include "VkUniformBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    UniformBuffer::UniformBuffer(std::shared_ptr<VulkanContext> context,
                                 const uint32_t                 stages,
                                 const std::vector<uint8_t>&    initialData) : context_(std::move(context))
    {
        buffers_.resize(stages);

        if (!initialData.empty())
            Write(initialData);
    }

    void UniformBuffer::Tick()
    {
        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(buffers_.size());

        if (leftoverWrites_ == 0)
        {
            readIndex_ = writeIndex;
            return;
        }

        if (uniformData_.empty())
        {
            leftoverWrites_--;
            readIndex_ = writeIndex;
            return;
        }

        auto& buffer = buffers_[writeIndex];

        if (!buffer || buffer->size() < uniformData_.size())
        {
            buffer =
                context_->CreateBuffer(static_cast<uint32_t>(uniformData_.size()),
                                        vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                        vk::SharingMode::eExclusive,
                                        {});

            for (const auto& dependent : dependents_)
            {
                const auto binding = dependent.lock();
                if (binding)
                    binding->UpdateNext();
            }
        }

        auto buf =
            context_->CreateCpuToGpuTransferBuffer(uniformData_.data(), static_cast<uint32_t>(uniformData_.size()));

        CopyBufferToBufferImmediate(*context_->m_device,
                                    *context_->commandPool_,
                                    context_->graphicsQueue_,
                                    buf->Buffer(),
                                    buffer->Buffer(),
                                    static_cast<uint32_t>(uniformData_.size()));

        leftoverWrites_--;
        readIndex_ = writeIndex;

        if (leftoverWrites_ == 0)
        {
            uniformData_.clear();
            uniformData_.shrink_to_fit();
        }
    }

    void UniformBuffer::Write(const std::vector<uint8_t>& data)
    {
        uniformData_.assign(data.begin(), data.end());
        leftoverWrites_ = static_cast<uint32_t>(buffers_.size());
    }

    void UniformBuffer::RegisterUser(const std::weak_ptr<UniformBinding>& binding) { dependents_.insert(binding); }

    void UniformBuffer::UnRegisterUser(const UniformBinding* binding)
    {
        const auto iterator =
            std::find_if(dependents_.begin(), dependents_.end(), [&](const std::weak_ptr<UniformBinding>& ptr) {
                return ptr.lock().get() == binding;
            });
        if (iterator == dependents_.end())
            return;
        dependents_.extract(iterator);
    }
} // namespace Voxium::Platform::Desktop::Vulkan
