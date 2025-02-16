#pragma once

#include <memory>

#include "IController.h"

namespace Voxium::Platform::Input
{
    class IGlobalInputContext
    {
    public:
        IGlobalInputContext()                                                = default;
        virtual ~IGlobalInputContext()                                       = default;
        IGlobalInputContext(const IGlobalInputContext& other)                = delete;
        IGlobalInputContext(IGlobalInputContext&& other) noexcept            = delete;
        IGlobalInputContext& operator=(const IGlobalInputContext& other)     = delete;
        IGlobalInputContext& operator=(IGlobalInputContext&& other) noexcept = delete;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IController>> GetControllers() = 0;

        virtual void Update() = 0;
    };
} // namespace Voxium::Platform::Input
