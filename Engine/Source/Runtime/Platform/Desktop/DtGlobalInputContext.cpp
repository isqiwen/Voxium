#include "DtGlobalInputContext.h"

#include <algorithm>

#include <GLFW.h>

#include "DtController.h"

namespace Voxium::Platform::Desktop
{
    std::vector<std::shared_ptr<Voxium::Platform::Input::IController>> GlobalInputContext::GetControllers()
    {
        Initialize();
        return controllers_;
    }

    void GlobalInputContext::Update()
    {
        Initialize();

        // Remove disconnected controllers
        controllers_.erase(std::remove_if(controllers_.begin(),
                                           controllers_.end(),
                                           [](const std::shared_ptr<Voxium::Platform::Input::IController>& c) {
                                               return !std::static_pointer_cast<Controller>(c)->IsConnected();
                                           }),
                            controllers_.end());
    }

    void GlobalInputContext::Initialize()
    {
        if (!initialized_)
        {
            initialized_ = true;

            for (auto i = 0u; i < 16; ++i)
            {
                if (glfwJoystickPresent(static_cast<int>(i)))
                {
                    auto guid = std::string(glfwGetJoystickGUID(i));
                    controllers_.push_back(std::make_shared<Controller>(i, guid));
                }
            }
        }
    }
} // namespace Voxium::Platform::Desktop
