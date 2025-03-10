﻿#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <functional>

#include "ICommandBuffer.h"
#include "IFrameBuffer.h"
#include "IFrameBufferFormat.h"
#include "IRenderPipeline.h"
#include "IRenderTarget.h"
#include "IShader.h"
#include "ITextureBinding.h"
#include "ITextureSampler.h"
#include "IUniformBinding.h"
#include "IUniformBuffer.h"
#include "IVertexBuffer.h"
#include "IRenderSurface.h"

namespace Voxium::Platform::Render
{
    enum class FrameBufferAttachmentType : uint8_t
    {
        COLOR,
        DEPTH_STENCIL
    };

    struct FrameBufferAttachmentDescriptor
    {
        const FrameBufferAttachmentType type;
        const TextureFormat             format;
    };

    struct FrameBufferRenderStageDescriptor
    {
        const std::vector<uint32_t> colorAttachments;
        const uint32_t              depthStencilAttachment;
        const std::vector<uint32_t> dependencies;
    };

    enum class NumericType : uint8_t
    {
        BYTE,
        UBYTE,
        SHORT,
        USHORT,
        INT,
        UINT,
        LONG,
        ULONG,
        FLOAT,
        DOUBLE,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT4X4
    };

    enum class VertexFormatDescriptorRate : uint8_t
    {
        VERTEX,
        INSTANCE
    };

    struct VertexFormatElement
    {
        const uint32_t    location;
        const NumericType type;
        const uint32_t    offset;
    };

    struct VertexFormatDescriptor
    {
        const std::vector<VertexFormatElement> elements;
        const uint32_t                         size;
        const VertexFormatDescriptorRate       rate;
    };

    enum class Topology : uint8_t
    {
        POINTS,
        LINES,
        LINE_STRIPS,
        TRIANGLES,
        TRIANGLE_STRIPS,
        TRIANGLE_FANS
    };

    enum class RasterMode : uint8_t
    {
        FILL,
        LINE,
        POINT
    };

    enum class CullingMode : uint8_t
    {
        FRONT,
        BACK,
        FRONT_AND_BACK,
        NONE
    };

