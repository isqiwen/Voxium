#include "DtController.h"

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Voxium::Platform::Desktop
{
    bool Controller::IsConnected() const { return glfwJoystickPresent(id_); }

    std::vector<uint8_t> Controller::GetButtonStates() const
    {
        if (!glfwJoystickPresent(id_))
            return std::vector<uint8_t>();

        int         count;
        const auto* buttonStates = glfwGetJoystickButtons(id_, &count);

        std::vector<uint8_t> vector;
        vector.reserve(count);
        for (auto i = 0; i < count; ++i)
            vector.push_back(buttonStates[i]);

        return vector;
    }

    std::vector<float> Controller::GetJoysticks() const
    {
        if (!glfwJoystickPresent(id_))
            return std::vector<float>();

        int         count;
        const auto* joystickStates = glfwGetJoystickAxes(id_, &count);
        return std::vector(joystickStates, joystickStates + count);
    }

    std::vector<std::bitset<4>> Controller::GetHatStates() const
    {
        if (!glfwJoystickPresent(id_))
            return std::vector<std::bitset<4>>();

        int         count;
        const auto* hatStates = glfwGetJoystickHats(id_, &count);

        std::vector<std::bitset<4>> vector;
        vector.reserve(count);
        for (auto i = 0; i < count; ++i)
            vector.emplace_back(hatStates[i]);

        return vector;
    }
} // namespace Voxium::Platform::Desktop
