#pragma once

#include "Platform/Render/IFrameBuffer.h"

#include "VkFrameBufferFormat.h"
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
            context_(std::move(context)), images_(std::move(images)), imageViews_(std::move(imageViews)),
            width_(width), height_(height)
        {}

        [[nodiscard]] uint32_t GetWidth() override { return width_; }

        [[nodiscard]] uint32_t GetHeight() override { return height_; }

        [[nodiscard]] vk::ImageView& Get() override { return *imageViews_[readIndex_]; }

    private:
        std::shared_ptr<VulkanContext>            context_;
        std::vector<std::unique_ptr<VulkanImage>> images_;
        std::vector<vk::UniqueImageView>          imageViews_;
        uint32_t                                  width_, height_;
        uint32_t                                  readIndex_ = 0;

        friend class FrameBuffer;
    };

    class FrameBuffer final : public Voxium::Platform::Render::FrameBuffer
    {
    public:
        FrameBuffer(std::shared_ptr<VulkanContext>      context,
                    std::shared_ptr<IFrameBufferFormat> format,
                    uint32_t                            width,
                    uint32_t                            height,
                    uint32_t                            stages);

        FrameBuffer(std::shared_ptr<VulkanContext>      context,
                    std::shared_ptr<IFrameBufferFormat> format,
                    uint32_t                            width,
                    uint32_t                            height,
                    std::vector<vk::UniqueImageView>    imageViews,
                    std::vector<vk::UniqueFrameBuffer>  FrameBuffers);

        [[nodiscard]] const FrameBufferFormat&         GetFormat() const override { return *format_; }
        [[nodiscard]] uint32_t                         GetWidth() const override { return width_; }
        [[nodiscard]] uint32_t                         GetHeight() const override { return height_; }
        [[nodiscard]] std::shared_ptr<Voxium::Platform::Render::Texture> GetTexture(const uint32_t attachment) override
        {
            return textures_[attachment];
        }

        [[nodiscard]] vk::FrameBuffer& getTarget() { return *frameBuffers_[GetWriteIndex()]; }

        void Advance();;

        void TransitionTexturesPost(const vk::CommandBuffer& cmd);

    private:
        [[nodiscard]] uint32_t GetWriteIndex() const { return (writeIndex_ + 1) % frameBuffers_.size(); }

        std::shared_ptr<VulkanContext> context_;

        const std::shared_ptr<IFrameBufferFormat> format_;
        const uint32_t                            width_, height_;

        std::vector<vk::UniqueFrameBuffer>               frameBuffers_;
        std::vector<std::shared_ptr<FrameBufferTexture>> textures_;
        uint32_t                                         writeIndex_ = 0;
        bool                                             shouldTransition_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan
