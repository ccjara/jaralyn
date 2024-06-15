#ifndef NOEDEN_USINGS_HXX
#define NOEDEN_USINGS_HXX

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = unsigned long long;
using i128 = long long;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using Id = u64;

constinit static const Id null_id = 0U;

namespace fs = std::filesystem;

#endif
