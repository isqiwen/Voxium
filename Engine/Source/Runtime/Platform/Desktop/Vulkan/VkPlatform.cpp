#include "VkPlatform.h"

#include "Platform/Desktop/DtRenderSurface.h"

#include "VkRenderContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    std::shared_ptr<Voxium::Platform::Render::RenderSurface> RenderManager::RequestRenderSurface(RenderSurfaceCreationHints hints)
    {
        return std::make_shared<IRenderSurface>(
            glfwContext_,
            nullptr,
            [hints](RenderSurface& surface, RenderSurface* parent) {
                std::shared_ptr<VulkanContext> context;
                if (parent != nullptr)
                    context = dynamic_cast<const RenderContext&>(parent->GetContext()).context_;
                else
                    context = std::make_shared<VulkanContext>(RenderSurface::GetSurfaceExtensions());

                auto vkSurface = surface.CreateVulkanSurface(context->getInstance());
                if (!context->InitializeOrValidateDeviceCompatibility(*vkSurface))
                {
                    if (parent == nullptr)
                        throw std::runtime_error("Could not create Vulkan context.");

                    if (!hints.fallbackOnIncompatibleParent)
                        throw std::runtime_error("Incompatible parent surface. Cannot share Vulkan context.");

                    context = std::make_shared<VulkanContext>(RenderSurface::GetSurfaceExtensions());
                    if (!context->InitializeOrValidateDeviceCompatibility(*vkSurface))
                        throw std::runtime_error(
                            "Incompatible parent surface. Could not create fallback Vulkan context.");
                }

                return std::make_unique<RenderContext>(surface, std::move(context), std::move(vkSurface));
            },
            hints.width,
            hints.height,
            hints.title,
            hints.fullscreen);
    }
} // namespace Voxium::Platform::Desktop::Vulkan
