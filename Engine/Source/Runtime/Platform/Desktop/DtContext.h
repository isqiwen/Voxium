#pragma once

namespace Voxium::Platform::Desktop
{
    class GLFWContext final
    {
    public:
        GLFWContext(bool noApi);
        ~GLFWContext();
        GLFWContext(const GLFWContext& other)                = delete;
        GLFWContext(GLFWContext&& other) noexcept            = delete;
        GLFWContext& operator=(const GLFWContext& other)     = delete;
        GLFWContext& operator=(GLFWContext&& other) noexcept = delete;

    private:
        friend class RenderSurface;
    };
} // namespace Voxium::Platform::Desktop
