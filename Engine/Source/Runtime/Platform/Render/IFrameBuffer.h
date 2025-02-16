#pragma once

#include "IFrameBufferFormat.h"
#include "IRenderTarget.h"
#include "ITexture.h"

namespace Voxium::Platform::Render
{
    class IFrameBuffer : public IRenderTarget, public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        IFrameBuffer()                                         = default;
        ~IFrameBuffer() override                               = default;
        IFrameBuffer(const IFrameBuffer& other)                = delete;
        IFrameBuffer(IFrameBuffer&& other) noexcept            = delete;
        IFrameBuffer& operator=(const IFrameBuffer& other)     = delete;
        IFrameBuffer& operator=(IFrameBuffer&& other) noexcept = delete;

        [[nodiscard]] virtual const IFrameBufferFormat& GetFormat() const = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const  = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        [[nodiscard]] virtual std::shared_ptr<ITexture> GetTexture(uint32_t attachment) = 0;

        [[nodiscard]] IFrameBuffer& GetFrameBuffer() override { return *this; }
    };
} // namespace Voxium::Platform::Render
