include_directories(${PROJECT_SOURCE_DIR}/src) 

add_executable(
    jaralyn
    "../deps/imgui/imgui.cpp"
    "../deps/imgui/imgui_demo.cpp"
    "../deps/imgui/imgui_draw.cpp"
    "../deps/imgui/imgui_impl_sdl.cpp"
    "../deps/imgui/imgui_impl_opengl3.cpp"
    "../deps/imgui/imgui_tables.cpp"
    "../deps/imgui/imgui_widgets.cpp"

    "ai/ai_context.cxx"
    "ai/ai_condition.cxx"
    "ai/ai_closest_entity.cxx"
    "ai/ai_node.cxx"
    "ai/ai_selector.cxx"
    "ai/ai_sequence.cxx"
    "ai/ai_script.cxx"
    "ai/ai_walk.cxx"
    "ai/blackboard.cxx"
    "ai/generic_ai_node.hxx"

    "engine/service_locator.hxx"

    "entity/action.cxx"
    "entity/action_creator.hxx"
    "entity/action_event.hxx"
    "entity/action_processor.hxx"
    "entity/action_queue.cxx"
    "entity/action_result.hxx"
    "entity/action_type.hxx"
    "entity/archetype.hxx"
    "entity/catalog.cxx"
    "entity/components/behavior.cxx"
    "entity/components/component.cxx"
    "entity/components/generic_component.cxx"
    "entity/components/render.cxx"
    "entity/components/skills.cxx"
    "entity/components/vision.cxx"
    "entity/display_info.hxx"
    "entity/entity.cxx"
    "entity/entity_factory.cxx"
    "entity/entity_event.hxx"
    "entity/fov.cxx"
    "entity/move_action.cxx"
    "entity/null_action.hxx"

    "events/event_manager.cxx"

    "game/config.hxx"
    "game/game.cxx"
    "game/platform_event.hxx"
    "game/window.cxx"

    "gfx/display_cell.hxx"
    "gfx/color.hxx"
    "gfx/display.cxx"
    "gfx/gfx_event.hxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"

    "grid.hxx"

    "log/log.cxx"

    "main.cxx"

    "math/bresenham.hxx"
    "math/rect.hxx"
    "math/vec2.hxx"

    "input/input.cxx"
    "input/input_event.hxx"
    "input/key.hxx"
    "input/keyboard_state.cxx"
    "input/mouse_button.hxx"
    "input/mouse_state.cxx"

    "lang/translator.cxx"

    "scene/scene.cxx"
    "scene/tile.cxx"
    "scene/tile_builder.cxx"

    "scripts/api/catalog_api.cxx"
    "scripts/api/log_api.cxx"
    "scripts/api/lua_api.hxx"
    "scripts/api/scene_api.cxx"
    "scripts/api/ui/ui_api.cxx"

    "scripts/callback_store.cxx"
    "scripts/script.cxx"
    "scripts/script_event.hxx"
    "scripts/script_util.hxx"
    "scripts/scripting.cxx"

    "ui/ui.cxx"
    "ui/ui_node.cxx"
    "ui/ui_text.cxx"
    "ui/ui_tree.cxx"
    "ui/ui_window.cxx"

    "world/entity_reader.hxx"
    "world/entity_writer.hxx"
    "world/player_controller.hxx"
    "world/tile_reader.hxx"
    "world/world.cxx"
    "world/game_player_controller.cxx"

    "xray/log_xray.cxx"
    "xray/scene_xray.cxx"
    "xray/script_xray.cxx"
    "xray/ui_xray.cxx"
    "xray/xray.cxx"
    "xray/xray_event.hxx"
    "xray/xray_interface.hxx"
    "xray/xray_style.hxx"
)

target_link_libraries(jaralyn ${LUA_LIBRARIES})
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)

# copy script sources to runtime dir
add_custom_command(
    TARGET jaralyn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/scripts/lua
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
)

# also assets
add_custom_command(
    TARGET jaralyn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)

# language dictionaries
add_custom_command(
    TARGET jaralyn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/lang/dictionaries
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dictionaries
)
