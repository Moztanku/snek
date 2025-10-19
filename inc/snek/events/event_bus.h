//
// Created by adame on 10/19/2025.
//

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <functional>
#include <map>
#include <any>
#include <string_view>

/**
 * This is needed only to decouple input logic from game logic
 * it is used by
    * input_manager (user input logic)
    * game_controller (game logic)
 */
class EventBus {
public:
    // from what I've read string_view is just a fancier string reference
    template <typename T>
    void subscribe(const std::string_view eventType, std::function<void(const T&)> listener) {
        listeners[eventType].push_back([listener](const std::any& data) {
            listener(std::any_cast<const T&>(data));
        });
    }

    template <typename T>
    void publish(const std::string_view event_type, const T& eventData) {
        if (listeners.contains(event_type)) {
            for (const auto& listener : listeners.at(event_type)) {
                listener(eventData);
            }
        }
    }
private:
    std::map<std::string_view, std::vector<std::function<void(const std::any&)>>> listeners;
};

#endif //EVENT_BUS_H
