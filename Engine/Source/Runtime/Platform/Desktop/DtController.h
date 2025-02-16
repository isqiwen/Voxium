#pragma once

#include "Platform/Input/IController.h"

namespace Voxium::Platform::Desktop
{
    class Controller final : public Voxium::Platform::Input::IController
    {
    public:
        Controller(uint32_t id, const std::string& guid) : id_(id), guid_(guid) {}

        [[nodiscard]] bool IsConnected() const override;

        [[nodiscard]] std::string GetGUID() const override { return guid_; }

        [[nodiscard]] std::vector<uint8_t>        GetButtonStates() const override;
        [[nodiscard]] std::vector<float>          GetJoysticks() const override;
        [[nodiscard]] std::vector<std::bitset<4>> GetHatStates() const override;

    private:
        uint32_t    id_;
        std::string guid_;
    };
} // namespace Voxium::Platform::Desktop
