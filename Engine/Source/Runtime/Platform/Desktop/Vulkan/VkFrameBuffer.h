#pragma once
#include "../../render/FrameBuffer.h"
#include "vk_FrameBuffer_format.h"
#include "VkTexture.h"

namespace Voxium::Platform::Desktop::Vulkan
{
    class FrameBufferTexture final : public Texture
    {
    public:
        FrameBufferTexture(std::shared_ptr<VulkanContext>            context,
                           std::vector<std::unique_ptr<VulkanImage>> images,
                           std::vector<vk::UniqueImageView>          imageViews,
                           const uint32_t                            width,
                           const uint32_t                            height) :
            context_(std::move(context)), image_s(std::move(images)), image_Views(std::move(imageViews)),
            width_(width), height_(height)
        {}

        [[nodiscard]] uint32_t GetWidth() override { return width_; }

        [[nodiscard]] uint32_t GetHeight() override { return height_; }

        [[nodiscard]] vk::ImageView& get() override { return *image_Views[readIndex_]; }

    private:
        std::shared_ptr<VulkanContext>            context_;
        std::vector<std::unique_ptr<VulkanImage>> image_s;
        std::vector<vk::UniqueImageView>          image_Views;
        uint32_t                                  width_, height_;
        uint32_t                                  readIndex_ = 0;

        friend class FrameBuffer;
    };

    class FrameBuffer final : public Voxium::Platform::Render::FrameBuffer
    {
    public:
        FrameBuffer(std::shared_ptr<VulkanContext>     context,
                    std::shared_ptr<IFrameBufferFormat> format,
                    uint32_t                           width,
                    uint32_t                           height,
                    uint32_t                           stages);

        FrameBuffer(std::shared_ptr<VulkanContext>     context,
                    std::shared_ptr<IFrameBufferFormat> format,
                    uint32_t                           width,
                    uint32_t                           height,
                    std::vector<vk::UniqueImageView>   imageViews,
                    std::vector<vk::UniqueFrameBuffer> FrameBuffers);

        [[nodiscard]] const FrameBufferFormat&         GetFormat() const override { return *m_format; }
        [[nodiscard]] uint32_t                         GetWidth() const override { return width_; }
        [[nodiscard]] uint32_t                         GetHeight() const override { return height_; }
        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::Texture> GetTexture(const uint32_t attachment) override
        {
            return m_textures[attachment];
        }

        [[nodiscard]] vk::FrameBuffer& getTarget() { return *m_FrameBuffers[getWriteIndex()]; }

        void advance();

        void transitionTexturesPost(const vk::CommandBuffer& cmd);

    private:
        [[nodiscard]] uint32_t GetWriteIndex() const { return (m_writeIndex + 1) % m_FrameBuffers.size(); }

        std::shared_ptr<VulkanContext> context_;

        const std::shared_ptr<IFrameBufferFormat> m_format;
        const uint32_t                           width_, height_;

        std::vector<vk::UniqueFrameBuffer>               m_FrameBuffers;
        std::vector<std::shared_ptr<FrameBufferTexture>> m_textures;
        uint32_t                                         m_writeIndex = 0;
        bool                                             m_shouldTransition;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
