// Header-only library: a generic event system
#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace events {

template <typename... Args>
class Event {
public:
    using Handler = std::function<void(Args...)>;
    using HandlerId = std::size_t;

    HandlerId subscribe(Handler handler) {
        handlers_.push_back({next_id_, std::move(handler)});
        return next_id_++;
    }

    void unsubscribe(HandlerId id) {
        handlers_.erase(
            std::remove_if(handlers_.begin(), handlers_.end(),
                           [id](const Entry& e) { return e.id == id; }),
            handlers_.end());
    }

    void emit(Args... args) const {
        for (const auto& entry : handlers_) {
            entry.handler(args...);
        }
    }

    std::size_t subscriber_count() const { return handlers_.size(); }

    void clear() { handlers_.clear(); }

private:
    struct Entry {
        HandlerId id;
        Handler handler;
    };
    std::vector<Entry> handlers_;
    HandlerId next_id_ = 0;
};

// Convenience: named event bus
class EventBus {
public:
    template <typename Func>
    std::size_t on(const std::string& event_name, Func&& handler) {
        auto& ev = events_[event_name];
        ev.push_back({next_id_, std::forward<Func>(handler)});
        return next_id_++;
    }

    void emit(const std::string& event_name) {
        auto it = events_.find(event_name);
        if (it != events_.end()) {
            for (const auto& entry : it->second) {
                entry.handler();
            }
        }
    }

private:
    struct Entry {
        std::size_t id;
        std::function<void()> handler;
    };
    std::unordered_map<std::string, std::vector<Entry>> events_;
    std::size_t next_id_ = 0;
};

}  // namespace events

#endif  // EVENT_HPP
