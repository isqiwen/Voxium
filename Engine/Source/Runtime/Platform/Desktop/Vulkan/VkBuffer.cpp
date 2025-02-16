#include "VkBuffer.h"
#include "VkContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    VulkanBuffer::VulkanBuffer(std::shared_ptr<VulkanContext> context,
                               vk::UniqueBuffer               buffer,
                               vma::Allocation                memoryAllocation,
                               const uint32_t                 size) :
        context_(std::move(context)), buffer_(std::move(buffer)), memoryAllocation_(memoryAllocation), size_(size)
    {}

    VulkanBuffer::~VulkanBuffer() { context_->memoryAllocator_.freeMemory(memoryAllocation_); }
} // namespace Voxium::Platform::Desktop::Vulkan
