#include "VkImage.h"

#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    VulkanImage::VulkanImage(std::shared_ptr<VulkanContext> context,
                             vk::UniqueImage                image,
                             const vma::Allocation          memoryAllocation) :
        context_(std::move(context)), image_(std::move(image)), memoryAllocation_(memoryAllocation)
    {}

    VulkanImage::~VulkanImage() { context_->memoryAllocator_.freeMemory(memoryAllocation_); }
} // namespace Voxium::Platform::Desktop::Vulkan
