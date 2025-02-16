#include "VkContext.h"

#include <iostream>

#include "VkUtil.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    std::vector<const char*> getRequiredLayers()
    {
        return std::vector<const char*> {
#ifdef DB_DEBUG
            "VK_LAYER_KHRONOS_validation"
#endif
        };
    }

    std::vector<const char*> getRequiredInstanceExtensions()
    {
        return std::vector<const char*> {
#ifdef DB_DEBUG
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
        };
    }

    std::vector<const char*> getRequiredDeviceExtensions()
    {
        return std::vector<const char*> {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            // VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        };
    }

    vk::ApplicationInfo getApplicationInfo()
    {
        return vk::ApplicationInfo {
            "DigBuild", VK_MAKE_VERSION(1, 0, 0), "DigBuild", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0};
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL logDebugMessage(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                   const VkDebugUtilsMessageTypeFlagsEXT       messageTypes,
                                                   VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
                                                   void* /*pUserData*/)
    {
        std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
                  << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
        std::cerr << "\t"
                  << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
        std::cerr << "\t"
                  << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
        std::cerr << "\t"
                  << "message         = <" << pCallbackData->pMessage << ">" << std::endl;
        return true;
    }

    VulkanContext::VulkanContext(const std::vector<const char*>& surfaceExtensions)
    {
        util::InitializeDispatcher();

        m_requiredLayers = getRequiredLayers();
        if (!util::AreAllLayersAvailable(m_requiredLayers))
            throw std::runtime_error("Not all requested layers are available.");

        const auto instanceExtensions = getRequiredInstanceExtensions();

        std::vector<const char*> requiredExtensions;
        requiredExtensions.reserve(surfaceExtensions.size() + instanceExtensions.size());
        requiredExtensions.insert(requiredExtensions.end(), surfaceExtensions.begin(), surfaceExtensions.end());
        requiredExtensions.insert(requiredExtensions.end(), instanceExtensions.begin(), instanceExtensions.end());

        const auto appInfo = getApplicationInfo();
        m_instance         = vk::createInstanceUnique(vk::InstanceCreateInfo {{},
                                                                      &appInfo,
                                                                      static_cast<uint32_t>(m_requiredLayers.size()),
                                                                      m_requiredLayers.data(),
                                                                      static_cast<uint32_t>(requiredExtensions.size()),
                                                                      requiredExtensions.data()});
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);

#ifdef DB_DEBUG
        m_debugMessenger = m_instance->createDebugUtilsMessengerEXTUnique(vk::DebugUtilsMessengerCreateInfoEXT(
            {},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &logDebugMessage));
#endif
    }

    VulkanContext::~VulkanContext()
    {
        WaitIdle();
        m_memoryAllocator.destroy();
    }

    bool VulkanContext::InitializeOrValidateDeviceCompatibility(const vk::SurfaceKHR& surface)
    {
        m_deviceInitLock.lock();
        if (m_deviceInitialized)
        {
            m_deviceInitLock.unlock();
            return ValidateDeviceCompatibility(surface);
        }
        const auto result = InitializeDevice(surface);
        m_deviceInitLock.unlock();
        return result;
    }

    bool VulkanContext::InitializeDevice(const vk::SurfaceKHR& surface)
    {
        m_requiredDeviceExtensions = getRequiredDeviceExtensions();

        const auto deviceDescriptor = util::FindOptimalPhysicalDevice(*m_instance, surface, m_requiredDeviceExtensions);
        m_physicalDevice            = deviceDescriptor.device;
        m_familyIndices             = deviceDescriptor.familyIndices;

        m_device =
            util::CreateLogicalDevice(m_physicalDevice, m_familyIndices, m_requiredLayers, m_requiredDeviceExtensions);

        m_graphicsQueue = m_device->getQueue(m_familyIndices.graphicsFamily_.value(), 0);
        m_presentQueue  = m_device->getQueue(m_familyIndices.presentFamily_.value(), 0);

        m_commandPool   = util::CreateCommandPool(*m_device, m_familyIndices.graphicsFamily_.value());
        m_pipelineCache = util::CreatePipelineCache(*m_device);

        m_memoryAllocator = vma::createAllocator({{},
                                                  m_physicalDevice,
                                                  *m_device,
                                                  0,
                                                  nullptr,
                                                  nullptr,
                                                  0,
                                                  nullptr,
                                                  nullptr,
                                                  nullptr,
                                                  *m_instance,
                                                  VK_API_VERSION_1_0});

        m_deviceInitialized = true;
        return true;
    }

    bool VulkanContext::ValidateDeviceCompatibility(const vk::SurfaceKHR& surface) const
    {
        return util::GetPhysicalDeviceDescriptor(m_physicalDevice, surface, m_requiredDeviceExtensions).has_value();
    }

    void VulkanContext::WaitIdle() const
    {
        if (m_presentQueue)
            m_presentQueue.WaitIdle();
        if (m_graphicsQueue)
            m_graphicsQueue.WaitIdle();
        if (m_device)
            m_device->WaitIdle();
    }

    [[nodiscard]] util::SwapChainDescriptor VulkanContext::GetSwapChainDescriptor(const vk::SurfaceKHR& surface) const
    {
        return util::GetSwapChainDescriptor(m_physicalDevice, surface);
    }

    [[nodiscard]] vk::UniqueSwapchainKHR VulkanContext::CreateSwapChain(const vk::SurfaceKHR&      surface,
                                                                        const uint32_t             imageCount,
                                                                        const vk::SurfaceFormatKHR format,
                                                                        const vk::PresentModeKHR   presentMode,
                                                                        const vk::Extent2D         extent,
                                                                        const vk::SurfaceTransformFlagBitsKHR transform,
                                                                        const vk::SwapchainKHR& oldSwapchain) const
    {
        vk::SwapchainCreateInfoKHR swapchainCreateInfo({},
                                                       surface,
                                                       imageCount,
                                                       format.format,
                                                       format.colorSpace,
                                                       extent,
                                                       1,
                                                       vk::ImageUsageFlagBits::eColorAttachment,
                                                       vk::SharingMode::eExclusive,
                                                       0,
                                                       nullptr,
                                                       transform,
                                                       vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                       presentMode,
                                                       true,
                                                       oldSwapchain);
        uint32_t queueFamilyIndices[] = {m_familyIndices.graphicsFamily_.value(), m_familyIndices.presentFamily_.value()};
        if (m_familyIndices.graphicsFamily_ != m_familyIndices.presentFamily_)
        {
            swapchainCreateInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
        }
        return m_device->CreateSwapChainKHRUnique(swapchainCreateInfo);
    }

    [[nodiscard]] std::vector<vk::UniqueImageView>
    VulkanContext::CreateSwapChainViews(const vk::SwapchainKHR& swapChain, const vk::Format format) const
    {
        const auto                       images = m_device->getSwapchainImagesKHR(swapChain);
        std::vector<vk::UniqueImageView> views;
        views.reserve(images.size());
        for (const auto& image : images)
            views.push_back(CreateImageView(image, format, vk::ImageAspectFlagBits::eColor));
        return std::move(views);
    }

    vk::UniqueSampler VulkanContext::CreateTextureSampler(const vk::Filter             minFilter,
                                                          const vk::Filter             magFilter,
                                                          const vk::SamplerAddressMode addressMode,
                                                          const vk::SamplerMipmapMode  mipmapMode,
                                                          const float                  mipLodBias,
                                                          const float                  minLod,
                                                          const float                  maxLod,
                                                          const bool                   enableAnisotropy,
                                                          const float                  anisotropyLevel,
                                                          const bool                   enableCompare,
                                                          const vk::CompareOp          compareOp,
                                                          const vk::BorderColor        borderColor,
                                                          const bool                   unnormalizedCoords) const
    {
        return m_device->createSamplerUnique({{},
                                              magFilter,
                                              minFilter,
                                              mipmapMode,
                                              addressMode,
                                              addressMode,
                                              addressMode,
                                              mipLodBias,
                                              enableAnisotropy,
                                              anisotropyLevel,
                                              enableCompare,
                                              compareOp,
                                              minLod,
                                              maxLod,
                                              borderColor,
                                              unnormalizedCoords});
    }

    std::unique_ptr<VulkanImage> VulkanContext::CreateImage(const uint32_t                width,
                                                            const uint32_t                height,
                                                            const vk::Format              format,
                                                            const vk::ImageUsageFlags     usageFlags,
                                                            const vk::MemoryPropertyFlags memoryProperties)
    {
        const auto queueIndex = m_familyIndices.graphicsFamily_.value();
        auto       image      = m_device->CreateImageUnique({{},
                                                             vk::ImageType::e2D,
                                                             format,
                                                             vk::Extent3D {width, height, 1},
                                                             1,
                                                             1,
                                                             vk::SampleCountFlagBits::e1,
                                                             vk::ImageTiling::eOptimal,
                                                             usageFlags,
                                                             vk::SharingMode::eExclusive,
                                                             1,
                                                             &queueIndex,
                                                             vk::ImageLayout::eUndefined});

        const auto memoryAllocation =
            m_memoryAllocator.allocateMemoryForImage(*image, {{}, vma::MemoryUsage::eGpuOnly, memoryProperties});
        m_memoryAllocator.bindImageMemory(memoryAllocation, *image);

        return std::make_unique<VulkanImage>(shared_from_this(), std::move(image), memoryAllocation);
    }

    [[nodiscard]] vk::UniqueImageView VulkanContext::CreateImageView(const vk::Image&           image,
                                                                     const vk::Format           format,
                                                                     const vk::ImageAspectFlags aspectFlags) const
    {
        return m_device->CreateImageViewUnique(
            {{}, image, vk::ImageViewType::e2D, format, {}, vk::ImageSubresourceRange {aspectFlags, 0, 1, 0, 1}});
    }

    [[nodiscard]] vk::UniqueFrameBuffer VulkanContext::CreateFrameBuffer(const vk::RenderPass&             pass,
                                                                         const vk::Extent2D&               extent,
                                                                         const std::vector<vk::ImageView>& images) const
    {
        return m_device->CreateFrameBufferUnique(
            {{}, pass, static_cast<uint32_t>(images.size()), images.data(), extent.width, extent.height, 1});
    }

    [[nodiscard]] std::vector<vk::UniqueFrameBuffer>
    VulkanContext::CreateFrameBuffers(const vk::RenderPass&                   pass,
                                      const vk::Extent2D&                     extent,
                                      const std::vector<vk::UniqueImageView>& images) const
    {
        std::vector<vk::UniqueFrameBuffer> FrameBuffers;
        FrameBuffers.reserve(images.size());
        for (auto i = 0u; i < images.size(); i++)
            FrameBuffers.push_back(CreateFrameBuffer(pass, extent, {*images[i]}));
        return std::move(FrameBuffers);
    }

    std::vector<vk::UniqueFrameBuffer>
    VulkanContext::CreateFrameBuffers(const vk::RenderPass&                          pass,
                                      const vk::Extent2D&                            extent,
                                      const std::vector<std::vector<vk::ImageView>>& images) const
    {
        std::vector<vk::UniqueFrameBuffer> FrameBuffers;
        FrameBuffers.reserve(images.size());
        for (auto i = 0u; i < images.size(); i++)
            FrameBuffers.push_back(CreateFrameBuffer(pass, extent, images[i]));
        return std::move(FrameBuffers);
    }

    [[nodiscard]] vk::UniqueRenderPass
    VulkanContext::CreateSimpleRenderPass(const std::vector<RenderPassAttachment>& colorAttachments) const
    {
        std::vector<vk::AttachmentDescription> attachments;
        std::vector<vk::AttachmentReference>   references;
        attachments.reserve(colorAttachments.size());
        references.reserve(colorAttachments.size());
        for (const auto& attachment : colorAttachments)
        {
            attachments.push_back({{},
                                   attachment.format,
                                   vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   attachment.targetLayout});
            references.emplace_back(static_cast<uint32_t>(references.size()), vk::ImageLayout::eColorAttachmentOptimal);
        }

        const vk::SubpassDescription subpass {{},
                                              vk::PipelineBindPoint::eGraphics,
                                              0,
                                              nullptr,
                                              static_cast<uint32_t>(references.size()),
                                              references.data(),
                                              nullptr,
                                              nullptr,
                                              0,
                                              nullptr};
        const vk::SubpassDependency  subpassDependency {VK_SUBPASS_EXTERNAL,
                                                       0,
                                                       vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                                       vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                                        {},
                                                       vk::AccessFlagBits::eColorAttachmentWrite};

        auto subpasses   = std::vector {subpass};
        auto subpassDeps = std::vector {subpassDependency};
        return m_device->createRenderPassUnique({{}, attachments, subpasses, subpassDeps});
    }

    std::vector<vk::UniqueCommandBuffer> VulkanContext::CreateCommandBuffers(const uint32_t               stages,
                                                                             const vk::CommandBufferLevel level) const
    {
        return m_device->allocateCommandBuffersUnique({*m_commandPool, level, stages});
    }

    [[nodiscard]] util::StagingResource<vk::CommandBuffer>
    VulkanContext::CreateCommandBuffer(const uint32_t stages, const vk::CommandBufferLevel level) const
    {
        auto commandBuffers = m_device->allocateCommandBuffersUnique({*m_commandPool, level, stages});
        return util::StagingResource(std::move(commandBuffers));
    }

    [[nodiscard]] std::unique_ptr<VulkanBuffer>
    VulkanContext::CreateBuffer(const uint32_t                size,
                                const vk::BufferUsageFlags    usage,
                                const vk::SharingMode         sharingMode,
                                const vk::MemoryPropertyFlags memoryProperties)
    {
        auto       buffer = m_device->CreateBufferUnique({{}, size, usage, sharingMode});
        const auto memoryAllocation =
            m_memoryAllocator.allocateMemoryForBuffer(*buffer, {{}, vma::MemoryUsage::eGpuOnly, memoryProperties});
        m_memoryAllocator.bindBufferMemory(memoryAllocation, *buffer);
        return std::make_unique<VulkanBuffer>(shared_from_this(), std::move(buffer), memoryAllocation, size);
    }

    [[nodiscard]] std::unique_ptr<VulkanBuffer> VulkanContext::CreateCpuToGpuTransferBuffer(const void*    data,
                                                                                            const uint32_t size)
    {
        auto buffer = m_device->CreateBufferUnique(
            {{}, size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive});
        const auto memoryAllocation = m_memoryAllocator.allocateMemoryForBuffer(
            *buffer,
            {{},
             vma::MemoryUsage::eCpuToGpu,
             vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent});
        m_memoryAllocator.bindBufferMemory(memoryAllocation, *buffer);

        const auto memory = m_memoryAllocator.mapMemory(memoryAllocation);
        memcpy(memory, data, size);
        m_memoryAllocator.unmapMemory(memoryAllocation);

        return std::make_unique<VulkanBuffer>(shared_from_this(), std::move(buffer), memoryAllocation, size);
    }

    vk::UniqueShaderModule VulkanContext::CreateShaderModule(const std::vector<uint8_t>& bytes) const
    {
        return m_device->CreateShaderModuleUnique({{}, bytes.size(), reinterpret_cast<const uint32_t*>(bytes.data())});
    }

    vk::UniqueDescriptorSetLayout
    VulkanContext::CreateDescriptorSetLayout(const vk::DescriptorSetLayoutBinding& binding)
    {
        auto bindings = std::vector {binding};
        return m_device->CreateDescriptorSetLayoutUnique({{}, bindings});
    }

    vk::UniqueDescriptorPool VulkanContext::CreateDescriptorPool(const uint32_t           maxSets,
                                                                 const vk::DescriptorType type) const
    {
        auto poolSizes = std::vector {vk::DescriptorPoolSize {type, maxSets}};
        return m_device->CreateDescriptorPoolUnique(
            {vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxSets, poolSizes});
    }

    void VulkanContext::UpdateDescriptorSets(const std::vector<vk::WriteDescriptorSet>& writes,
                                             const std::vector<vk::CopyDescriptorSet>&  copies) const
    {
        m_device->UpdateDescriptorSets(writes, copies);
    }

    std::vector<vk::UniqueDescriptorSet> VulkanContext::CreateDescriptorSets(vk::DescriptorPool&      descriptorPool,
                                                                             vk::DescriptorSetLayout& layout,
                                                                             const uint32_t           count) const
    {
        std::vector<vk::DescriptorSetLayout> vector;
        vector.reserve(count);
        for (auto i = 0u; i < count; ++i)
            vector.push_back(layout);
        return m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo {descriptorPool, vector});
    }

    [[nodiscard]] util::StagingResource<vk::Semaphore> VulkanContext::CreateSemaphore(const uint32_t stages) const
    {
        std::vector<vk::UniqueSemaphore> semaphores;
        semaphores.reserve(stages);
        for (auto i = 0u; i < stages; ++i)
            semaphores.push_back(m_device->CreateSemaphoreUnique({}));
        return util::StagingResource<vk::Semaphore>(std::move(semaphores));
    }

    [[nodiscard]] util::StagingResource<vk::Fence> VulkanContext::CreateFence(const uint32_t stages,
                                                                              const bool     signaled) const
    {
        vk::FenceCreateInfo createInfo;
        if (signaled)
            createInfo.flags |= vk::FenceCreateFlagBits::eSignaled;
        std::vector<vk::UniqueFence> fences;
        fences.reserve(stages);
        for (auto i = 0u; i < stages; ++i)
            fences.push_back(m_device->CreateFenceUnique(createInfo));
        return util::StagingResource<vk::Fence>(std::move(fences));
    }

    void VulkanContext::Wait(const vk::Fence& fence) const
    {
        const auto result = m_device->WaitForFences(1, &fence, true, UINT64_MAX);
        if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to Wait for fence.");
    }

    void VulkanContext::Reset(const vk::Fence& fence) const
    {
        const auto result = m_device->resetFences(1, &fence);
        if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to reset fence.");
    }

    vk::ResultValue<uint32_t> VulkanContext::AcquireNextImage(const vk::SwapchainKHR& swapChain,
                                                              const vk::Semaphore&    semaphore) const
    {
        return m_device->AcquireNextImageKHR(swapChain, UINT64_MAX, semaphore, nullptr);
    }

    void VulkanContext::Submit(const vk::CommandBuffer& commandBuffer,
                               const vk::Semaphore&     WaitSemaphore,
                               const vk::Semaphore&     signalSemaphore,
                               const vk::Fence&         fence) const
    {
        vk::PipelineStageFlags WaitFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        vk::SubmitInfo         SubmitInfo {1, &WaitSemaphore, &WaitFlags, 1, &commandBuffer, 1, &signalSemaphore};
        const auto             result = m_graphicsQueue.Submit(1, &SubmitInfo, fence);
        if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to Submit work.");
    }

    [[nodiscard]] vk::Result VulkanContext::Present(const vk::Semaphore&    WaitSemaphore,
                                                    const vk::SwapchainKHR& swapChain,
                                                    const uint32_t          imageIndex) const
    {
        vk::PresentInfoKHR presentInfo {1, &WaitSemaphore, 1, &swapChain, &imageIndex, nullptr};
        return m_presentQueue.presentKHR(&presentInfo);
    }
} // namespace Voxium::Platform::Desktop::Vulkan
