#pragma once

#include "Platform/Render/IUniformBinding.h"

#include "VkContext.h"
#include "VkShader.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class UniformBinding final : public Voxium::Platform::Render::IUniformBinding
    {
    public:
        UniformBinding(std::shared_ptr<VulkanContext>                context,
                       std::shared_ptr<IShader>                      shader,
                       uint32_t                                      binding,
                       uint32_t                                      stages,
                       const std::shared_ptr<Voxium::Platform::Render::UniformBuffer>& uniformBuffer);

        ~UniformBinding() override;

        void tick();

        void Update(const std::shared_ptr<Voxium::Platform::Render::UniformBuffer> uniformBuffer) override
        {
            Update(uniformBuffer, true);
        }

        [[nodiscard]] vk::DescriptorSet& Get() { return *descriptorSets_[readIndex_]; }

        [[nodiscard]] std::shared_ptr<IShader>& GetShader() { return shader_; }

        [[nodiscard]] uint32_t GetBinding() const { return binding_; }

        [[nodiscard]] uint32_t GetBindingSize() const { return bindingSize_; }

        void UpdateNext();

    private:
        void Update(std::shared_ptr<Voxium::Platform::Render::UniformBuffer> uniformBuffer, bool RegisterUser);

        std::shared_ptr<VulkanContext> context_;
        std::shared_ptr<IShader>       shader_;
        uint32_t                       binding_;
        uint32_t                       bindingSize_;

        std::vector<std::shared_ptr<IUniformBuffer>> buffers_;

        vk::UniqueDescriptorPool             descriptorPool_;
        std::vector<vk::UniqueDescriptorSet> descriptorSets_;

        uint32_t readIndex_      = 0;
        uint32_t leftoverWrites_ = 0;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