    enum class FrontFace : uint8_t
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };

    struct DepthBias
    {
        const bool  enabled;
        const float constantFactor;
        const float clamp;
        const float slopeFactor;
    };

    enum class CompareOperation : uint8_t
    {
        NEVER,
        LESS,
        LESS_OR_EQUAL,
        EQUAL,
        NOT_EQUAL,
        GREATER_OR_EQUAL,
        GREATER,
        ALWAYS
    };

    struct DepthTest
    {
        const bool             enabled;
        const CompareOperation comparison;
        const bool             write;
    };

    enum class StencilOperation : uint8_t
    {
        ZERO,
        KEEP,
        REPLACE,
        INVERT,
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP
    };

    struct StencilFaceOperation
    {
        const StencilOperation stencilFailOperation;
        const StencilOperation depthFailOperation;
        const StencilOperation successOperation;
        const CompareOperation compareOperation;
        const uint32_t         compareMask;
        const uint32_t         writeMask;
        const uint32_t         value;
    };

    struct StencilTest
    {
        const bool                 enabled;
        const StencilFaceOperation front;
        const StencilFaceOperation back;
    };

    enum class BlendFactor : uint8_t
    {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CST_COLOR,
        ONE_MINUS_CST_COLOR,
        CST_ALPHA,
        ONE_MINUS_CST_ALPHA
    };

    enum class BlendOperation : uint8_t
    {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };

    enum class ColorComponents : uint8_t
    {
        RED   = 1 << 0,
        GREEN = 1 << 1,
        BLUE  = 1 << 2,
        ALPHA = 1 << 3
    };

    inline ColorComponents operator&(ColorComponents lhs, ColorComponents rhs)
    {
        return static_cast<ColorComponents>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }

    struct BlendOptions
    {
        const bool            enabled;
        const BlendFactor     srcColor, dstColor;
        const BlendOperation  colorOperation;
        const BlendFactor     srcAlpha, dstAlpha;
        const BlendOperation  alphaOperation;
        const ColorComponents components;
    };

    struct RenderState
    {
        const Topology                   topology;
        const RasterMode                 rasterMode;
        const bool                       discardRaster;
        const std::optional<float>       lineWidth;
        const std::optional<DepthBias>   depthBias;
        const std::optional<DepthTest>   depthTest;
        const std::optional<StencilTest> stencilTest;
        const std::optional<CullingMode> cullingMode;
        const std::optional<FrontFace>   frontFace;
    };

    enum class ShaderType : uint8_t
    {
        VERTEX,
        FRAGMENT
    };

    struct ShaderUniformProperty
    {
        const NumericType type;
    };

    enum class ShaderBindingType : uint8_t
    {
        UNIFORM,
        SAMPLER
    };

    struct ShaderBinding
    {
        const ShaderBindingType                  type;
        const uint32_t                           size;
        const std::vector<ShaderUniformProperty> properties;
    };

    enum class TextureFiltering : uint8_t
    {
        LINEAR,
        NEAREST
    };

    enum class TextureWrapping : uint8_t
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        MIRRORED_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class TextureBorderColor : uint8_t
    {
        TRANSPARENT_BLACK,
        OPAQUE_BLACK,
        OPAQUE_WHITE
    };

    class IRenderContext
    {
    public:
        IRenderContext()                                           = default;
        virtual ~IRenderContext()                                  = default;
        IRenderContext(const IRenderContext& other)                = delete;
        IRenderContext(IRenderContext&& other) noexcept            = delete;
        IRenderContext& operator=(const IRenderContext& other)     = delete;
        IRenderContext& operator=(IRenderContext&& other) noexcept = delete;

        [[nodiscard]] virtual std::shared_ptr<IFrameBufferFormat> GetSurfaceFormat() = 0;

        [[nodiscard]] virtual std::shared_ptr<IFrameBufferFormat>
        CreateFrameBufferFormat(const std::vector<FrameBufferAttachmentDescriptor>&  attachments,
                                const std::vector<FrameBufferRenderStageDescriptor>& renderStages) = 0;

        [[nodiscard]] virtual std::shared_ptr<IFrameBuffer>
        CreateFrameBuffer(const std::shared_ptr<IFrameBufferFormat>& format, uint32_t width, uint32_t height) = 0;

        [[nodiscard]] virtual std::shared_ptr<IShader>
        CreateShader(ShaderType type, const std::vector<uint8_t>& data, const std::vector<ShaderBinding>& bindings) = 0;

        [[nodiscard]] virtual std::shared_ptr<IRenderPipeline>
        CreatePipeline(const std::shared_ptr<IFrameBufferFormat>&   format,
                       uint32_t                                     stage,
                       const std::vector<std::shared_ptr<IShader>>& shaders,
                       const VertexFormatDescriptor&                vertexFormat,
                       const VertexFormatDescriptor&                instanceFormat,
                       RenderState                                  state,
                       const std::vector<BlendOptions>&             blendOptions) = 0;

        [[nodiscard]] virtual std::shared_ptr<IUniformBinding>
        CreateUniformBinding(const std::shared_ptr<IShader>&        shader,
                             uint32_t                               binding,
                             const std::shared_ptr<IUniformBuffer>& uniformBuffer) = 0;

        [[nodiscard]] virtual std::shared_ptr<IUniformBuffer>
        CreateUniformBuffer(const std::vector<uint8_t>& initialData) = 0;

        [[nodiscard]] virtual std::shared_ptr<IVertexBuffer>
        CreateVertexBuffer(const std::vector<uint8_t>& initialData, uint32_t vertexSize, bool writable) = 0;

        [[nodiscard]] virtual std::shared_ptr<ITextureBinding>
        CreateTextureBinding(const std::shared_ptr<IShader>&         shader,
                             uint32_t                                binding,
                             const std::shared_ptr<ITextureSampler>& sampler,
                             const std::shared_ptr<ITexture>&        texture) = 0;

        [[nodiscard]] virtual std::shared_ptr<ITextureSampler> CreateTextureSampler(TextureFiltering   minFiltering,
                                                                                    TextureFiltering   magFiltering,
                                                                                    TextureWrapping    wrapping,
                                                                                    TextureBorderColor borderColor,
                                                                                    bool               enableAnisotropy,
                                                                                    uint32_t           anisotropyLevel) = 0;

        [[nodiscard]] virtual std::shared_ptr<ITexture>
        CreateTexture(uint32_t width, uint32_t height, const std::vector<uint8_t>& data) = 0;

        [[nodiscard]] virtual std::shared_ptr<ICommandBuffer> CreateCommandBuffer() = 0;

        [[nodiscard]] virtual IFrameBuffer& GetFrameBuffer() = 0;

        virtual void UpdateFirst() = 0;
        virtual void UpdateLast()  = 0;

        virtual void Enqueue(const std::shared_ptr<IRenderTarget>&  renderTarget,
                             const std::shared_ptr<ICommandBuffer>& commandBuffer) = 0;
    };

    using RenderContextFactory = std::function<std::unique_ptr<IRenderContext>(IRenderSurface&, IRenderSurface*)>;
} // namespace Voxium::Platform::Render
