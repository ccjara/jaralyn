#ifndef JARALYN_EVENT_MANAGER_HXX
#define JARALYN_EVENT_MANAGER_HXX

enum class EventType;

template<typename T>
concept MethodInvocable = std::is_member_function_pointer<T>::value;

template<typename T>
concept EventLike = requires {
    { T::event_type } -> std::convertible_to<EventType>;
};

class EventManager {
public:
    /**
     * @brief Registers a free function to be called if the given event is triggered.
     *
     * The function must return a boolean, indicating whether the event was `handled`
     * and should not propagate further to any other event handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike E, typename Fn>
    void on(Fn callable, i32 priority = 0) {
        EventPartition<E>& partition = get_or_create_partition<E>();
        partition.add_callable(callable, priority);
    }

    /**
     * @brief Registers member function to be called if the given event is triggered.
     *
     * The function must return a boolean, indicating whether the event was `handled`
     * and should not propagate further to any other event handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike E, typename Inst, MethodInvocable Fn>
    void on(Inst* instance, Fn method, i32 priority = 0) {
        EventPartition<E>& partition = get_or_create_partition<E>();
        partition.add_callable(
            [instance, method](E& event) -> bool {
                return (instance->*method)(event);
            },
            priority
        );
    }

    /**
     * @brief Notifies all event listeners.
     *
     * Constructs an instance of the given event type in-place and calls all
     * registered handlers in order of their registered priority.
     *
     * Short-circuits if any handler returned `true`.
     */
    template<EventLike E, typename... Args>
    void trigger(Args&& ...args) {
        E event(std::forward<Args>(args)...);
        EventPartition<E>& partition = get_or_create_partition<E>();
        for (auto& handler : partition.event_handlers) {
            if (handler.callable(event)) {
                break;
            }
        }
    }

    /**
     * @brief Clears all event partitions
     */
    void clear();
private:
    template<EventLike E>
    struct EventHandler {
        std::function<bool(E&)> callable;
        i32 priority = 0;

        explicit EventHandler(std::function<bool(E&)>&& callable, i32 priority) :
            callable(std::move(callable)),
            priority(priority) {
        }
    };

    struct BaseEventPartition {
    };

    template<EventLike E>
    struct EventPartition : BaseEventPartition {
        explicit EventPartition(EventType event_type) : event_type(event_type) {
        }

        template<typename Callable>
        void add_callable(Callable callable, i32 priority) {
            event_handlers.emplace_back(callable, priority);
            // OPTIMIZE
            std::sort(
                event_handlers.begin(),
                event_handlers.end(),
                [](const EventHandler<E>& a, const EventHandler<E>& b) {
                    return a.priority > b.priority;
                }
            );
        }

        EventType event_type;
        std::vector<EventHandler<E>> event_handlers;
    };

    template<EventLike E>
    EventPartition<E>& get_or_create_partition() {
        EventType type = E::event_type;
        if (partitions_.find(type) == partitions_.end()) {
            partitions_[type] = std::make_unique<EventPartition<E>>(type);
        }
        return *static_cast<EventPartition<E>*>(partitions_[type].get());
    }

    std::unordered_map<EventType, std::unique_ptr<BaseEventPartition>> partitions_;
};

#endif
