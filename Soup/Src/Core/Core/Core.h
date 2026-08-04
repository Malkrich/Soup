#pragma once

#if defined(SP_WINDOWS)
#define SP_DEBUG_BREAK __debugbreak()
#elif defined(SP_LINUX)
#include "signal.h"
#define SP_DEBUG_BREAK raise(SIGTRAP)
#else
#error "SP_DEUG_BREAK not supported on this platform!"
#endif

#define SP_CONCAT_INTERNAL(a, b) a##b
#define SP_CONCAT(a, b) SP_CONCAT_INTERNAL(a, b)

#define SP_BIND_EVENT_FUNCTION(function) std::bind(&function, this, std::placeholders::_1)

#define SP_STRUCT_PADDING(count) float SP_CONCAT(__padding_, __LINE__)[count] = {}

#ifdef SP_DEBUG
#define SP_ASSERT(condition, ...)                                                                                      \
  do                                                                                                                   \
  {                                                                                                                    \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
      SP_LOG_ERROR(__VA_ARGS__);                                                                                       \
      SP_DEBUG_BREAK;                                                                                                  \
    }                                                                                                                  \
  } while (false)
#else
#define SP_ASSERT(condition, ...)
#endif

#include <memory>

namespace Soup
{

  template <typename T>
  using Scope = std::unique_ptr<T>;
  template <typename T, typename... Args>
  constexpr Scope<T> CreateScope(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using Ref = std::shared_ptr<T>;
  template <typename T, typename... Args>
  constexpr Ref<T> CreateRef(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  // std add-ons
  // from boost: https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
  template <typename T>
  inline void HashCombine(size_t& seed, const T& v)
  {
    std::hash<T> hv;
    seed ^= hv(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

}
