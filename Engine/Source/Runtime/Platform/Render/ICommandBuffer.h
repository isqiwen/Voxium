#pragma once

#include <memory>

#include "Platform/Utils/VecMath.h"

#include "IFrameBufferFormat.h"
#include "IRenderPipeline.h"
#include "IRenderTarget.h"
#include "IResource.h"
#include "ITextureBinding.h"
#include "IUniformBinding.h"
#include "IVertexBuffer.h"

namespace Voxium::Platform::Render
{
    class ICommandBuffer : public IResource, public std::enable_shared_from_this<ICommandBuffer>
    {
    public:
        ICommandBuffer()                                           = default;
        ~ICommandBuffer() override                                 = default;
        ICommandBuffer(const ICommandBuffer& other)                = delete;
        ICommandBuffer(ICommandBuffer&& other) noexcept            = delete;
        ICommandBuffer& operator=(const ICommandBuffer& other)     = delete;
        ICommandBuffer& operator=(ICommandBuffer&& other) noexcept = delete;

        virtual void BeginRecording(const std::shared_ptr<IFrameBufferFormat>& format)                              = 0;
        virtual void SetViewportAndScissor(std::shared_ptr<IRenderTarget> renderTarget)                             = 0;
        virtual void SetViewport(Voxium::Platform::Utils::Extents2D extents)                                        = 0;
        virtual void SetScissor(Voxium::Platform::Utils::Extents2D extents)                                         = 0;
        virtual void BindUniform(std::shared_ptr<IRenderPipeline> pipeline,
                                 std::shared_ptr<IUniformBinding> uniformBinding,
                                 uint32_t                         binding)                                            = 0;
        virtual void BindTexture(std::shared_ptr<IRenderPipeline> pipeline, std::shared_ptr<ITextureBinding> binding) = 0;
        virtual void Draw(std::shared_ptr<IRenderPipeline> pipeline,
                          std::shared_ptr<IVertexBuffer>   vertexBuffer,
                          std::shared_ptr<IVertexBuffer>   instanceBuffer)                                           = 0;
        virtual void FinishRecording()                                                                               = 0;
    };
} // namespace Voxium::Platform::Render
