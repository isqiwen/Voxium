#pragma once

#include "../../render/render_pipeline.h"
#include "vk_FrameBuffer_format.h"
#include "VkContext.h"
#include "VkShader.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class RenderPipeline final : public Voxium::Platform::Render::RenderPipeline
    {
    public:
        RenderPipeline(std::shared_ptr<VulkanContext>           context,
                       std::shared_ptr<IFrameBufferFormat>       format,
                       uint32_t                                 stage,
                       std::vector<std::shared_ptr<IShader>>     shaders,
                       const Voxium::Platform::Render::VertexFormatDescriptor&    vertexFormat,
                       const Voxium::Platform::Render::VertexFormatDescriptor&    instanceFormat,
                       Voxium::Platform::Render::RenderState                      state,
                       const std::vector<Voxium::Platform::Render::BlendOptions>& blendOptions);

        [[nodiscard]] vk::Pipeline& get() { return *m_pipeline; }

        [[nodiscard]] vk::PipelineLayout& getLayout() { return *m_layout; }

        [[nodiscard]] uint32_t getLayoutOffset(const std::shared_ptr<IShader>& shader) const
        {
            return shader_LayoutOffsets.at(shader.get());
        }

    private:
        std::shared_ptr<VulkanContext>        context_;
        std::shared_ptr<IFrameBufferFormat>    m_format;
        std::vector<std::shared_ptr<IShader>>  shader_s;
        std::unordered_map<Shader*, uint32_t> shader_LayoutOffsets;
        std::vector<vk::DescriptorSetLayout>  m_descriptorSetLayouts;
        vk::UniquePipelineLayout              m_layout;
        vk::UniquePipeline                    m_pipeline;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
