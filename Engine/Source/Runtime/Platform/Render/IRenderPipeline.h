#pragma once

#include <memory>

#include "IResource.h"

namespace Voxium::Platform::Render
{
    class IRenderPipeline : public IResource, public std::enable_shared_from_this<IRenderPipeline>
    {
    public:
        IRenderPipeline()                                            = default;
        ~IRenderPipeline() override                                  = default;
        IRenderPipeline(const IRenderPipeline& other)                = delete;
        IRenderPipeline(IRenderPipeline&& other) noexcept            = delete;
        IRenderPipeline& operator=(const IRenderPipeline& other)     = delete;
        IRenderPipeline& operator=(IRenderPipeline&& other) noexcept = delete;
    };
} // namespace Voxium::Platform::Render
