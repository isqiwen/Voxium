#pragma once

namespace Voxium::Core::Pattern
{
    template<typename T>
    class Singleton
    {
    public:
        static T& Instance() noexcept
        {
            static T instance {Token {}};
            return instance;
        }

        Singleton(const Singleton&)            = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&)                 = delete;
        Singleton& operator=(Singleton&&)      = delete;

    protected:
        struct Token
        {};

        Singleton() {}
    };

} // namespace Voxium::Core::Pattern
