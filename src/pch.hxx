#ifndef NOEDEN_PCH_HXX
#define NOEDEN_PCH_HXX

#ifndef NOEDEN_RELEASE
#define NOEDEN_XRAY
#define NOEDEN_LOGGING
#endif

#define NOMINMAX

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <charconv>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <source_location>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <type_traits>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GL/glew.h>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#define TOML_COMPILER_HAS_EXCEPTIONS 0
#define TOML_COMPILER_HAS_RTTI 0
#include "tomlplusplus/toml.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "stb/stb_image.h"
#include "stb/stb_perlin.h"

#include "framework/macros.hxx"
#include "framework/usings.hxx"
#include "framework/concepts.hxx"
#include "framework/event.hxx"

#include "instrumentation/profiler.hxx"

#include "log/log.hxx"
#include "lang/translator.hxx"

#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#include "framework/event_manager.hxx"
#include "framework/service_locator.hxx"
#include "framework/grid.hxx"

#include "world/world_pos.hxx"
#include "world/chunk_pos.hxx"

#include "input/input.hxx"
#include "input/input_event.hxx"

#endif
