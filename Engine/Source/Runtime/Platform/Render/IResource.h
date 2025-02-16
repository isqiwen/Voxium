#pragma once

namespace Voxium::Platform::Render
{
    class IResource
    {
    public:
        IResource()                                      = default;
        virtual ~IResource()                             = default;
        IResource(const IResource& other)                = delete;
        IResource(IResource&& other) noexcept            = delete;
        IResource& operator=(const IResource& other)     = delete;
        IResource& operator=(IResource&& other) noexcept = delete;
    };
} // namespace Voxium::Platform::Render
