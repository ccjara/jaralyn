#ifndef NOEDEN_EVENT_MANAGER_HXX
#define NOEDEN_EVENT_MANAGER_HXX

template<typename T>
concept MethodInvocable = std::is_member_function_pointer<T>::value;

template<typename Event, typename EventType>
concept EventLike = requires {
    { Event::event_type } -> std::convertible_to<EventType>;
};

enum class EventResult {
    /**
     * @brief Continue propagating the event to other handlers
     */
    Continue,

    /**
     * @brief Halt propagation of the event to other handlers
     */
    Halt,
};

template<typename Event>
struct Subscription;

template<typename EventType>
class EventManager {
public:
    /**
     * @brief Registers a free function to be called if the given event is triggered.
     *
     * The function must return an EventResult, indicating whether the event may propagate further to other handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Fn>
    [[nodiscard("Returned subscription instance must be stored otherwise the handler will be removed immediately after this call.")]]
    Subscription<Event> on(Fn callable, i32 priority = 0) {
        const Id id = generate_id();
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(id, callable, priority);
        return Subscription<Event>(this, id);
    }

    /**
     * @brief Registers a method to be called if the given event is triggered.
     *
     * The function must return an EventResult, indicating whether the event may propagate further to other handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Inst, MethodInvocable Fn>
    [[nodiscard("Returned subscription instance must be stored otherwise the handler will be removed immediately after this call.")]]
    Subscription<Event> on(Inst* instance, Fn method, i32 priority = 0) {
        const Id id = generate_id();
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(
            id,
            [instance, method](Event& event) -> EventResult {
                return (instance->*method)(event);
            },
            priority
        );
        return Subscription<Event>(this, id);
    }

    template<EventLike<EventType> Event>
    void off(Subscription<Event>& subscription) {
        auto partition = get_partition(Event::event_type);
        if (!partition) {
            return;
        }
        partition->remove_listener(subscription.id());
    }

    /**
     * @brief Notifies all event listeners.
     *
     * Constructs an instance of the given event type in-place and calls all
     * registered handlers in order of their registered priority.
     *
     * Short-circuits if any handler returned `true`.
     */
    template<EventLike<EventType> Event, typename... Args>
    void trigger(Args&& ...args) {
        Event event(std::forward<Args>(args)...);
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        for (auto& handler : partition.event_handlers) {
            if (handler.callable(event) == EventResult::Halt) {
                break;
            }
        }
    }

    /**
     * @brief Clears all event partitions
     */
    void clear() {
        partitions_.clear();
    }

    /**
     * @brief Returns the number of event handlers in a partition
     */
    size_t handler_count(EventType type) const {
        auto partition = get_partition(type);
        if (!partition) {
            return 0;
        }
        return partition->handler_count();
    }

    ~EventManager() {
        clear();
    }
private:
    template<EventLike<EventType> Event>
    struct EventHandler {
        Id id = 0;
        std::function<EventResult(Event&)> callable {};
        i32 priority = 0;

        explicit EventHandler(Id id, std::function<EventResult(Event&)>&& callable, i32 priority) :
            id(id),
            callable(std::move(callable)),
            priority(priority) {
        }
    };

    struct BaseEventPartition {
        virtual ~BaseEventPartition() = default;

        virtual void remove_listener(Id id) = 0;

        virtual size_t handler_count() const = 0;
    };

    template<EventLike<EventType> Event>
    struct EventPartition : BaseEventPartition {
        explicit EventPartition(EventType event_type) : event_type(event_type) {
        }

        template<typename Callable>
        void add_callable(Id id, Callable callable, i32 priority) {
            event_handlers.emplace_back(id, callable, priority);
            // OPTIMIZE
            std::sort(
                event_handlers.begin(),
                event_handlers.end(),
                [](const EventHandler<Event>& a, const EventHandler<Event>& b) {
                    return a.priority > b.priority;
                }
            );
        }

        void remove_listener(Id id) override {
            event_handlers.erase(
                std::remove_if(
                    event_handlers.begin(),
                    event_handlers.end(),
                    [id](const EventHandler<Event>& handler) -> bool {
                        return handler.id == id;
                    }
                ),
                event_handlers.end()
            );
        }

        virtual size_t handler_count() const override {
            return event_handlers.size();
        }

        EventType event_type;
        std::vector<EventHandler<Event>> event_handlers {};
    };

    template<EventLike<EventType> Event>
    EventPartition<Event>& get_or_create_partition() {
        EventType type = Event::event_type;
        auto [it, inserted] = partitions_.emplace(type, nullptr);
        if (inserted) {
            it->second = std::make_unique<EventPartition<Event>>(type);
        }
        return *static_cast<EventPartition<Event>*>(it->second.get());
    }

    BaseEventPartition* get_partition(EventType type) const {
        auto it = partitions_.find(type);
        if (it == partitions_.end()) {
            return nullptr;
        }
        return static_cast<BaseEventPartition*>(it->second.get());
    }

    Id generate_id() {
        return next_id_++;
    }

    std::unordered_map<EventType, std::unique_ptr<BaseEventPartition>> partitions_;
    std::atomic<Id> next_id_ = 1;
};

/**
 * @brief Subscription to an event manager which automatically unsubscribes when it goes out of scope.
 *
 * This class does not validate that the subscription actually exists. It should therefore be always assigned from
 * the return value of EventManager::on.
 *
 * @tparam Event Event type
 */
template<typename Event>
class Subscription {
public:
    using EventManagerType = EventManager<typename Event::EventType>;

    /**
     * @brief Constructs the subscription with the given event manager and id.
     *
     * If the event manage is null or the id is zero the subscription is considered inactive.
     *
     * @param event_manager Event manager this subscription is associated with
     * @param id Unique id of the subscription
     */
    Subscription(EventManagerType* event_manager, Id id) : event_manager_(event_manager), id_(id) {
    }

    /**
     * @brief Unsubscribes the handler from the event manager if the subscription is active.
     */
    inline void unsubscribe() {
        if (active()) {
            event_manager_->off(*this);
        }
        id_ = 0;
        event_manager_ = nullptr;
    }

    inline bool active() const {
        return event_manager_ != nullptr && id_ != 0;
    }

    /**
     * @brief Returns the unique id of the subscription
     */
    inline Id id() const {
        return id_;
    }

    /**
     * @brief Returns the associated event manager
     */
    inline EventManagerType* event_manager() const {
        return event_manager_;
    }

    /**
     * @brief Unsubscribes the handler from the event manager
     */
    ~Subscription() {
        unsubscribe();
    }

    /**
     * @brief Constructs the subscription in an inactive state
     */
    Subscription() = default;

    /**
     * @brief Move constructor, transfers the subscription to the new instance and potentially unsubscribes the old one.
     *
     * @param other Subscription to move from
     * @return New subscription instance
     */
    Subscription& operator=(Subscription&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (event_manager_ != other.event_manager_ || id_ != other.id_) {
            unsubscribe();
        }
        event_manager_ = std::exchange(other.event_manager_, nullptr);
        id_ = std::exchange(other.id_, 0);
        return *this;
    }

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;
private:
    /**
     * @brief Associated event manager
     */
    EventManagerType* event_manager_ = nullptr;

    /**
     * @brief Unique id of the subscription
     */
    Id id_ = 0;
};

#endif
