#include "Platform.h"

#include "Platform/Desktop/Vulkan/VkPlatform.h"

namespace Voxium::Platform
{
    Platform& Platform::Instance()
    {
        static Voxium::Desktop::Vulkan::Platform instance;
        return instance;
    }
} // namespace Voxium::Platform
