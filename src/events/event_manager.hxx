#ifndef JARALYN_EVENT_MANAGER_HXX
#define JARALYN_EVENT_MANAGER_HXX

template<typename T>
concept MethodInvocable = std::is_member_function_pointer<T>::value;

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
    template<typename E, typename Fn>
    void on(Fn callable, i32 priority = 0) {
        EventPartition<E>& partition = event_partition_of<E>();
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
    template<typename E, typename Inst, MethodInvocable Fn>
    void on(Inst* instance, Fn method, i32 priority = 0) {
        EventPartition<E>& partition = event_partition_of<E>();
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
    template<typename E, typename... Args>
    void trigger(Args&& ...args) {
        E event(std::forward<Args>(args)...);
        EventPartition<E>& partition = event_partition_of<E>();
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
    using PartitionIndex = u32;

    template<typename E>
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

    template<typename E>
    struct EventPartition : BaseEventPartition {
        explicit EventPartition(PartitionIndex event_id) : event_id(event_id) {
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

        PartitionIndex event_id;
        std::vector<EventHandler<E>> event_handlers;
    };

    template<typename E>
    PartitionIndex get_or_create_partition_index() {
        static PartitionIndex index = create_partition<E>(); // invoked once per `E`
        return index;
    }

    template<typename E>
    PartitionIndex create_partition() {
        const auto index = next_partition_index_++;
        partitions_.emplace_back(new EventPartition<E>(index));
        return index;
    }

    template<typename E>
    EventPartition<E>& event_partition_of() {
        const auto partitionIndex = get_or_create_partition_index<E>();
        // if this fails there is a linkage problem (probably multiple defined partitions_)
        // due to anonymous namespaces + static linkage or similar
        assert(partitions_.size() > partitionIndex);
        return *static_cast<EventPartition<E>*>(partitions_[partitionIndex].get());
    }

    std::vector<std::unique_ptr<BaseEventPartition>> partitions_;
    PartitionIndex next_partition_index_ = 0U;
};

#endif
