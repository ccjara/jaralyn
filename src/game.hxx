#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "systems.hxx"
#include "env/env_manager.hxx"

class j_game {
    friend class j_game_factory;

    std::unique_ptr<j_env_manager> env_;
    std::unique_ptr<j_systems> systems_;
    entt::registry entities_;
    entt::dispatcher dispatcher_;
public:
    j_game();
    j_systems* systems();
    entt::registry* entities();
    entt::dispatcher* events();
    
    j_env_manager& env();

    void run();
};

/**
 * @brief Globally accessible game instance pointer
 *
 * Initialized in game factory
 *
 * @see j_game_factory::run()
 */
extern j_game* game;

#endif
