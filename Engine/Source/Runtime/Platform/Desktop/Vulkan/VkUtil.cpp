#include "VkUtil.h"

#include <map>

#include <vulkan/vulkan.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace Voxium::Platform::Desktop::Vulkan
{
    bool QueueFamilyIndices::IsComplete() const { return graphicsFamily_.has_value() && presentFamily_.has_value(); }

    std::set<uint32_t> QueueFamilyIndices::AsSet() const
    {
        std::set<uint32_t> set {
            graphicsFamily_.has_value() ? graphicsFamily_.value() : 0xFFFFFFFF,
            presentFamily_.has_value() ? presentFamily_.value() : 0xFFFFFFFF,
        };
        set.erase(0xFFFFFFFF);
        return set;
    }

    bool SwapChainDescriptor::IsValid()  const { return !formats_.empty() && !presentModes_.empty(); }

    vk::SurfaceFormatKHR SwapChainDescriptor::GetOptimalFormat() const
    {
        for (const auto& availableFormat : formats_)
            if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
                availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return availableFormat;
        return formats_[0];
    }

    vk::PresentModeKHR SwapChainDescriptor::GetOptimalPresentMode() const
    {
        for (const auto& availablePresentMode : presentModes_)
            if (availablePresentMode == vk::PresentModeKHR::eMailbox)
                return availablePresentMode;
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapChainDescriptor::GetOptimalExtent(const uint32_t width, const uint32_t height) const
    {
        if (capabilities_.currentExtent.width != UINT32_MAX)
            return capabilities_.currentExtent;

        return vk::Extent2D {
            std::max(capabilities_.minImageExtent.width, std::min(capabilities_.maxImageExtent.width, width)),
            std::max(capabilities_.minImageExtent.height, std::min(capabilities_.maxImageExtent.height, height))};
    }

    uint32_t SwapChainDescriptor::GetOptimalImageCount() const
    {
        const auto count = capabilities_.minImageCount + 1;
        if (capabilities_.maxImageCount > 0 && capabilities_.maxImageCount < count)
            return capabilities_.maxImageCount;
        return count;
    }

    vk::SurfaceTransformFlagBitsKHR SwapChainDescriptor::GetTransform() const { return capabilities_.currentTransform; }

    void InitializeDispatcher()
    {
        const vk::DynamicLoader dl;
        const auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
    }

    bool AreAllLayersAvailable(const std::vector<const char*>& requestedLayers)
    {
        auto availableLayers = vk::enumerateInstanceLayerProperties();

        for (const auto* requested : requestedLayers)
        {
            for (auto available : availableLayers)
                if (strcmp(requested, available.layerName.data()) == 0)
                    goto found;

            return false;
        found:;
        }

        return true;
    }

    int32_t GetPhysicalDeviceScore(const vk::PhysicalDevice& device)
    {
        auto score = 0;

        const auto props = device.getProperties();

        score += 1000 * (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu);
        score += 100 * (props.deviceType == vk::PhysicalDeviceType::eIntegratedGpu);

        return score;
    }

    QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        QueueFamilyIndices indices;

        auto     queueFamilies = device.getQueueFamilyProperties();
        uint32_t i             = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
                indices.graphicsFamily_ = i;

            if (device.getSurfaceSupportKHR(i, surface))
                indices.presentFamily_ = i;

            i++;
        }

        return indices;
    }

    bool AreAllExtensionsSupported(const vk::PhysicalDevice& device, const std::vector<const char*>& extensions)
    {
        auto                  availableExtensions = device.enumerateDeviceExtensionProperties();
        std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
    }

    SwapChainDescriptor GetSwapChainDescriptor(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        return SwapChainDescriptor {device.getSurfaceCapabilitiesKHR(surface),
                                    device.getSurfaceFormatsKHR(surface),
                                    device.getSurfacePresentModesKHR(surface)};
    }

    std::optional<PhysicalDeviceDescriptor>
    GetPhysicalDeviceDescriptor(const vk::PhysicalDevice&       device,
                                const vk::SurfaceKHR&           surface,
                                const std::vector<const char*>& requiredExtensions)
    {
        const auto indices = FindQueueFamilies(device, surface);
        if (!indices.IsComplete())
            return {};

        if (!AreAllExtensionsSupported(device, requiredExtensions))
            return {};

        const auto swapChainDescriptor = GetSwapChainDescriptor(device, surface);
        if (!swapChainDescriptor.IsValid() )
            return {};

        return PhysicalDeviceDescriptor {device, indices};
    }

    PhysicalDeviceDescriptor FindOptimalPhysicalDevice(const vk::Instance&             instance,
                                                       const vk::SurfaceKHR&           surface,
                                                       const std::vector<const char*>& requiredExtensions)
    {
        auto                                             devices = instance.enumeratePhysicalDevices();
        std::multimap<int32_t, PhysicalDeviceDescriptor> candidates;

        for (auto device : devices)
        {
            const auto descriptor = GetPhysicalDeviceDescriptor(device, surface, requiredExtensions);
            if (descriptor.has_value())
                candidates.emplace(GetPhysicalDeviceScore(device), descriptor.value());
        }

        if (candidates.empty() || candidates.rbegin()->first <= 0)
            throw std::runtime_error("Failed to find a suitable GPU.");

        return candidates.rbegin()->second;
    }

    vk::UniqueDevice CreateLogicalDevice(const vk::PhysicalDevice&       physicalDevice,
                                         const QueueFamilyIndices&       familyIndices,
                                         const std::vector<const char*>& requiredLayers,
                                         const std::vector<const char*>& requiredExtensions)
    {
        auto                                   uniqueFamilyIndices = familyIndices.AsSet();
        const auto                             queuePriority       = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;
        deviceQueueCreateInfos.reserve(uniqueFamilyIndices.size());
        for (auto index : uniqueFamilyIndices)
            deviceQueueCreateInfos.push_back(vk::DeviceQueueCreateInfo({}, index, 1, &queuePriority));

        vk::PhysicalDeviceFeatures deviceFeatures;
        vk::DeviceCreateInfo       deviceCreateInfo(
            {}, deviceQueueCreateInfos, requiredLayers, requiredExtensions, &deviceFeatures);
        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT ext1 {true};
        deviceCreateInfo.setPNext(&ext1);

        auto device = physicalDevice.createDeviceUnique(deviceCreateInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*device);
        return device;
    }

    vk::UniqueCommandPool CreateCommandPool(const vk::Device& device, const uint32_t graphicsFamily_)
    {
        return device.CreateCommandPoolUnique(
            vk::CommandPoolCreateInfo {vk::CommandPoolCreateFlagBits::eResetCommandBuffer, graphicsFamily_});
    }

    vk::UniquePipelineCache CreatePipelineCache(const vk::Device& device)
    {
        return device.CreatePipelineCacheUnique(vk::PipelineCacheCreateInfo {{}, 0, nullptr});
    }

    void DirectExecuteCommands(const vk::Device&                              device,
                               const vk::CommandPool&                         commandPool,
                               const vk::Queue&                               queue,
                               const std::function<void(vk::CommandBuffer&)>& commands)
    {
        auto  buffers = device.allocateCommandBuffersUnique({commandPool, vk::CommandBufferLevel::ePrimary, 1});
        auto& buffer  = *buffers[0];

        buffer.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        commands(buffer);
        buffer.end();

        auto buffers2 = std::vector {buffer};
        queue.Submit({{{}, {}, buffers2, {}}}, nullptr);
        queue.WaitIdle();
    }

    ImageMemoryBarrierSet CreateImageMemoryBarriers(const std::vector<ImageTransitionInfo>& transitions)
    {
        vk::PipelineStageFlags              srcStageMask_ = {}, dstStageMask_ = {};
        std::vector<vk::ImageMemoryBarrier> barriers_;

        for (const auto& transition : transitions)
        {
            vk::AccessFlags srcAccessMask, dstAccessMask;
            if (transition.oldLayout_ == vk::ImageLayout::eUndefined &&
                transition.newLayout_ == vk::ImageLayout::eTransferDstOptimal)
            {
                srcAccessMask = {};
                dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageMask_ |= vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageMask_ |= vk::PipelineStageFlagBits::eTransfer;
            }
            else if (transition.oldLayout_ == vk::ImageLayout::eTransferDstOptimal &&
                     transition.newLayout_ == vk::ImageLayout::eShaderReadOnlyOptimal)
            {
                srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageMask_ |= vk::PipelineStageFlagBits::eTransfer;
                dstStageMask_ |= vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (transition.oldLayout_ == vk::ImageLayout::eUndefined &&
                     transition.newLayout_ == vk::ImageLayout::eColorAttachmentOptimal)
            {
                srcAccessMask = {};
                dstAccessMask = vk::AccessFlagBits::eShaderWrite;
                srcStageMask_ |= vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageMask_ |= vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (transition.oldLayout_ == vk::ImageLayout::eUndefined &&
                     transition.newLayout_ == vk::ImageLayout::eDepthStencilAttachmentOptimal)
            {
                srcAccessMask = {};
                dstAccessMask =
                    vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                srcStageMask_ |= vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageMask_ |= vk::PipelineStageFlagBits::eEarlyFragmentTests;
            }
            else if (transition.oldLayout_ == vk::ImageLayout::eColorAttachmentOptimal &&
                     transition.newLayout_ == vk::ImageLayout::eShaderReadOnlyOptimal)
            {
                srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageMask_ |= vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dstStageMask_ |= vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (transition.oldLayout_ == vk::ImageLayout::eDepthStencilAttachmentOptimal &&
                     transition.newLayout_ == vk::ImageLayout::eShaderReadOnlyOptimal)
            {
                srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageMask_ |= vk::PipelineStageFlagBits::eLateFragmentTests;
                dstStageMask_ |= vk::PipelineStageFlagBits::eFragmentShader;
            }
            else
            {
                throw std::invalid_argument("Unsupported layout transition!");
            }

            barriers_.push_back({srcAccessMask,
                                dstAccessMask,
                                transition.oldLayout_,
                                transition.newLayout_,
                                VK_QUEUE_FAMILY_IGNORED,
                                VK_QUEUE_FAMILY_IGNORED,
                                transition.image_,
                                vk::ImageSubresourceRange {transition.aspectFlags_, 0, 1, 0, 1}});
        }

        return ImageMemoryBarrierSet {barriers_, srcStageMask_, dstStageMask_};
    }

    void TransitionImageLayouts(const vk::CommandBuffer& cmd, const std::vector<ImageTransitionInfo>& transitions)
    {
        const auto barrierSet = CreateImageMemoryBarriers(transitions);
        cmd.pipelineBarrier(barrierSet.srcStageMask_, barrierSet.dstStageMask_, {}, {}, {}, barrierSet.barriers_);
    }

    void TransitionImageLayoutsImmediate(const vk::Device&                       device,
                                         const vk::CommandPool&                  commandPool,
                                         const vk::Queue&                        graphicsQueue,
                                         const std::vector<ImageTransitionInfo>& transitions)
    {
        DirectExecuteCommands(device, commandPool, graphicsQueue, [&](vk::CommandBuffer& cmd) {
            TransitionImageLayouts(cmd, transitions);
        });
    }

    void
    CopyBufferToBuffer(const vk::CommandBuffer& cmd, const vk::Buffer& src, const vk::Buffer& dst, const uint32_t size)
    {
        const auto regions = {vk::BufferCopy {0, 0, size}};
        cmd.copyBuffer(src, dst, regions);
    }

    void CopyBufferToBufferImmediate(const vk::Device&      device,
                                     const vk::CommandPool& commandPool,
                                     const vk::Queue&       graphicsQueue,
                                     const vk::Buffer&      src,
                                     const vk::Buffer&      dst,
                                     const uint32_t         size)
    {
        DirectExecuteCommands(device, commandPool, graphicsQueue, [&](vk::CommandBuffer& cmd) {
            CopyBufferToBuffer(cmd, src, dst, size);
        });
    }

    void CopyBufferToImage(const vk::CommandBuffer& cmd,
                           const vk::Buffer&        buffer,
                           const vk::Image&         image,
                           const uint32_t           width,
                           const uint32_t           height)
    {
        cmd.CopyBufferToImage(
            buffer,
            image,
            vk::ImageLayout::eTransferDstOptimal,
            {{0, width, height, {vk::ImageAspectFlagBits::eColor, 0, 0, 1}, {0, 0, 0}, {width, height, 1}}});
    }

    void CopyBufferToImageImmediate(const vk::Device&      device,
                                    const vk::CommandPool& commandPool,
                                    const vk::Queue&       graphicsQueue,
                                    const vk::Buffer&      buffer,
                                    const vk::Image&       image,
                                    const uint32_t         width,
                                    const uint32_t         height)
    {
        DirectExecuteCommands(device, commandPool, graphicsQueue, [&](vk::CommandBuffer& cmd) {
            CopyBufferToImage(cmd, buffer, image, width, height);
        });
    }

    vk::Format ToVulkanFormat(const Voxium::Platform::Render::TextureFormat format)
    {
        switch (format)
        {
            case Voxium::Platform::Render::TextureFormat::R8G8B8A8_SRGB:
                return vk::Format::eR8G8B8A8Srgb;
            case Voxium::Platform::Render::TextureFormat::B8G8R8A8_SRGB:
                return vk::Format::eB8G8R8A8Srgb;
            case Voxium::Platform::Render::TextureFormat::R32G32B32A32S:
                return vk::Format::eR32G32B32A32Sfloat;
            case Voxium::Platform::Render::TextureFormat::D32SFLOAT_S8UINT:
                return vk::Format::eD32SfloatS8Uint;
        }
        throw std::runtime_error("Invalid type.");
    }

    uint32_t FindMemoryType(const vk::PhysicalDevice&     device,
                            const uint32_t                memoryTypeBits,
                            const vk::MemoryPropertyFlags memoryProperties)
    {
        const auto properties = device.getMemoryProperties();
        for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
        {
            if ((memoryTypeBits & (1 << i)) &&
                (properties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
            {
                return i;
            }
        }
        throw std::runtime_error("Failed to find suitable memory type.");
    }
} // namespace Voxium::Platform::Desktop::Vulkan
