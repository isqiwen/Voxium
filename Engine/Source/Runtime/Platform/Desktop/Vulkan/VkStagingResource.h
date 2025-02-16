#pragma once

#include <vector>

#include <vulkan.h>

namespace Voxium::Platform::Desktop::Vulkan
{
    class IScalableStagingResource
    {
    public:
        IScalableStagingResource()          = default;
        virtual ~IScalableStagingResource() = default;

        virtual void Reserve(uint32_t stages) = 0;
    };

    template<typename T>
    class StagingResource final
    {
        using Handle = vk::UniqueHandle<T, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    public:
        StagingResource() = default;
        explicit StagingResource(std::vector<Handle>&& handles) : handles_(std::move(handles)) {}
        ~StagingResource() noexcept                   = default;
        StagingResource(const StagingResource& other) = delete;
        StagingResource(StagingResource&& other) noexcept : handles_(std::move(other.handles_)) {}
        StagingResource& operator=(const StagingResource& other) = delete;
        StagingResource& operator=(StagingResource&& other) noexcept
        {
            handles_ = std::move(other.handles_);
            return *this;
        }

        size_t   Size() const { return handles_.size(); }
        T&       operator[](size_t i) { return *handles_[i]; }
        const T& operator[](size_t i) const { return *handles_[i]; }

    private:
        std::vector<Handle> handles_;
    };
} // namespace Voxium::Platform::Desktop::Vulkan::util
