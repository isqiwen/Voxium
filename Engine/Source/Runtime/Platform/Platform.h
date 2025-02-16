#pragma once

#include <memory>

#include "Platform/Input/IGlobalInputContext.h"
#include "Platform/Render/IRenderSurface.h"

namespace Voxium::Platform
{
    struct RenderSurfaceCreationHints
    {
        uint32_t width_, height_;
        char*    title_;
        bool     fullscreen_;
        bool     fallbackOnIncompatibleParent_;
    };

    class IRenderManager
    {
    public:
        IRenderManager()                                           = default;
        virtual ~IRenderManager()                                  = default;
        IRenderManager(const IRenderManager& other)                = delete;
        IRenderManager(IRenderManager&& other) noexcept            = delete;
        IRenderManager& operator=(const IRenderManager& other)     = delete;
        IRenderManager& operator=(IRenderManager&& other) noexcept = delete;

        [[nodiscard]] virtual bool SupportsMultipleRenderSurfaces() const = 0;

        [[nodiscard]] virtual std::shared_ptr<Voxium::Platform::Render::IRenderSurface>
        RequestRenderSurface(RenderSurfaceCreationHints hints) = 0;
    };

    class Platform
    {
    public:
        static Platform& Instance();
        Platform()                                      = default;
        virtual ~Platform()                             = default;
        Platform(const Platform& other)                = delete;
        Platform(Platform&& other) noexcept            = delete;
        Platform& operator=(const Platform& other)     = delete;
        Platform& operator=(Platform&& other) noexcept = delete;

        [[nodiscard]] virtual Voxium::Platform::Input::IGlobalInputContext&  GetGlobalInputContext() = 0;
        [[nodiscard]] virtual IRenderManager&                                GetRenderManager()      = 0;
    };
} // namespace Voxium::Platform
