#pragma once

#include <optional>
#include <set>
#include <vector>

#include <vulkan/vulkan.h>

#include "Platform/Render/ITexture.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily_;
        std::optional<uint32_t> presentFamily_;

        [[nodiscard]] bool               IsComplete() const;
        [[nodiscard]] std::set<uint32_t> AsSet() const;
    };

    struct SwapChainDescriptor
    {
        vk::SurfaceCapabilitiesKHR        capabilities_;
        std::vector<vk::SurfaceFormatKHR> formats_;
        std::vector<vk::PresentModeKHR>   presentModes_;

        [[nodiscard]] bool                            IsValid() const;
        [[nodiscard]] vk::SurfaceFormatKHR            GetOptimalFormat() const;
        [[nodiscard]] vk::PresentModeKHR              GetOptimalPresentMode() const;
        [[nodiscard]] vk::Extent2D                    GetOptimalExtent(uint32_t width, uint32_t height) const;
        [[nodiscard]] uint32_t                        GetOptimalImageCount() const;
        [[nodiscard]] vk::SurfaceTransformFlagBitsKHR GetTransform() const;
    };

    struct PhysicalDeviceDescriptor
    {
        vk::PhysicalDevice device;
        QueueFamilyIndices familyIndices;
    };

    struct ImageTransitionInfo
    {
        vk::Image            image_;
        vk::ImageAspectFlags aspectFlags_;
        vk::ImageLayout      oldLayout_;
        vk::ImageLayout      newLayout_;
    };

    struct ImageMemoryBarrierSet
    {
        const std::vector<vk::ImageMemoryBarrier> barriers_;
        const vk::PipelineStageFlags              srcStageMask_;
        const vk::PipelineStageFlags              dstStageMask_;
    };

    void InitializeDispatcher();

    [[nodiscard]] bool AreAllLayersAvailable(const std::vector<const char*>& requestedLayers);

    [[nodiscard]] int32_t            GetPhysicalDeviceScore(const vk::PhysicalDevice& device);
    [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
    [[nodiscard]] bool               AreAllExtensionsSupported(const vk::PhysicalDevice&       device,
                                                               const std::vector<const char*>& extensions);
    [[nodiscard]] SwapChainDescriptor GetSwapChainDescriptor(const vk::PhysicalDevice& device,
                                                             const vk::SurfaceKHR&     surface);
    [[nodiscard]] std::optional<PhysicalDeviceDescriptor>
    GetPhysicalDeviceDescriptor(const vk::PhysicalDevice&       device,
                                const vk::SurfaceKHR&           surface,
                                const std::vector<const char*>& requiredExtensions);
    [[nodiscard]] PhysicalDeviceDescriptor
    FindOptimalPhysicalDevice(const vk::Instance&             instance,
                              const vk::SurfaceKHR&           surface,
                              const std::vector<const char*>& requiredExtensions);

    [[nodiscard]] vk::UniqueDevice CreateLogicalDevice(const vk::PhysicalDevice&       physicalDevice,
                                                       const QueueFamilyIndices&       familyIndices,
                                                       const std::vector<const char*>& requiredLayers,
                                                       const std::vector<const char*>& requiredExtensions);

    [[nodiscard]] vk::UniqueCommandPool CreateCommandPool(const vk::Device& device, uint32_t graphicsFamily_);

    [[nodiscard]] vk::UniquePipelineCache CreatePipelineCache(const vk::Device& device);

    void DirectExecuteCommands(const vk::Device&                              device,
                               const vk::CommandPool&                         commandPool,
                               const vk::Queue&                               queue,
                               const std::function<void(vk::CommandBuffer&)>& commands);

    ImageMemoryBarrierSet CreateImageMemoryBarriers(const std::vector<ImageTransitionInfo>& transitions);

    void TransitionImageLayouts(const vk::CommandBuffer& cmd, const std::vector<ImageTransitionInfo>& transitions);

    void TransitionImageLayoutsImmediate(const vk::Device&                       device,
                                         const vk::CommandPool&                  commandPool,
                                         const vk::Queue&                        graphicsQueue,
                                         const std::vector<ImageTransitionInfo>& transitions);

    void
    CopyBufferToBuffer(const vk::CommandBuffer& cmd, const vk::Buffer& src, const vk::Buffer& dst, const uint32_t size);

    void CopyBufferToBufferImmediate(const vk::Device&      device,
                                     const vk::CommandPool& commandPool,
                                     const vk::Queue&       graphicsQueue,
                                     const vk::Buffer&      src,
                                     const vk::Buffer&      dst,
                                     const uint32_t         size);

    void CopyBufferToImage(const vk::CommandBuffer& cmd,
                           const vk::Buffer&        buffer,
                           const vk::Image&         image,
                           uint32_t                 width,
                           uint32_t                 height);

    void CopyBufferToImageImmediate(const vk::Device&      device,
                                    const vk::CommandPool& commandPool,
                                    const vk::Queue&       graphicsQueue,
                                    const vk::Buffer&      buffer,
                                    const vk::Image&       image,
                                    uint32_t               width,
                                    uint32_t               height);

    [[nodiscard]] vk::Format ToVulkanFormat(Voxium::Platform::Render::TextureFormat format);

    [[nodiscard]] uint32_t
    FindMemoryType(const vk::PhysicalDevice& device, uint32_t memoryTypeBits, vk::MemoryPropertyFlags memoryProperties);
} // namespace Voxium::Platform::Desktop::Vulkan
