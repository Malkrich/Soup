#pragma once

#include "Core/Event/Event.h"
#include "Utils/StringUtils.h"

#include <glm/glm.hpp>

#define SP_FORMAT_CUSTOM_TYPE(type, formatString, ...)                                                                 \
  template <>                                                                                                          \
  struct fmt::formatter<type>                                                                                          \
  {                                                                                                                    \
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())                                           \
    {                                                                                                                  \
      return ctx.end();                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    template <typename FormatContext>                                                                                  \
    auto format(const type& object, FormatContext& ctx) const -> decltype(ctx.out())                                   \
    {                                                                                                                  \
      return format_to(ctx.out(), formatString, __VA_ARGS__);                                                          \
    }                                                                                                                  \
  }

SP_FORMAT_CUSTOM_TYPE(std::chrono::year_month_day, "{}", Soup::StringUtils::DateToString(object));

SP_FORMAT_CUSTOM_TYPE(glm::vec2, "[{}, {}]", object.x, object.y);
SP_FORMAT_CUSTOM_TYPE(glm::vec3, "[{}, {}, {}]", object.x, object.y, object.z);
SP_FORMAT_CUSTOM_TYPE(glm::ivec2, "[{}, {}]", object.x, object.y);
SP_FORMAT_CUSTOM_TYPE(glm::ivec3, "[{}, {}, {}]", object.x, object.y, object.z);
SP_FORMAT_CUSTOM_TYPE(glm::uvec2, "[{}, {}]", object.x, object.y);
SP_FORMAT_CUSTOM_TYPE(glm::uvec3, "[{}, {}, {}]", object.x, object.y, object.z);

SP_FORMAT_CUSTOM_TYPE(Soup::Event, "{}", object.ToString());

namespace Soup
{

  enum class LogColor
  {
    Default = 0, // Reset

    White,
    Black,

    Red,
    Green,
    Blue
  };

  class LogCore
  {
  public:
    static void SetStdCoutForegroundColor(LogColor color);
  };

}
