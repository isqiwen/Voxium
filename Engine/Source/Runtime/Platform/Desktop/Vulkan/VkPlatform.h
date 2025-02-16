#pragma once

#include "Platform/Platform.h"
#include "Platform/Desktop/DtContext.h"
#include "Platform/Desktop/DtGlobalInputContext.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class RenderManager final : public Voxium::Platform::IRenderManager
    {
    public:
        RenderManager() : glfwContext_(true) {}

        [[nodiscard]] bool SupportsMultipleRenderSurfaces() const override { return true; }

        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::IRenderSurface>
        RequestRenderSurface(RenderSurfaceCreationHints hints) override;

    private:
        GLFWContext glfwContext_;
    };

    class Platform final : public Voxium::Platform::Platform
    {
    public:
        Platform() = default;

        [[nodiscard]] Voxium::Platform::Input::GlobalInputContext& GetGlobalInputContext() override
        {
            return globalInputContext_;

        }
        [[nodiscard]] platform::RenderManager& GetRenderManager() override { return renderManager_; }

    private:
        RenderManager      renderManager_;
        GlobalInputContext globalInputContext_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
