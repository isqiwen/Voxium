#include "DtContext.h"

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Voxium::Platform::Desktop
{
    GLFWContext::GLFWContext(const bool noApi)
    {
        glfwInit();
        if (noApi)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    GLFWContext::~GLFWContext() { glfwTerminate(); }
} // namespace Voxium::Platform::Desktop
