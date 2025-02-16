#pragma once

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IFrameBuffer;

    class IRenderTarget : public IResource
    {
    public:
        IRenderTarget()                                          = default;
        ~IRenderTarget() override                                = default;
        IRenderTarget(const IRenderTarget& other)                = delete;
        IRenderTarget(IRenderTarget&& other) noexcept            = delete;
        IRenderTarget& operator=(const IRenderTarget& other)     = delete;
        IRenderTarget& operator=(IRenderTarget&& other) noexcept = delete;

        [[nodiscard]] virtual IFrameBuffer& GetFrameBuffer() = 0;
    };
} // namespace Voxium::Platform::Render
