#pragma once

#include <memory>

#include "IResource.h"
#include "IUniformBuffer.h"

namespace Voxium::Platform::Render
{
    class IUniformBinding : public IResource, public std::enable_shared_from_this<IUniformBinding>
    {
    public:
        IUniformBinding()                                            = default;
        ~IUniformBinding() override                                  = default;
        IUniformBinding(const IUniformBinding& other)                = delete;
        IUniformBinding(IUniformBinding&& other) noexcept            = delete;
        IUniformBinding& operator=(const IUniformBinding& other)     = delete;
        IUniformBinding& operator=(IUniformBinding&& other) noexcept = delete;

        virtual void Update(std::shared_ptr<IUniformBuffer> buffer) = 0;
    };
} // namespace Voxium::Platform::Render
