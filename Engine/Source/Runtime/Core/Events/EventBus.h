#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <typeindex>
#include <unordered_map>

#include "Core/Pattern/Singleton.h"

#include "IEvent.h"

namespace Voxium::Core::Event
{

    class EventBus : public Voxium::Core::Pattern::Singleton<EventBus>
    {
        friend class Voxium::Core::Pattern::Singleton<EventBus>;

    private:
        EventBus(Token) {}

    public:
        template<typename TEvent>
        void Subscribe(std::function<void(const TEvent&)> handler)
        {
            static_assert(std::is_base_of_v<IEvent, TEvent>, "TEvent must inherit from IEvent");
            std::unique_lock lock(mutex_);
            auto&            subscribers = eventMap_[typeid(TEvent)];
            subscribers.emplace_back([handler](const IEvent& evt) { handler(static_cast<const TEvent&>(evt)); });
        }

        template<typename TEvent>
        void UnsubscribeAll()
        {
            static_assert(std::is_base_of_v<IEvent, TEvent>, "TEvent must inherit from IEvent");
            std::unique_lock lock(mutex_);
            eventMap_.erase(typeid(TEvent));
        }

        template<typename TEvent>
        void Post(const TEvent& evt)
        {
            static_assert(std::is_base_of_v<IEvent, TEvent>, "TEvent must inherit from IEvent");
            std::shared_lock lock(mutex_);
            auto             it = eventMap_.find(typeid(TEvent));
            if (it != eventMap_.end())
            {
                for (const auto& subscriber : it->second)
                {
                    subscriber(evt);
                }
            }
        }

    private:
        std::unordered_map<std::type_index, std::deque<std::function<void(const IEvent&)>>> eventMap_;
        mutable std::shared_mutex                                                           mutex_;
    };

} // namespace Voxium::Core::Event
