#pragma once

#include <memory>
#include <vector>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IVertexBuffer : public IResource, public std::enable_shared_from_this<IVertexBuffer>
    {
    public:
        IVertexBuffer()                                          = default;
        ~IVertexBuffer() override                                = default;
        IVertexBuffer(const IVertexBuffer& other)                = delete;
        IVertexBuffer(IVertexBuffer&& other) noexcept            = delete;
        IVertexBuffer& operator=(const IVertexBuffer& other)     = delete;
        IVertexBuffer& operator=(IVertexBuffer&& other) noexcept = delete;

        [[nodiscard]] virtual uint32_t GetVertexSize() = 0;

        virtual void Write(const std::vector<uint8_t>& data) = 0;
    };
} // namespace Voxium::Platform::Render
