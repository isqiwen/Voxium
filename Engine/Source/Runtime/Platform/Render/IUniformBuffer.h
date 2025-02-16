#pragma once

#include <memory>
#include <vector>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IUniformBuffer : public IResource, public std::enable_shared_from_this<IUniformBuffer>
    {
    public:
        IUniformBuffer()                                           = default;
        ~IUniformBuffer() override                                 = default;
        IUniformBuffer(const IUniformBuffer& other)                = delete;
        IUniformBuffer(IUniformBuffer&& other) noexcept            = delete;
        IUniformBuffer& operator=(const IUniformBuffer& other)     = delete;
        IUniformBuffer& operator=(IUniformBuffer&& other) noexcept = delete;

        virtual void Write(const std::vector<uint8_t>& data) = 0;
    };
} // namespace Voxium::Platform::Render
