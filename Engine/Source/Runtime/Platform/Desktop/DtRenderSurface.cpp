#include "DtRenderSurface.h"

#include <atomic>

namespace Voxium::Platform::Desktop
{
    void InputContext::ConsumeKeyboardEvents(const Voxium::Platform::Input::KeyboardEventConsumer consumer)
    {
        while (!keyboardEvents_.empty())
        {
            auto& evt = keyboardEvents_.front();
            consumer(evt.code, evt.action);
            keyboardEvents_.pop_front();
        }
    }

    void InputContext::ConsumeMouseEvents(const Voxium::Platform::Input::MouseEventConsumer consumer)
    {
        while (!mouseEvents_.empty())
        {
            auto& evt = mouseEvents_.front();
            consumer(evt.button, evt.action);
            mouseEvents_.pop_front();
        }
    }

    void InputContext::ConsumeScrollEvents(const Voxium::Platform::Input::ScrollEventConsumer consumer)
    {
        while (!scrollEvents_.empty())
        {
            auto& evt = scrollEvents_.front();
            consumer(evt.xOffset, evt.yOffset);
            scrollEvents_.pop_front();
        }
    }

    void InputContext::ConsumeCursorEvents(const Voxium::Platform::Input::CursorEventConsumer consumer)
    {
        while (!cursorEvents_.empty())
        {
            auto& evt = cursorEvents_.front();
            consumer(evt.x, evt.y, evt.action);
            cursorEvents_.pop_front();
        }
    }

    void InputContext::SetCursorMode(const Voxium::Platform::Input::CursorMode mode)
    {
        cursorMode_ = mode;
        glfwSetInputMode(surface_->window_,
                         GLFW_CURSOR,
                         mode == Voxium::Platform::Input::CursorMode::HIDDEN ? GLFW_CURSOR_HIDDEN :
                         mode == Voxium::Platform::Input::CursorMode::RAW    ? GLFW_CURSOR_DISABLED :
                                                                               GLFW_CURSOR_NORMAL);
    }

    void InputContext::CenterCursor()
    {
        glfwSetCursorPos(surface_->window_, surface_->width_ / 2, surface_->height_ / 2);
    }

    RenderSurface::RenderSurface(const GLFWContext&               glfwContext,
                                 std::shared_ptr<RenderSurface>&& parent,
                                 const RenderContextFactory&      contextFactory,
                                 const uint32_t                   width,
                                 const uint32_t                   height,
                                 const std::string&               title,
                                 const bool                       fullscreen) :
        glfwContext_(glfwContext), parent_(std::move(parent)), inputContext_(InputContext(this)), width_(width),
        height_(height), title_(title), fullscreen_(fullscreen)
    {
        GLFWmonitor* monitor = nullptr;
        if (fullscreen)
            monitor = glfwGetPrimaryMonitor();

        GLFWwindow* share = nullptr;
        if (parent_ != nullptr)
            share = parent_->window_;

        window_ = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        glfwSetWindowUserPointer(window_, this);

        // Create the context once we have a window
        context_ = contextFactory(*this, parent_.get());

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glfwSetFrameBufferSizeCallback(window_, [](GLFWwindow* win, const int width, const int height) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            // [[maybe_unused]] std::scoped_lock<std::mutex> lock(window->renderLock_);

            window->width_       = width;
            window->height_      = height;
            window->visible_     = width != 0 && height != 0;
            window->justResized_ = true;
            window->resized_     = true;
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* win, int, const int scancode, const int action, int) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            window->inputContext_.keyboardEvents_.push_back(
                {static_cast<uint32_t>(scancode), static_cast<Voxium::Platform::Input::KeyboardAction>(action)});
        });

        glfwSetCharCallback(window_, [](GLFWwindow* win, const unsigned int c) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            window->inputContext_.keyboardEvents_.push_back({c, Voxium::Platform::Input::KeyboardAction::CHARACTER});
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* win, const int button, const int action, int) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            window->inputContext_.mouseEvents_.push_back(
                {static_cast<uint32_t>(button), static_cast<Voxium::Platform::Input::MouseAction>(action)});
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* win, const double xOff, const double yOff) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            window->inputContext_.scrollEvents_.push_back({xOff, yOff});
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* win, const double x, const double y) {
            auto* window = static_cast<RenderSurface*>(glfwGetWindowUserPointer(win));

            window->inputContext_.cursorEvents_.push_back(
                {static_cast<uint32_t>(x), static_cast<uint32_t>(y), Voxium::Platform::Input::CursorAction::MOVE});
        });

        // TODO: Support IME and alternative input methods
        // glfwSetCharCallback(
        // 	window_,
        // 	[](GLFWwindow* win, unsigned int character)
        // 	{
        // 	}
        // );
    }

    bool RenderSurface::IsActive() const { return !close_ && !glfwWindowShouldClose(window_); }

    Voxium::Platform::Render::IRenderContext* RenderSurface::UpdateFirst()
    {
        if (width_ == 0 || height_ == 0)
            glfwWaitEvents();
        else
            glfwPollEvents();

        if (width_ == 0 || height_ == 0)
            return nullptr;

        context_->UpdateFirst();

        return context_.get();
    }

    void RenderSurface::UpdateLast()
    {
        context_->UpdateLast();

        justResized_ = false;
    }

    void RenderSurface::Terminate(bool force)
    {
        // Manually Terminate the context before the window closes
        context_ = nullptr;

        glfwDestroyWindow(window_);
    }

    void RenderSurface::SetWidth(const uint32_t width)
    {
        width_ = width;
        glfwSetWindowSize(window_, width_, height_);
    }

    void RenderSurface::SetHeight(const uint32_t height)
    {
        height_ = height;
        glfwSetWindowSize(window_, width_, height_);
    }

    void RenderSurface::SetTitle(const std::string& title)
    {
        title_ = title;
        glfwSetWindowTitle(window_, title_.c_str());
    }

    void RenderSurface::SetFullscreen(const bool fullscreen)
    {
        // TODO: Implement fullscreen support
    }

    vk::UniqueSurfaceKHR RenderSurface::CreateVulkanSurface(const vk::Instance& instance) const
    {
        VkSurfaceKHR tmpSurface;
        if (glfwCreateWindowSurface(instance, window_, nullptr, &tmpSurface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface.");

        return vk::UniqueSurfaceKHR(reinterpret_cast<vk::SurfaceKHR&>(tmpSurface),
                                    vk::ObjectDestroy<vk::Instance, vk::DispatchLoaderDynamic>(instance));
    }

    std::vector<const char*> RenderSurface::GetSurfaceExtensions()
    {
        uint32_t    glfwExtensionCount;
        auto* const glfwExtensionPtr = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        return std::vector<const char*>(glfwExtensionPtr, glfwExtensionPtr + glfwExtensionCount);
    }
} // namespace Voxium::Platform::Desktop
