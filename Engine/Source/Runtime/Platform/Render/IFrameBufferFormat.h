#pragma once

#include <memory>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IFrameBufferFormat : public IResource, public std::enable_shared_from_this<IFrameBufferFormat>
    {
    public:
        IFrameBufferFormat()                                               = default;
        ~IFrameBufferFormat() override                                     = default;
        IFrameBufferFormat(const IFrameBufferFormat& other)                = delete;
        IFrameBufferFormat(IFrameBufferFormat&& other) noexcept            = delete;
        IFrameBufferFormat& operator=(const IFrameBufferFormat& other)     = delete;
        IFrameBufferFormat& operator=(IFrameBufferFormat&& other) noexcept = delete;

        [[nodiscard]] virtual uint32_t GetAttachmentCount() const = 0;
    };
} // namespace Voxium::Platform::Render
