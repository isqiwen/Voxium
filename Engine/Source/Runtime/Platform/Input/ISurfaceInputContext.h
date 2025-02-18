#pragma once

#include <functional>

namespace Voxium::Platform::Input
{
    enum class KeyboardAction : uint8_t
    {
        RELEASE,
        PRESS,
        REPEAT,
        CHARACTER
    };

    enum class MouseAction : uint8_t
    {
        RELEASE,
        PRESS
    };

    enum class CursorAction : uint8_t
    {
        MOVE,
    };

    enum CursorMode : uint8_t
    {
        NORMAL,
        HIDDEN,
        RAW
    };

    using KeyboardEventConsumer = std::function<void(uint32_t code, KeyboardAction action)>;
    using MouseEventConsumer    = std::function<void(uint32_t button, MouseAction action)>;
    using ScrollEventConsumer   = std::function<void(double xOffset, double yOffset)>;
    using CursorEventConsumer   = std::function<void(uint32_t x, uint32_t y, CursorAction action)>;

    class ISurfaceInputContext
    {
    public:
        ISurfaceInputContext()                                                 = default;
        virtual ~ISurfaceInputContext()                                        = default;
        ISurfaceInputContext(const ISurfaceInputContext& other)                = delete;
        ISurfaceInputContext(ISurfaceInputContext&& other) noexcept            = delete;
        ISurfaceInputContext& operator=(const ISurfaceInputContext& other)     = delete;
        ISurfaceInputContext& operator=(ISurfaceInputContext&& other) noexcept = delete;

        virtual void ConsumeKeyboardEvents(KeyboardEventConsumer consumer) = 0;
        virtual void ConsumeMouseEvents(MouseEventConsumer consumer)       = 0;
        virtual void ConsumeScrollEvents(ScrollEventConsumer consumer)     = 0;
        virtual void ConsumeCursorEvents(CursorEventConsumer consumer)     = 0;

        [[nodiscard]] virtual CursorMode GetCursorMode() const          = 0;
        virtual void                     SetCursorMode(CursorMode mode) = 0;
        virtual void                     CenterCursor()                 = 0;
    };
} // namespace Voxium::Platform::Input
