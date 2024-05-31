#ifndef JARALYN_GAME_PLAYER_CONTROLLER_HXX
#define JARALYN_GAME_PLAYER_CONTROLLER_HXX

#include "world/player_controller.hxx"

class IEntityReader;
class IActionCreator;
class KeyDownEvent;

class GamePlayerController : public IPlayerController {
public:
    explicit GamePlayerController(
        IEntityReader* entity_reader,
        IActionCreator* action_creator,
        EventManager* events
    );
private:
    bool on_key_down(KeyDownEvent& e);

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionCreator* action_creator_ = nullptr;
};

#endif