#pragma once

#include <deque>
#include <mutex>
#include <thread>

#include <GLFW.h>
#include <vulkan.h>

#include "Platform/Render/IRenderSurface.h"

#include "DtContext.h"
#include "IDtRenderContext.h"

namespace Voxium::Platform::Desktop
{
    struct KeyboardEvent
    {
        const uint32_t                                code;
        const Voxium::Platform::Input::KeyboardAction action;
    };

    struct MouseEvent
    {
        const uint32_t                             button;
        const Voxium::Platform::Input::MouseAction action;
    };

    struct ScrollEvent
    {
        const double xOffset;
        const double yOffset;
    };

    struct CursorEvent
    {
        const uint32_t                              x, y;
        const Voxium::Platform::Input::CursorAction action;
    };

    class InputContext final : public Voxium::Platform::Input::ISurfaceInputContext
    {
    public:
        explicit InputContext(Voxium::Platform::Voxium::Platform::Render::IRenderSurface* surface) : surface_(surface) {}

        void ConsumeKeyboardEvents(Voxium::Platform::Input::KeyboardEventConsumer consumer) override;
        void ConsumeMouseEvents(Voxium::Platform::Input::MouseEventConsumer consumer) override;
        void ConsumeScrollEvents(Voxium::Platform::Input::ScrollEventConsumer consumer) override;
        void ConsumeCursorEvents(Voxium::Platform::Input::CursorEventConsumer consumer) override;

        [[nodiscard]] Voxium::Platform::Input::CursorMode GetCursorMode() const override { return cursorMode_; }
        void SetCursorMode(Voxium::Platform::Input::CursorMode mode) override;
        void CenterCursor() override;

    private:
        Voxium::Platform::Voxium::Platform::Render::IRenderSurface* surface_;
        std::deque<KeyboardEvent>                 keyboardEvents_;
        std::deque<MouseEvent>                    mouseEvents_;
        std::deque<ScrollEvent>                   scrollEvents_;
        std::deque<CursorEvent>                   cursorEvents_;
        Voxium::Platform::Input::CursorMode       cursorMode_ = Voxium::Platform::Input::NORMAL;

        friend class Voxium::Platform::Voxium::Platform::Render::IRenderSurface;
    };

    class RenderSurface final : public Voxium::Platform::Voxium::Platform::Render::IRenderSurface
    {
    public:
        RenderSurface(const GLFWContext&               glfwContext,
                      std::shared_ptr<RenderSurface>&& parent,
                      const RenderContextFactory&      contextFactory,
                      uint32_t                         width,
                      uint32_t                         height,
                      const std::string&               title,
                      bool                             fullscreen);

        [[nodiscard]] Voxium::Platform::Input::ISurfaceInputContext& GetInputContext() override { return inputContext_; }

        [[nodiscard]] uint32_t    GetWidth() const override { return width_; }
        [[nodiscard]] uint32_t    GetHeight() const override { return height_; }
        [[nodiscard]] std::string GetTitle() const override { return title_; }
        [[nodiscard]] bool        IsFullscreen() const override { return fullscreen_; }
        [[nodiscard]] bool        IsVisible() const override { return visible_; }
        [[nodiscard]] bool        IsResized() const override { return resized_; }
        [[nodiscard]] bool        WasJustResized() const { return justResized_; }
        void                      ResetResized() { resized_ = false; }

        void SetWidth(uint32_t width) override;
        void SetHeight(uint32_t height) override;
        void SetTitle(const std::string& title) override;
        void SetFullscreen(bool fullscreen) override;

        bool                                      IsActive() const override;
        Voxium::Platform::Render::IRenderContext* UpdateFirst() override;
        void                                      UpdateLast() override;
        void                                      Terminate(bool force) override;

        const RenderContext&               GetContext() const { return *context_; }
        vk::UniqueSurfaceKHR               CreateVulkanSurface(const vk::Instance& instance) const;
        static std::vector<const char*>    GetSurfaceExtensions();
        [[nodiscard]] Voxium::Platform::Render::FrameBuffer& GetFrameBuffer() override { return context_->GetFrameBuffer(); }

    private:
        const GLFWContext&                   glfwContext_;
        const std::shared_ptr<RenderSurface> parent_;
        InputContext                         inputContext_;
        std::unique_ptr<IRenderContext>      context_;
        uint32_t                             width_, height_;
        std::string                          title_;
        bool                                 fullscreen_, visible_, justResized_, resized_ = false;
        bool                                 close_ = false;
        GLFWwindow*                          window_;
        std::mutex                           renderLock_;

        friend class GLFWContext;
        friend class InputContext;
    };
} // namespace Voxium::Platform::Desktop
