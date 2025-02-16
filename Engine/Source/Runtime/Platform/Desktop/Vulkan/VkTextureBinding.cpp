#include "VkTextureBinding.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    TextureBinding::TextureBinding(std::shared_ptr<VulkanContext>                 context,
                                   std::shared_ptr<IShader>                        shader,
                                   const uint32_t                                 binding,
                                   const uint32_t                                 stages,
                                   const std::shared_ptr<Voxium::Platform::Render::TextureSampler>& sampler,
                                   const std::shared_ptr<Voxium::Platform::Render::Texture>&        texture) :
        context_(std::move(context)), shader_(std::move(shader)), binding_(binding)
    {
        m_samplers.resize(stages);
        m_textures.resize(stages);

        descriptorPool_ = context_->CreateDescriptorPool(stages, vk::DescriptorType::eCombinedImageSampler);
        descriptorSets_ =
            context_->CreateDescriptorSets(*descriptorPool_, shader_->GetDescriptorSetLayouts()[binding], stages);

        if (texture != nullptr)
            Update(sampler, texture);
    }

    void TextureBinding::tick()
    {
        const auto writeIndex = (readIndex_ + 1) % static_cast<uint32_t>(descriptorSets_.size());

        if (leftoverWrites_ == 0)
        {
            readIndex_ = writeIndex;
            return;
        }

        const vk::DescriptorImageInfo imageInfo {
            m_samplers[writeIndex]->get(), m_textures[writeIndex]->get(), vk::ImageLayout::eShaderReadOnlyOptimal};
        const vk::WriteDescriptorSet write {*descriptorSets_[writeIndex],
                                            binding_,
                                            0,
                                            1,
                                            vk::DescriptorType::eCombinedImageSampler,
                                            &imageInfo,
                                            nullptr,
                                            nullptr};

        context_->UpdateDescriptorSets({write}, {});

        const auto nextWriteIndex  = (writeIndex + 1) % static_cast<uint32_t>(descriptorSets_.size());
        m_samplers[nextWriteIndex] = m_samplers[writeIndex];
        m_textures[nextWriteIndex] = m_textures[writeIndex];

        leftoverWrites_--;
        readIndex_ = writeIndex;
    }

    void TextureBinding::Update(const std::shared_ptr<Voxium::Platform::Render::TextureSampler> sampler,
                                const std::shared_ptr<Voxium::Platform::Render::Texture>        texture)
    {
        const auto writeIndex  = (readIndex_ + 1) % static_cast<uint32_t>(descriptorSets_.size());
        m_samplers[writeIndex] = std::static_pointer_cast<TextureSampler>(sampler);
        m_textures[writeIndex] = std::static_pointer_cast<Texture>(texture);

        leftoverWrites_ = static_cast<uint32_t>(descriptorSets_.size());
    }
} // namespace Voxium::Platform::Desktop::Vulkan
