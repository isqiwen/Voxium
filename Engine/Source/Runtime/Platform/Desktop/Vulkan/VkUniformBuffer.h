#pragma once

#include "Platform/Render/IUniformBuffer.h"

#include "VkContext.h"
#include "VkShader.h"
#include "VkUniformBinding.h"


namespace Voxium::Platform::Desktop::Vulkan
{
    class UniformBuffer final : public Voxium::Platform::Render::IUniformBuffer
    {
    public:
        UniformBuffer(std::shared_ptr<VulkanContext> context, uint32_t stages, const std::vector<uint8_t>& initialData);

        void Tick();

        void Write(const std::vector<uint8_t>& data) override;

        [[nodiscard]] vk::Buffer& Buffer() { return buffers_[readIndex_]->Buffer(); }

        void RegisterUser(const std::weak_ptr<UniformBinding>& binding);

        void UnRegisterUser(const UniformBinding* binding);

    private:
        std::shared_ptr<VulkanContext> context_;

        std::vector<std::unique_ptr<VulkanBuffer>> buffers_;
        std::vector<uint8_t>                       uniformData_;

        std::set<std::weak_ptr<UniformBinding>, std::owner_less<>> dependents_;

        uint32_t readIndex_      = 0;
        uint32_t leftoverWrites_ = 0;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
