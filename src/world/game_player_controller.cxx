#include "world/entity_reader.hxx"
#include "world/game_player_controller.hxx"
#include "events/event_manager.hxx"
#include "input/input_event.hxx"
#include "entity/entity.hxx"
#include "entity/action_creator.hxx"
#include "entity/move_action.hxx"

GamePlayerController::GamePlayerController(
    IEntityReader* entity_reader,
    IActionCreator* action_creator,
    EventManager* events
) : entity_reader_(entity_reader),
    action_creator_(action_creator),
    events_(events) {
    assert(events_);

    events_->on<KeyDownEvent>(this, &GamePlayerController::on_key_down);
}

bool GamePlayerController::on_key_down(KeyDownEvent& e) {
    Entity* player = entity_reader_->player();
    if (!player) {
        return false;
    }
    auto move_relative = [=](Vec2<i32> direction) {
        // TODO: player action integration in queue and controller + world
        auto res = action_creator_->create_action(ActionType::Move, *player, true);
        if (res.action != nullptr) {
             static_cast<MoveAction*>(res.action)->destination = player->position + direction;
        }
    };
    switch (e.key) {
        case Key::W:
            move_relative(Vec2<i32> { 0, -1 });
            return true;
        case Key::A:
            move_relative(Vec2<i32> { -1, 0 });
            return true;
        case Key::S:
            move_relative(Vec2<i32> { 0, 1 });
            return true;
        case Key::D:
            move_relative(Vec2<i32> { 1, 0 });
            return true;
        default:
            return false;
    }
}