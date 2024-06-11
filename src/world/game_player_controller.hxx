#ifndef NOEDEN_GAME_PLAYER_CONTROLLER_HXX
#define NOEDEN_GAME_PLAYER_CONTROLLER_HXX

#include "world/player_controller.hxx"
#include "action/action_event.hxx"
#include "input/input_event.hxx"

struct Events;
class IEntityReader;
class IActionCreator;

class GamePlayerController : public IPlayerController {
public:
    explicit GamePlayerController(
        IEntityReader* entity_reader,
        IActionCreator* action_creator,
        Events* events
    );
private:
    EventResult on_key_down(KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;

    /**
     * Attempts to schedule a move action for the player
     * @param direction Direction to move in
     * @return true if the action was successfully scheduled
     */
    bool move_relative(const WorldPos& direction);

    Events* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionCreator* action_creator_ = nullptr;

};

#endif