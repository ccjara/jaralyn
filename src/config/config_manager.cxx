#include "config/config_manager.hxx"
#include "config/config_event.hxx"

ConfigManager::ConfigManager(EventManager* events) : events_(events) {
    assert(events_);
}

void ConfigManager::update(Config&& config) {
    Config old_config = std::move(config_);
    config_ = std::move(config);
    events_->trigger<ConfigUpdatedEvent>(old_config, config_);
}

const Config& ConfigManager::get() const {
    return config_;
}

Config& ConfigManager::get() {
    return config_;
}


