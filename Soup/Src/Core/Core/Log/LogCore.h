#pragma once

#include <spdlog/fmt/ostr.h>

std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& date);

namespace Soup
{

  enum class LogColor
  {
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
