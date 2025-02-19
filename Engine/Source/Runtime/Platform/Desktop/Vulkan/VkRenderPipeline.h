#pragma once

#include "Platform/Render/IRenderPipeline.h"
#include "VkFrameBufferFormat.h"
#include "VkContext.h"
#include "VkShader.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class RenderPipeline final : public Voxium::Platform::Render::IRenderPipeline
    {
    public:
        RenderPipeline(std::shared_ptr<VulkanContext>            context,
                       std::shared_ptr<IFrameBufferFormat>       format,
                       uint32_t                                  stage,
                       std::vector<std::shared_ptr<IShader>>     shaders,
                       const Voxium::Platform::Render::VertexFormatDescriptor&    vertexFormat,
                       const Voxium::Platform::Render::VertexFormatDescriptor&    instanceFormat,
                       Voxium::Platform::Render::RenderState                      state,
                       const std::vector<Voxium::Platform::Render::BlendOptions>& blendOptions);

        [[nodiscard]] vk::Pipeline& Get() { return *pipeline_; }

        [[nodiscard]] vk::PipelineLayout& GetLayout() { return *layout_; }

        [[nodiscard]] uint32_t GetLayoutOffset(const std::shared_ptr<IShader>& shader) const
        {
            return shaderLayoutOffsets_.at(shader.get());
        }

    private:
        std::shared_ptr<VulkanContext>        context_;
        std::shared_ptr<IFrameBufferFormat>   format_;
        std::vector<std::shared_ptr<IShader>> shaders_;
        std::unordered_map<Shader*, uint32_t> shaderLayoutOffsets_;
        std::vector<vk::DescriptorSetLayout>  descriptorSetLayouts_;
        vk::UniquePipelineLayout              layout_;
        vk::UniquePipeline                    pipeline_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
