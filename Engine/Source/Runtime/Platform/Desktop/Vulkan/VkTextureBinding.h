#pragma once

#include "Platform/Render/ITextureBinding.h"

#include "VkContext.h"
#include "VkShader.h"
#include "VkTexture.h"
#include "VkTextureSampler.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class TextureBinding final : public Voxium::Platform::Render::TextureBinding
    {
    public:
        TextureBinding(std::shared_ptr<VulkanContext>                 context,
                       std::shared_ptr<IShader>                        shader,
                       uint32_t                                       binding,
                       uint32_t                                       stages,
                       const std::shared_ptr<Voxium::Platform::Render::TextureSampler>& sampler,
                       const std::shared_ptr<Voxium::Platform::Render::Texture>&        texture);

        void tick();

        void Update(std::shared_ptr<Voxium::Platform::Render::TextureSampler> sampler, std::shared_ptr<Voxium::Platform::Render::Texture> texture) override;

        [[nodiscard]] vk::DescriptorSet& get() { return *descriptorSets_[readIndex_]; }

        [[nodiscard]] std::shared_ptr<IShader>& getShader() { return shader_; }

        [[nodiscard]] uint32_t getBinding() const { return binding_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        std::shared_ptr<IShader>        shader_;
        uint32_t                       binding_;

        std::vector<std::shared_ptr<ITextureSampler>> m_samplers;
        std::vector<std::shared_ptr<ITexture>>        textures_;

        vk::UniqueDescriptorPool             descriptorPool_;
        std::vector<vk::UniqueDescriptorSet> descriptorSets_;

        uint32_t readIndex_      = 0;
        uint32_t leftoverWrites_ = 0;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
