find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

enable_testing()

set(SRC ${CMAKE_SOURCE_DIR}/src)

include_directories(NoEdenTest PRIVATE ${CATCH2_SOURCE_DIR}/include)
include_directories(NoEdenTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
include_directories(NoEdenTest PRIVATE ${CMAKE_SOURCE_DIR}/test)

add_executable(NoEdenTest
    framework/event_manager.cxx
    gfx/text.cxx
    input/input.cxx
    lang/translator.cxx
    lang/translation_loader.cxx
    realm/realm_manager.cxx

    ${SRC}/framework/event_manager.hxx
    ${SRC}/gfx/text.cxx
    ${SRC}/input/input.cxx
    ${SRC}/input/input_event.hxx
    ${SRC}/input/keyboard_state.cxx
    ${SRC}/input/mouse_button.hxx
    ${SRC}/input/mouse_state.cxx
    ${SRC}/lang/translation_loader.cxx
    ${SRC}/lang/translator.cxx
    ${SRC}/lang/dictionary.cxx
    ${SRC}/realm/realm_manager.cxx
    ${SRC}/realm/realm.cxx
)

target_link_libraries(NoEdenTest PRIVATE Catch2::Catch2WithMain)
target_precompile_headers(NoEdenTest PRIVATE pch.hxx)

catch_discover_tests(NoEdenTest)
