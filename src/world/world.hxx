#ifndef JARALYN_WORLD_HXX
#define JARALYN_WORLD_HXX

class IPlayerController;

class World {
public:
    /**
     * @brief Sets the player controller for this world
     * 
     * @param controller Player controller to bind
     */
    void set_player_controller(IPlayerController* controller);
private:
    /**
     * @brief Player controller bound to this world
     */
    IPlayerController* player_controller = nullptr;
};

#endif
