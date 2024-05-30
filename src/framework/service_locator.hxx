#ifndef JARALYN_SERVICE_LOCATOR_HXX
#define JARALYN_SERVICE_LOCATOR_HXX

/**
 * @brief Service locator providing a way to register and retrieve services.
 * 
 * The service locator does not take ownership of the services registered with
 * it. It is the responsibility of the caller to ensure the lifetime of the
 * services outlives the service locator.
 * 
 * This class is thread-safe beyond the point of registering all services
 * which should happen at engine startup.
 */
class ServiceLocator {
public:
    /**
     * @brief Retrieves a service from the service locator
     * 
     * @tparam Service Type of the service to retrieve
     * @return Service* Pointer to the service instance or nullptr if not found
     */
    template<typename Service>
    Service* get() {
        auto it = service_map_.find(key<Service>());
        if (it != service_map_.end()) {
            return static_cast<Service*>(it->second);
        }
        return nullptr;
    }

    friend class Game;
private:
    /**
     * @brief Registers a service at the service locator
     * 
     * @tparam Service Type of the service to register
     * @param service Service instance
     */
    template<typename Service>
    void provide(Service* service) {
        assert(service);
        service_map_[key<Service>()] = service;
    }

    /**
     * @brief Map of service keys to service instances
     */
    std::unordered_map<u32, void*> service_map_;

    /**
     * @brief Next key to use when registering a service
     */
    u32 next_key_ = 0;

    /**
     * @brief Generates a unique key for a service type
     * 
     * @attention Not thread safe as it makes use of static initialization.
     * 
     * @tparam T Type of the service
     * @return u32 Unique key for the service type
     */
    template<typename T>
    u32 key() {
        static const u32 key = next_key_++;
        return key;
    }
};

#endif
