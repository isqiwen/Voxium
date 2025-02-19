#include "VkRenderPipeline.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    const uint32_t BINDING_VERTEX   = 0;
    const uint32_t BINDING_INSTANCE = 1;

    const Voxium::Platform::Render::CullingMode DEFAULT_CULLING_MODE = Voxium::Platform::Render::CullingMode::BACK;
    const Voxium::Platform::Render::FrontFace   DEFAULT_FRONT_FACE   = Voxium::Platform::Render::FrontFace::CLOCKWISE;
    const Voxium::Platform::Render::DepthBias   DEFAULT_DEPTH_BIAS   = Voxium::Platform::Render::DepthBias {false, 0.0f, 0.0f, 0.0f};
    const float               DEFAULT_LINE_WIDTH   = 1.0f;
    const Voxium::Platform::Render::DepthTest   DEFAULT_DEPTH_TEST   = Voxium::Platform::Render::DepthTest {false, Voxium::Platform::Render::CompareOperation::NEVER, false};
    const Voxium::Platform::Render::StencilTest DEFAULT_STENCIL_TEST = Voxium::Platform::Render::StencilTest {false, {}, {}};

    vk::Format ToVulkan(const Voxium::Platform::Render::NumericType type)
    {
        switch (type)
        {
            case Voxium::Platform::Render::NumericType::BYTE:
                return vk::Format::eR8Sint;
            case Voxium::Platform::Render::NumericType::UBYTE:
                return vk::Format::eR8Uint;
            case Voxium::Platform::Render::NumericType::SHORT:
                return vk::Format::eR16Sint;
            case Voxium::Platform::Render::NumericType::USHORT:
                return vk::Format::eR16Uint;
            case Voxium::Platform::Render::NumericType::INT:
                return vk::Format::eR32Sint;
            case Voxium::Platform::Render::NumericType::UINT:
                return vk::Format::eR32Uint;
            case Voxium::Platform::Render::NumericType::LONG:
                return vk::Format::eR64Sint;
            case Voxium::Platform::Render::NumericType::ULONG:
                return vk::Format::eR64Uint;
            case Voxium::Platform::Render::NumericType::FLOAT:
                return vk::Format::eR32Sfloat;
            case Voxium::Platform::Render::NumericType::DOUBLE:
                return vk::Format::eR64Sfloat;
            case Voxium::Platform::Render::NumericType::FLOAT2:
                return vk::Format::eR32G32Sfloat;
            case Voxium::Platform::Render::NumericType::FLOAT3:
                return vk::Format::eR32G32B32Sfloat;
            case Voxium::Platform::Render::NumericType::FLOAT4:
                return vk::Format::eR32G32B32A32Sfloat;
            default:
                throw std::runtime_error("Unsupported type.");
        }
    }

    std::vector<vk::VertexInputAttributeDescription>
    ToVulkan(const Voxium::Platform::Render::VertexFormatDescriptor& format, const uint32_t binding, const uint32_t locationOffset)
    {
        std::vector<vk::VertexInputAttributeDescription> descriptions;
        descriptions.reserve(format.elements.size());
        for (const auto& element : format.elements)
        {
            vk::VertexInputAttributeDescription description {element.location + locationOffset,
                                                             binding,
                                                             ToVulkan(element.type),
                                                             static_cast<uint32_t>(element.offset)};
            descriptions.push_back(description);
        }
        return descriptions;
    }

    vk::PrimitiveTopology ToVulkan(const Voxium::Platform::Render::Topology topology)
    {
        switch (topology)
        {
            case Voxium::Platform::Render::Topology::POINTS:
                return vk::PrimitiveTopology::ePointList;
            case Voxium::Platform::Render::Topology::LINES:
                return vk::PrimitiveTopology::eLineList;
            case Voxium::Platform::Render::Topology::LINE_STRIPS:
                return vk::PrimitiveTopology::eLineStrip;
            case Voxium::Platform::Render::Topology::TRIANGLES:
                return vk::PrimitiveTopology::eTriangleList;
            case Voxium::Platform::Render::Topology::TRIANGLE_STRIPS:
                return vk::PrimitiveTopology::eTriangleStrip;
            case Voxium::Platform::Render::Topology::TRIANGLE_FANS:
                return vk::PrimitiveTopology::eTriangleFan;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::PolygonMode ToVulkan(const Voxium::Platform::Render::RasterMode rasterMode)
    {
        switch (rasterMode)
        {
            case Voxium::Platform::Render::RasterMode::FILL:
                return vk::PolygonMode::eFill;
            case Voxium::Platform::Render::RasterMode::LINE:
                return vk::PolygonMode::eLine;
            case Voxium::Platform::Render::RasterMode::POINT:
                return vk::PolygonMode::ePoint;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::CullModeFlags ToVulkan(const Voxium::Platform::Render::CullingMode cullingMode)
    {
        switch (cullingMode)
        {
            case Voxium::Platform::Render::CullingMode::FRONT:
                return vk::CullModeFlagBits::eFront;
            case Voxium::Platform::Render::CullingMode::BACK:
                return vk::CullModeFlagBits::eBack;
            case Voxium::Platform::Render::CullingMode::FRONT_AND_BACK:
                return vk::CullModeFlagBits::eFrontAndBack;
            case Voxium::Platform::Render::CullingMode::NONE:
                return vk::CullModeFlagBits::eNone;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::FrontFace ToVulkan(const Voxium::Platform::Render::FrontFace frontFace)
    {
        switch (frontFace)
        {
            case Voxium::Platform::Render::FrontFace::CLOCKWISE:
                return vk::FrontFace::eClockwise;
            case Voxium::Platform::Render::FrontFace::COUNTER_CLOCKWISE:
                return vk::FrontFace::eCounterClockwise;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::CompareOp ToVulkan(const Voxium::Platform::Render::CompareOperation operation)
    {
        switch (operation)
        {
            case Voxium::Platform::Render::CompareOperation::NEVER:
                return vk::CompareOp::eNever;
            case Voxium::Platform::Render::CompareOperation::LESS:
                return vk::CompareOp::eLess;
            case Voxium::Platform::Render::CompareOperation::LESS_OR_EQUAL:
                return vk::CompareOp::eLessOrEqual;
            case Voxium::Platform::Render::CompareOperation::EQUAL:
                return vk::CompareOp::eEqual;
            case Voxium::Platform::Render::CompareOperation::NOT_EQUAL:
                return vk::CompareOp::eNotEqual;
            case Voxium::Platform::Render::CompareOperation::GREATER_OR_EQUAL:
                return vk::CompareOp::eGreaterOrEqual;
            case Voxium::Platform::Render::CompareOperation::GREATER:
                return vk::CompareOp::eGreater;
            case Voxium::Platform::Render::CompareOperation::ALWAYS:
                return vk::CompareOp::eAlways;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::StencilOp ToVulkan(const Voxium::Platform::Render::StencilOperation operation)
    {
        switch (operation)
        {
            case Voxium::Platform::Render::StencilOperation::ZERO:
                return vk::StencilOp::eZero;
            case Voxium::Platform::Render::StencilOperation::KEEP:
                return vk::StencilOp::eKeep;
            case Voxium::Platform::Render::StencilOperation::REPLACE:
                return vk::StencilOp::eReplace;
            case Voxium::Platform::Render::StencilOperation::INVERT:
                return vk::StencilOp::eInvert;
            case Voxium::Platform::Render::StencilOperation::INCREMENT_AND_CLAMP:
                return vk::StencilOp::eIncrementAndClamp;
            case Voxium::Platform::Render::StencilOperation::DECREMENT_AND_CLAMP:
                return vk::StencilOp::eDecrementAndClamp;
            case Voxium::Platform::Render::StencilOperation::INCREMENT_AND_WRAP:
                return vk::StencilOp::eIncrementAndWrap;
            case Voxium::Platform::Render::StencilOperation::DECREMENT_AND_WRAP:
                return vk::StencilOp::eDecrementAndWrap;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::StencilOpState ToVulkan(const Voxium::Platform::Render::StencilFaceOperation operation)
    {
        return vk::StencilOpState {ToVulkan(operation.stencilFailOperation),
                                   ToVulkan(operation.successOperation),
                                   ToVulkan(operation.depthFailOperation),
                                   ToVulkan(operation.compareOperation),
                                   operation.compareMask,
                                   operation.writeMask,
                                   operation.value};
    }

    vk::BlendFactor ToVulkan(const Voxium::Platform::Render::BlendFactor factor)
    {
        switch (factor)
        {
            case Voxium::Platform::Render::BlendFactor::ZERO:
                return vk::BlendFactor::eZero;
            case Voxium::Platform::Render::BlendFactor::ONE:
                return vk::BlendFactor::eOne;
            case Voxium::Platform::Render::BlendFactor::SRC_COLOR:
                return vk::BlendFactor::eSrcColor;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_SRC_COLOR:
                return vk::BlendFactor::eOneMinusSrcColor;
            case Voxium::Platform::Render::BlendFactor::SRC_ALPHA:
                return vk::BlendFactor::eSrcAlpha;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_SRC_ALPHA:
                return vk::BlendFactor::eOneMinusSrcAlpha;
            case Voxium::Platform::Render::BlendFactor::DST_COLOR:
                return vk::BlendFactor::eDstColor;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_DST_COLOR:
                return vk::BlendFactor::eOneMinusDstColor;
            case Voxium::Platform::Render::BlendFactor::DST_ALPHA:
                return vk::BlendFactor::eDstAlpha;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_DST_ALPHA:
                return vk::BlendFactor::eOneMinusDstAlpha;
            case Voxium::Platform::Render::BlendFactor::CST_COLOR:
                return vk::BlendFactor::eConstantColor;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_CST_COLOR:
                return vk::BlendFactor::eOneMinusConstantColor;
            case Voxium::Platform::Render::BlendFactor::CST_ALPHA:
                return vk::BlendFactor::eConstantAlpha;
            case Voxium::Platform::Render::BlendFactor::ONE_MINUS_CST_ALPHA:
                return vk::BlendFactor::eOneMinusConstantAlpha;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::BlendOp ToVulkan(const Voxium::Platform::Render::BlendOperation operation)
    {
        switch (operation)
        {
            case Voxium::Platform::Render::BlendOperation::ADD:
                return vk::BlendOp::eAdd;
            case Voxium::Platform::Render::BlendOperation::SUBTRACT:
                return vk::BlendOp::eSubtract;
            case Voxium::Platform::Render::BlendOperation::REVERSE_SUBTRACT:
                return vk::BlendOp::eReverseSubtract;
            case Voxium::Platform::Render::BlendOperation::MIN:
                return vk::BlendOp::eMin;
            case Voxium::Platform::Render::BlendOperation::MAX:
                return vk::BlendOp::eMax;
        }
        throw std::runtime_error("Invalid type.");
    }

    vk::ColorComponentFlags ToVulkan(const Voxium::Platform::Render::ColorComponents components)
    {
        vk::ColorComponentFlags flags = {};
        if (static_cast<uint8_t>(components & Voxium::Platform::Render::ColorComponents::RED))
            flags |= vk::ColorComponentFlagBits::eR;
        if (static_cast<uint8_t>(components & Voxium::Platform::Render::ColorComponents::GREEN))
            flags |= vk::ColorComponentFlagBits::eG;
        if (static_cast<uint8_t>(components & Voxium::Platform::Render::ColorComponents::BLUE))
            flags |= vk::ColorComponentFlagBits::eB;
        if (static_cast<uint8_t>(components & Voxium::Platform::Render::ColorComponents::ALPHA))
            flags |= vk::ColorComponentFlagBits::eA;
        return flags;
    }

    vk::PipelineColorBlendAttachmentState ToVulkan(const Voxium::Platform::Render::BlendOptions options)
    {
        return vk::PipelineColorBlendAttachmentState {options.enabled,
                                                      ToVulkan(options.srcColor),
                                                      ToVulkan(options.dstColor),
                                                      ToVulkan(options.colorOperation),
                                                      ToVulkan(options.srcAlpha),
                                                      ToVulkan(options.dstAlpha),
                                                      ToVulkan(options.alphaOperation),
                                                      ToVulkan(options.components)};
    }

    std::vector<vk::DynamicState> ToVulkan(const Voxium::Platform::Render::RenderState state)
    {
        std::vector<vk::DynamicState> dynamicStates;
        if (!state.depthBias.has_value())
            dynamicStates.push_back(vk::DynamicState::eDepthBias);
        if (!state.cullingMode.has_value())
            dynamicStates.push_back(vk::DynamicState::eCullModeEXT);
        if (!state.frontFace.has_value())
            dynamicStates.push_back(vk::DynamicState::eFrontFaceEXT);
        if (!state.lineWidth.has_value())
            dynamicStates.push_back(vk::DynamicState::eLineWidth);
        if (!state.depthTest.has_value())
            dynamicStates.insert(dynamicStates.end(),
                                 {vk::DynamicState::eDepthTestEnableEXT,
                                  vk::DynamicState::eDepthCompareOpEXT,
                                  vk::DynamicState::eDepthWriteEnableEXT});
        if (!state.stencilTest.has_value())
            dynamicStates.insert(dynamicStates.end(),
                                 {vk::DynamicState::eStencilTestEnableEXT,
                                  vk::DynamicState::eStencilOpEXT,
                                  vk::DynamicState::eStencilCompareMask,
                                  vk::DynamicState::eStencilReference,
                                  vk::DynamicState::eStencilWriteMask});
        return dynamicStates;
    }

    RenderPipeline::RenderPipeline(std::shared_ptr<VulkanContext>           context,
                                   std::shared_ptr<IFrameBufferFormat>       format,
                                   const uint32_t                           stage,
                                   std::vector<std::shared_ptr<IShader>>     shaders,
                                   const Voxium::Platform::Render::VertexFormatDescriptor&    vertexFormat,
                                   const Voxium::Platform::Render::VertexFormatDescriptor&    instanceFormat,
                                   const Voxium::Platform::Render::RenderState                state,
                                   const std::vector<Voxium::Platform::Render::BlendOptions>& blendOptions) :
        context_(std::move(context)), format_(std::move(format)), shaders_(std::move(shaders))
    {
        std::vector<vk::VertexInputBindingDescription>   vertexBindings;
        std::vector<vk::VertexInputAttributeDescription> vertexAttributes;

        vertexBindings.emplace_back(BINDING_VERTEX, vertexFormat.size, vk::VertexInputRate::eVertex);
        auto perVertexAttributes = ToVulkan(vertexFormat, BINDING_VERTEX, 0);
        vertexAttributes.insert(vertexAttributes.end(), perVertexAttributes.begin(), perVertexAttributes.end());

        if (instanceFormat.size > 0)
        {
            vertexBindings.emplace_back(BINDING_INSTANCE, instanceFormat.size, vk::VertexInputRate::eInstance);
            auto perInstanceAttributes =
                ToVulkan(instanceFormat, BINDING_INSTANCE, static_cast<uint32_t>(vertexFormat.elements.size()));
            vertexAttributes.insert(vertexAttributes.end(), perInstanceAttributes.begin(), perInstanceAttributes.end());
        }

        vk::PipelineVertexInputStateCreateInfo   vertexInputStateCreateInfo {{}, vertexBindings, vertexAttributes};
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {{}, ToVulkan(state.topology), false};

        const auto                               depthBias = state.depthBias.value_or(DEFAULT_DEPTH_BIAS);
        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {
            {},
            false,
            state.discardRaster,
            ToVulkan(state.rasterMode),
            ToVulkan(state.cullingMode.value_or(DEFAULT_CULLING_MODE)),
            ToVulkan(state.frontFace.value_or(DEFAULT_FRONT_FACE)),
            depthBias.enabled,
            depthBias.constantFactor,
            depthBias.clamp,
            depthBias.slopeFactor,
            state.lineWidth.value_or(DEFAULT_LINE_WIDTH)};

        vk::PipelineTessellationStateCreateInfo tessellationStateCreateInfo {};
        vk::Viewport                            viewport {0, 0, 0, 0, 0, 1};
        vk::Rect2D                              scissor {{0, 0}, {0, 0}};
        vk::PipelineViewportStateCreateInfo     viewportStateCreateInfo {{}, 1, &viewport, 1, &scissor};
        vk::PipelineMultisampleStateCreateInfo  multisampleStateCreateInfo {
             {}, vk::SampleCountFlagBits::e1, false, 1.0f, nullptr, false, false};

        const auto                              depthTest   = state.depthTest.value_or(DEFAULT_DEPTH_TEST);
        const auto                              stencilTest = state.stencilTest.value_or(DEFAULT_STENCIL_TEST);
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo {{},
                                                                             depthTest.enabled,
                                                                             depthTest.write,
                                                                             ToVulkan(depthTest.comparison),
                                                                             false,
                                                                             stencilTest.enabled,
                                                                             ToVulkan(stencilTest.front),
                                                                             ToVulkan(stencilTest.back),
                                                                             0.0f,
                                                                             1.0f};

        std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
        for (auto i = 0u; i < format_->GetAttachmentCount(); ++i)
        {
            if (format_->GetAttachments()[i].type == Voxium::Platform::Render::FrameBufferAttachmentType::COLOR)
                blendAttachments.push_back(ToVulkan(blendOptions[i]));
        }
        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
            {}, false, vk::LogicOp::eCopy, blendAttachments, {0.0f, 0.0f, 0.0f, 0.0f}};

        auto dynamicStates = std::vector(ToVulkan(state));
        dynamicStates.insert(dynamicStates.end(), {vk::DynamicState::eViewport, vk::DynamicState::eScissor});
        vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo {{}, dynamicStates};

        uint32_t descriptorOffset = 0;
        for (const auto& shader : shaders_)
        {
            auto& layouts = shader->GetDescriptorSetLayouts();
            descriptorSetLayouts_.insert(descriptorSetLayouts_.end(), layouts.begin(), layouts.end());

            shaderLayoutOffsets_.emplace(shader.get(), descriptorOffset);
            descriptorOffset += static_cast<uint32_t>(layouts.size());
        }
        layout_ = context_->device_->CreatePipelineLayoutUnique({{}, descriptorSetLayouts_});

        // TODO: Other layout / uniform stuff

        std::vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos;
        for (const auto& shader : shaders_)
            pipelineShaderStageCreateInfos.push_back({{}, shader->GetStage(), shader->GetModule(), "main", {}});

        pipeline_ = context_->device_
                         ->createGraphicsPipelineUnique(*context_->pipelineCache_,
                                                        {{},
                                                         pipelineShaderStageCreateInfos,
                                                         &vertexInputStateCreateInfo,
                                                         &inputAssemblyStateCreateInfo,
                                                         &tessellationStateCreateInfo,
                                                         &viewportStateCreateInfo,
                                                         &rasterizationStateCreateInfo,
                                                         &multisampleStateCreateInfo,
                                                         &depthStencilStateCreateInfo,
                                                         &colorBlendStateCreateInfo,
                                                         &dynamicStateCreateInfo,
                                                         layout_.get(),
                                                         format_->GetPass(),
                                                         stage,
                                                         nullptr,
                                                         -1})
                         .value;
    }
} // namespace Voxium::Platform::Desktop::Vulkan
