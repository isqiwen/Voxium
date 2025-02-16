#pragma once

#include "Platform/Render/IRenderContext.h"
#include "Platform/Render/IShader.h"

#include "VkContext.h"


namespace Voxium::Platform::Desktop::Vulkan
{
    class Shader final : public Voxium::Platform::Render::IShader
    {
    public:
        Shader(std::shared_ptr<VulkanContext>                       context,
               Voxium::Platform::Render::ShaderType                 type,
               const std::vector<uint8_t>&                          data,
               std::vector<Voxium::Platform::Render::ShaderBinding> bindings);

        [[nodiscard]] vk::ShaderModule& GetModule() { return *module_; }

        [[nodiscard]] const std::vector<Voxium::Platform::Render::ShaderBinding>& GetBindings()() const { return bindings_; }

        [[nodiscard]] std::vector<vk::DescriptorSetLayout>& GetDescriptorSetLayouts() { return layoutDesc2_; }

        [[nodiscard]] vk::ShaderStageFlagBits GetStage() const { return stage_; }

    private:
        std::shared_ptr<VulkanContext>                       context_;
        vk::UniqueShaderModule                               module_;
        std::vector<Voxium::Platform::Render::ShaderBinding> bindings_;
        std::vector<vk::UniqueDescriptorSetLayout>           layoutDesc_;
        std::vector<vk::DescriptorSetLayout>                 layoutDesc2_;
        vk::ShaderStageFlagBits                              stage_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
