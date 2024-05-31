#ifndef JARATEST_PCH_HXX
#define JARATEST_PCH_HXX

#define NOMINMAX

#include <algorithm>
#include <any>
#include <array>
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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GL/glew.h>

#include <lua5.3/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "framework/usings.hxx"
#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"
#include "gfx/color.hxx"
#include "framework/event_manager.hxx"
#include "framework/service_locator.hxx"
#include "framework/grid.hxx"


#endif
