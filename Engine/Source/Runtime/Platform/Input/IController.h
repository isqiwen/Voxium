#pragma once

#include <bitset>
#include <memory>
#include <vector>

namespace Voxium::Platform::Input
{
    class IController : public std::enable_shared_from_this<IController>
    {
    public:
        IController()                                        = default;
        virtual ~IController()                               = default;
        IController(const IController& other)                = delete;
        IController(IController&& other) noexcept            = delete;
        IController& operator=(const IController& other)     = delete;
        IController& operator=(IController&& other) noexcept = delete;

        [[nodiscard]] virtual bool IsConnected() const = 0;

        [[nodiscard]] virtual std::string GetGUID() const = 0;

        [[nodiscard]] virtual std::vector<uint8_t>        GetButtonStates() const = 0;
        [[nodiscard]] virtual std::vector<float>          GetJoysticks() const    = 0;
        [[nodiscard]] virtual std::vector<std::bitset<4>> GetHatStates() const    = 0;
    };
} // namespace Voxium::Platform::Input
