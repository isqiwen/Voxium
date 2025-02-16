#pragma once

#include <memory>

#include <vulkan.h>

namespace Voxium::Platform::Desktop::Vulkan
{
    class VulkanContext;

    class VulkanBuffer final
    {
    public:
        VulkanBuffer(std::shared_ptr<VulkanContext> context,
                     vk::UniqueBuffer               buffer,
                     vma::Allocation                memoryAllocation,
                     uint32_t                       size);
        ~VulkanBuffer();
        VulkanBuffer(const VulkanBuffer& other)                = delete;
        VulkanBuffer(VulkanBuffer&& other) noexcept            = delete;
        VulkanBuffer& operator=(const VulkanBuffer& other)     = delete;
        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept = delete;

        [[nodiscard]] uint32_t    Size() const { return size_; }
        [[nodiscard]] vk::Buffer& Buffer() { return *buffer_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        vk::UniqueBuffer               buffer_;
        vma::Allocation                memoryAllocation_;
        uint32_t                       size_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
