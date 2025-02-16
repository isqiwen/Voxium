#include "VkUniformBinding.h"
#include "VkUniformBuffer.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    UniformBinding::UniformBinding(std::shared_ptr<VulkanContext>                context,
                                   std::shared_ptr<IShader>                       shader,
                                   const uint32_t                                binding,
                                   const uint32_t                                stages,
                                   const std::shared_ptr<Voxium::Platform::Render::UniformBuffer>& uniformBuffer) :
        context_(std::move(context)), shader_(std::move(shader)), binding_(binding),
        bindingSize_(shader_->GetBindings()()[binding].size)
    {
        buffers_.resize(stages);

        descriptorPool_ = context_->CreateDescriptorPool(stages, vk::DescriptorType::eUniformBuffer);
        descriptorSets_ =
            context_->CreateDescriptorSets(*descriptorPool_, shader_->GetDescriptorSetLayouts()[binding], stages);

        if (uniformBuffer != nullptr)
            Update(uniformBuffer, false);
    }

    UniformBinding::~UniformBinding()
    {
        const auto& currentUniformBuffer = buffers_[readIndex_];
        if (currentUniformBuffer)
            currentUniformBuffer->UnRegisterUser(this);
    }

    void UniformBinding::Tick()
    {
        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(descriptorSets_.size());

        if (leftoverWrites_ == 0)
        {
            readIndex_ = writeIndex;
            return;
        }

        const vk::DescriptorBufferInfo bufferInfo {buffers_[writeIndex]->Buffer(), 0, bindingSize_};
        const vk::WriteDescriptorSet   write {*descriptorSets_[writeIndex],
                                            binding_,
                                            0,
                                            1,
                                            vk::DescriptorType::eUniformBufferDynamic,
                                            nullptr,
                                            &bufferInfo,
                                            nullptr};

        context_->UpdateDescriptorSets({write}, {});

        const auto nextWriteIndex = (writeIndex + 1) % static_cast<uint32_t>(descriptorSets_.size());
        buffers_[nextWriteIndex] = buffers_[writeIndex];

        leftoverWrites_--;
        readIndex_ = writeIndex;
    }

    void UniformBinding::UpdateNext() { leftoverWrites_ = std::max(1u, leftoverWrites_); }

    void UniformBinding::Update(const std::shared_ptr<Voxium::Platform::Render::UniformBuffer> uniformBuffer, const bool RegisterUser)
    {
        const auto& currentUniformBuffer = buffers_[readIndex_];
        if (currentUniformBuffer)
            currentUniformBuffer->UnRegisterUser(this);

        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(descriptorSets_.size());
        const auto ub         = std::static_pointer_cast<UniformBuffer>(uniformBuffer);
        buffers_[writeIndex] = ub;
        if (RegisterUser)
            ub->RegisterUser(std::static_pointer_cast<UniformBinding>(this->shared_from_this()));

        leftoverWrites_ = static_cast<uint32_t>(descriptorSets_.size());
    }
} // namespace Voxium::Platform::Desktop::Vulkan
