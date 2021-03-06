#ifndef JARALYN_STDAFX_HXX
#define JARALYN_STDAFX_HXX

#define NOMINMAX

#include <algorithm>
#include <atomic>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

#include <GL/glew.h>

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#define SDL_MAIN_HANDLED
#define ELPP_NO_DEFAULT_LOG_FILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <entt/entt.hxx>
#include <easyloggingpp/easyloggingpp.hxx>

namespace fs = std::filesystem;

#include "identity.hxx"

#include "math/vec2.hxx"
#include "math/bresenham.hxx"
#include "math/rect.hxx"

#include "gfx/color.hxx"

#endif
