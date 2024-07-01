#ifndef NOEDENTEST_PCH_HXX
#define NOEDENTEST_PCH_HXX

#define NOMINMAX

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
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
#include <stack>
#include <string>
#include <source_location>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <type_traits>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>

#define TOML_COMPILER_HAS_EXCEPTIONS 0
#define TOML_COMPILER_HAS_RTTI 0
#include "tomlplusplus/toml.hpp"

#include <SDL2/SDL.h>

#include "framework/usings.hxx"
#include "framework/concepts.hxx"
#include "framework/event.hxx"

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

#endif
