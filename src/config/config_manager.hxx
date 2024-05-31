#ifndef JARALYN_CONFIG_MANAGER_HXX
#define JARALYN_CONFIG_MANAGER_HXX

#include "config/config.hxx"

class ConfigManager {
public:
    explicit ConfigManager(EventManager* events);

    /**
     * @brief Consumes the given config and updates the config source
     * 
     * @param config New configuration
     */
    void update(Config&& config);

    /**
     * @brief Returns the current configuration
     * 
     * @return const Config& Current configuration
     */
    const Config& get() const;

    /**
     * @brief Returns the current configuration
     * 
     * @return Config& Current configuration
     */
    Config& get();
private:
    /**
     * @brief The source of truth of the system configuration.
     *
     * Services may claim a copy of this config when subscribing
     * to ConfigUpdatedEvent.
     */
    Config config_;

    EventManager* events_;
};

#endif
