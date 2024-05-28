#ifndef JARALYN_STDAFX_HXX
#define JARALYN_STDAFX_HXX

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
#include <type_traits>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GL/glew.h>

#include <lua5.3/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using Id = u64;

constinit static const Id null_id = 0U;

namespace fs = std::filesystem;

#include "log/log.hxx"
#include "lang/translator.hxx"

#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#include "engine/service_locator.hxx"

#endif
