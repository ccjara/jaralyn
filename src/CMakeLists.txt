include_directories(${PROJECT_SOURCE_DIR}/src)

set(DEPS 
    "../deps/imgui/imgui.cpp"
    "../deps/imgui/imgui_demo.cpp"
    "../deps/imgui/imgui_draw.cpp"
    "../deps/imgui/imgui_impl_sdl.cpp"
    "../deps/imgui/imgui_impl_opengl3.cpp"
    "../deps/imgui/imgui_tables.cpp"
    "../deps/imgui/imgui_widgets.cpp"
)

set(COMMON_SOURCES
    "action/action.cxx"
    "action/action_creator.hxx"
    "action/action_event.hxx"
    "action/action_processor.hxx"
    "action/action_queue.cxx"
    "action/action_result.hxx"
    "action/action_type.hxx"
    "action/move_action.cxx"
    "action/null_action.hxx"

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

    "catalog/catalog.cxx"

    "component/behavior.cxx"
    "component/component.cxx"
    "component/generic_component.cxx"
    "component/render.cxx"
    "component/skills.cxx"
    "component/vision/vision.cxx"
    "component/vision/fov.cxx"

    "config/config.hxx"
    "config/config_event.hxx"
    "config/config_manager.cxx"

    "entity/archetype.hxx"
    "entity/entity.cxx"
    "entity/entity_event.hxx"
    "entity/entity_factory.cxx"
    "entity/entity_manager.cxx"
    "entity/entity_reader.hxx"
    "entity/entity_writer.hxx"

    "framework/event_type.hxx"
    "framework/event_manager.cxx"
    "framework/service_locator.hxx"
    "framework/grid.hxx"
    "framework/usings.hxx"

    "gfx/color.hxx"
    "gfx/display_cell.hxx"
    "gfx/display_info.hxx"
    "gfx/display.cxx"
    "gfx/gfx_event.hxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"

    "input/input.cxx"
    "input/input_event.hxx"
    "input/input_reader.hxx"
    "input/key.hxx"
    "input/keyboard_state.cxx"
    "input/mouse_button.hxx"
    "input/mouse_state.cxx"

    "log/log.cxx"

    "math/bresenham.hxx"
    "math/rect.hxx"
    "math/vec2.hxx"

    "lang/translator.cxx"

    "platform/platform_event.hxx"
    "platform/platform.cxx"

    "scripts/api/catalog_api.cxx"
    "scripts/api/config_api.cxx"
    "scripts/api/log_api.cxx"
    "scripts/api/lua_api.hxx"
    "scripts/api/scene_api.cxx"
    "scripts/api/ui/ui_api.cxx"

    "scripts/callback_store.cxx"
    "scripts/script.cxx"
    "scripts/script_event.hxx"
    "scripts/script_util.hxx"
    "scripts/scripting.cxx"

    "tile/tile.cxx"
    "tile/tile_factory.cxx"
    "tile/tile_manager.cxx"
    "tile/tile_reader.hxx"
    "tile/tile_writer.hxx"

    "ui/ui.cxx"
    "ui/ui_node.cxx"
    "ui/ui_text.cxx"
    "ui/ui_tree.cxx"
    "ui/ui_window.cxx"

    "world/player_controller.hxx"
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

set(SOURCES ${DEPS} ${COMMON_SOURCES})

set(LIBRARIES
    SDL2::SDL2
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    GLEW::GLEW
)

add_library(jarastatic STATIC ${SOURCES})
target_precompile_headers(jarastatic PRIVATE stdafx.hxx)

target_link_libraries(jarastatic ${LIBRARIES})

add_executable(jaralyn "game/game.cxx" "main.cxx")

target_link_libraries(jaralyn PRIVATE jarastatic)

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
