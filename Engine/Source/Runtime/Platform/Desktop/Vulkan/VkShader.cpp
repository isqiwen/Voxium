#include "VkShader.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    vk::ShaderStageFlagBits ToVulkan(const Voxium::Platform::Render::ShaderType type)
    {
        switch (type)
        {
            case Voxium::Platform::Render::ShaderType::VERTEX:
                return vk::ShaderStageFlagBits::eVertex;
            case Voxium::Platform::Render::ShaderType::FRAGMENT:
                return vk::ShaderStageFlagBits::eFragment;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::DescriptorType ToVulkan(const Voxium::Platform::Render::ShaderBindingType type)
    {
        switch (type)
        {
            case Voxium::Platform::Render::ShaderBindingType::UNIFORM:
                return vk::DescriptorType::eUniformBufferDynamic;
            case Voxium::Platform::Render::ShaderBindingType::SAMPLER:
                return vk::DescriptorType::eCombinedImageSampler;
        }
        throw std::runtime_error("Invalid type.");
    }

    Shader::Shader(std::shared_ptr<VulkanContext>            context,
        Voxium::Platform::Render::ShaderType                 type,
        const std::vector<uint8_t>&                          data,
        std::vector<Voxium::Platform::Render::ShaderBinding> bindings) :
        context_(std::move(context)), module_(context_->CreateShaderModule(data)), bindings_(std::move(bindings)),
        stage_(ToVulkan(type))
    {
        layoutDesc_.reserve(bindings_.size());
        layoutDesc2_.reserve(bindings_.size());
        for (const auto& binding : bindings_)
        {
            auto layout = context_->CreateDescriptorSetLayout(
                {static_cast<uint32_t>(layoutDesc_.size()), ToVulkan(binding.type), 1, stage_});
            layoutDesc2_.push_back(*layout);
            layoutDesc_.push_back(std::move(layout));
        }
    }
} // namespace Voxium::Platform::Desktop::Vulkan
