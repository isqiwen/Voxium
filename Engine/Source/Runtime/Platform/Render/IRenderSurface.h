#pragma once

#include <string>
#include <memory>

#include "Platform/Input/ISurfaceInputContext.h"

#include "IRenderTarget.h"

namespace Voxium::Platform::Render
{
    class IRenderContext;

    class IRenderSurface : public IRenderTarget, public std::enable_shared_from_this<IRenderSurface>
    {
    public:
        IRenderSurface()                                           = default;
        ~IRenderSurface() override                                 = default;
        IRenderSurface(const IRenderSurface& other)                = delete;
        IRenderSurface(IRenderSurface&& other) noexcept            = delete;
        IRenderSurface& operator=(const IRenderSurface& other)     = delete;
        IRenderSurface& operator=(IRenderSurface&& other) noexcept = delete;

        [[nodiscard]] virtual Voxium::Platform::Input::ISurfaceInputContext& GetInputContext() = 0;

        [[nodiscard]] virtual uint32_t    GetWidth() const     = 0;
        [[nodiscard]] virtual uint32_t    GetHeight() const    = 0;
        [[nodiscard]] virtual std::string GetTitle() const     = 0;
        [[nodiscard]] virtual bool        IsFullscreen() const = 0;
        [[nodiscard]] virtual bool        IsVisible() const    = 0;
        [[nodiscard]] virtual bool        IsResized() const    = 0;

        virtual void SetWidth(uint32_t width) {}
        virtual void SetHeight(uint32_t height) {}
        virtual void SetTitle(const std::string& title) {}
        virtual void SetFullscreen(bool fullscreen) {}

        [[nodiscard]] virtual bool IsActive() const      = 0;
        virtual IRenderContext*    UpdateFirst()         = 0;
        virtual void               UpdateLast()          = 0;
        virtual void               Terminate(bool force) = 0;
    };
} // namespace Voxium::Platform::Render
