#pragma once

#include <memory>
#include <mutex>

#include <vulkan/vulkan.h>

#include "VkBuffer.h"
#include "VkImage.h"
#include "VkStagingResource.h"
#include "VkUtil.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    struct RenderPassAttachment
    {
        vk::Format      format;
        vk::ImageLayout targetLayout;
    };

    class VulkanContext final : public std::enable_shared_from_this<VulkanContext>
    {
    public:
        explicit VulkanContext(const std::vector<const char*>& surfaceExtensions);
        ~VulkanContext();
        VulkanContext(const VulkanContext& other)                = delete;
        VulkanContext(VulkanContext&& other) noexcept            = delete;
        VulkanContext& operator=(const VulkanContext& other)     = delete;
        VulkanContext& operator=(VulkanContext&& other) noexcept = delete;

        [[nodiscard]] bool InitializeOrValidateDeviceCompatibility(const vk::SurfaceKHR& surface);

    private:
        [[nodiscard]] bool InitializeDevice(const vk::SurfaceKHR& surface);
        [[nodiscard]] bool ValidateDeviceCompatibility(const vk::SurfaceKHR& surface) const;

    public:
        void WaitIdle() const;

        [[nodiscard]] util::SwapChainDescriptor GetSwapChainDescriptor(const vk::SurfaceKHR& surface) const;

        [[nodiscard]] vk::UniqueSwapchainKHR CreateSwapChain(const vk::SurfaceKHR&           surface,
                                                             uint32_t                        imageCount,
                                                             vk::SurfaceFormatKHR            format,
                                                             vk::PresentModeKHR              presentMode,
                                                             vk::Extent2D                    extent,
                                                             vk::SurfaceTransformFlagBitsKHR transform,
                                                             const vk::SwapchainKHR&         oldSwapchain) const;

        [[nodiscard]] std::vector<vk::UniqueImageView> CreateSwapChainViews(const vk::SwapchainKHR& swapChain,
                                                                            vk::Format              format) const;

        [[nodiscard]] vk::UniqueSampler CreateTextureSampler(vk::Filter             minFilter,
                                                             vk::Filter             magFilter,
                                                             vk::SamplerAddressMode addressMode,
                                                             vk::SamplerMipmapMode  mipmapMode,
                                                             float                  mipLodBias,
                                                             float                  minLod,
                                                             float                  maxLod,
                                                             bool                   enableAnisotropy,
                                                             float                  anisotropyLevel,
                                                             bool                   enableCompare,
                                                             vk::CompareOp          compareOp,
                                                             vk::BorderColor        borderColor,
                                                             bool                   unnormalizedCoords) const;

        [[nodiscard]] std::unique_ptr<VulkanImage> CreateImage(uint32_t                width,
                                                               uint32_t                height,
                                                               vk::Format              format,
                                                               vk::ImageUsageFlags     usageFlags,
                                                               vk::MemoryPropertyFlags memoryProperties);

        [[nodiscard]] vk::UniqueImageView
        CreateImageView(const vk::Image& image, vk::Format format, vk::ImageAspectFlags aspectFlags) const;

        [[nodiscard]] vk::UniqueFrameBuffer CreateFrameBuffer(const vk::RenderPass&             pass,
                                                              const vk::Extent2D&               extent,
                                                              const std::vector<vk::ImageView>& images) const;

        [[nodiscard]] std::vector<vk::UniqueFrameBuffer>
        CreateFrameBuffers(const vk::RenderPass&                   pass,
                           const vk::Extent2D&                     extent,
                           const std::vector<vk::UniqueImageView>& images) const;

        [[nodiscard]] std::vector<vk::UniqueFrameBuffer>
        CreateFrameBuffers(const vk::RenderPass&                          pass,
                           const vk::Extent2D&                            extent,
                           const std::vector<std::vector<vk::ImageView>>& images) const;

        [[nodiscard]] vk::UniqueRenderPass
        CreateSimpleRenderPass(const std::vector<RenderPassAttachment>& colorAttachments) const;

        [[nodiscard]] std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(uint32_t               stages,
                                                                                vk::CommandBufferLevel level) const;

        [[nodiscard]] util::StagingResource<vk::CommandBuffer> CreateCommandBuffer(uint32_t               stages,
                                                                                   vk::CommandBufferLevel level) const;

        [[nodiscard]] std::unique_ptr<VulkanBuffer> CreateBuffer(uint32_t                size,
                                                                 vk::BufferUsageFlags    usage,
                                                                 vk::SharingMode         sharingMode,
                                                                 vk::MemoryPropertyFlags memoryProperties);

        [[nodiscard]] std::unique_ptr<VulkanBuffer> VulkanContext::CreateCpuToGpuTransferBuffer(const void* data,
                                                                                                uint32_t    size);

        [[nodiscard]] vk::UniqueShaderModule CreateShaderModule(const std::vector<uint8_t>& bytes) const;

        [[nodiscard]] vk::UniqueDescriptorSetLayout
        CreateDescriptorSetLayout(const vk::DescriptorSetLayoutBinding& binding);

        [[nodiscard]] vk::UniqueDescriptorPool CreateDescriptorPool(uint32_t maxSets, vk::DescriptorType type) const;

        void UpdateDescriptorSets(const std::vector<vk::WriteDescriptorSet>& writes,
                                  const std::vector<vk::CopyDescriptorSet>&  copies) const;

        [[nodiscard]] std::vector<vk::UniqueDescriptorSet>
        CreateDescriptorSets(vk::DescriptorPool& descriptorPool, vk::DescriptorSetLayout& layout, uint32_t count) const;

        [[nodiscard]] util::StagingResource<vk::Semaphore> CreateSemaphore(uint32_t stages) const;

        [[nodiscard]] util::StagingResource<vk::Fence> CreateFence(uint32_t stages, bool signaled) const;

        void Wait(const vk::Fence& fence) const;
        void Reset(const vk::Fence& fence) const;

        vk::ResultValue<uint32_t> AcquireNextImage(const vk::SwapchainKHR& swapChain,
                                                   const vk::Semaphore&    semaphore) const;

        void Submit(const vk::CommandBuffer& commandBuffer,
                    const vk::Semaphore&     WaitSemaphore,
                    const vk::Semaphore&     signalSemaphore,
                    const vk::Fence&         fence) const;

        [[nodiscard]] vk::Result
        Present(const vk::Semaphore& WaitSemaphore, const vk::SwapchainKHR& swapChain, uint32_t imageIndex) const;

        [[nodiscard]] const vk::Instance& Instance() { return *instance_; }

    private:
        std::vector<const char*>         requiredLayers_;
        vk::UniqueInstance               instance_;
        vk::UniqueDebugUtilsMessengerEXT debugMessenger_;

        std::mutex               deviceInitLock_;
        bool                     deviceInitialized_ = false;
        std::vector<const char*> requiredDeviceExtensions_;
        vk::PhysicalDevice       physicalDevice_;
        util::QueueFamilyIndices familyIndices_;
        vk::UniqueDevice         device_;
        vma::Allocator           memoryAllocator_;

        vk::Queue graphicsQueue_;
        vk::Queue presentQueue_;

        vk::UniqueCommandPool   commandPool_;
        vk::UniquePipelineCache pipelineCache_;

        friend class VulkanBuffer;
        friend class RenderPipeline;
        friend class FrameBufferFormat;
        friend class FrameBuffer;
        friend class StaticTexture;
        friend class VulkanImage;
        friend class StaticVertexBuffer;
        friend class DynamicVertexBuffer;
        friend class UniformBuffer;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
