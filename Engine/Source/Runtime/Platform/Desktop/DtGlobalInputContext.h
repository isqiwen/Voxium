#pragma once

#include "Platform/Input/IGlobalInputContext.h"

namespace Voxium::Platform::Desktop
{
    class GlobalInputContext final : public Voxium::Platform::Input::IGlobalInputContext
    {
    public:
        [[nodiscard]] std::vector<std::shared_ptr<Voxium::Platform::Input::IController>> GetControllers() override;

        void Update() override;

    private:
        void Initialize();

        std::vector<std::shared_ptr<Voxium::Platform::Input::IController>> controllers_;
        bool                                                               initialized_ = false;
    };
} // namespace Voxium::Platform::Desktop
