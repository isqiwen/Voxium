#include "DtContext.h"

#include <GLFW.h>

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
