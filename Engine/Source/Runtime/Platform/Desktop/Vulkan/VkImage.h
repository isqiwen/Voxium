#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

namespace Voxium::Platform::Desktop::Vulkan
{
    class VulkanContext;

    class VulkanImage final
    {
    public:
        VulkanImage(std::shared_ptr<VulkanContext> context, vk::UniqueImage image, vma::Allocation memoryAllocation);
        ~VulkanImage();
        VulkanImage(const VulkanImage& other)                = delete;
        VulkanImage(VulkanImage&& other) noexcept            = delete;
        VulkanImage& operator=(const VulkanImage& other)     = delete;
        VulkanImage& operator=(VulkanImage&& other) noexcept = delete;

        [[nodiscard]] vk::Image& Get() { return *image_; }

    private:
        std::shared_ptr<VulkanContext> context_;
        vk::UniqueImage                image_;
        vma::Allocation                memoryAllocation_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
