#include <catch2/catch_test_macros.hpp>
#include "framework/event_manager.hxx"
#include "input/input.hxx"
#include "input/input_event.hxx"

TEST_CASE("Input::set_mouse_position updates mouse coordinates", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Updates mouse_x and mouse_y") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(input.mouse_x() == 100);
        REQUIRE(input.mouse_y() == 200);
    }
}

TEST_CASE("Input::set_mouse_position triggers MouseMoveEvent", "[mouse][unit][event]") {
    EventManager events;
    std::optional<MouseMoveEvent> event = std::nullopt;
    events.on<MouseMoveEvent>([&event](const MouseMoveEvent& e) -> bool { event = e; return true; });
    Input input(&events);


    SECTION("Updates mouse_x and mouse_y") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(event.has_value());
        REQUIRE(event->position.x == 100);
        REQUIRE(event->position.y == 200);
    }
}
