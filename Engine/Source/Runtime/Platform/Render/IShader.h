#pragma once

#include <memory>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IShader : public IResource, public std::enable_shared_from_this<IShader>
    {
    public:
        IShader()                                    = default;
        ~IShader() override                          = default;
        IShader(const IShader& other)                = delete;
        IShader(IShader&& other) noexcept            = delete;
        IShader& operator=(const IShader& other)     = delete;
        IShader& operator=(IShader&& other) noexcept = delete;
    };
} // namespace Voxium::Platform::Render
