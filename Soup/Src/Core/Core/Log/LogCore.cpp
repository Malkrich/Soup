#include "LogCore.h"

#include "Utils/StringUtils.h"

#if defined(SP_WINDOWS)
#include <Windows.h>
#endif

std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& date)
{
  return os << Soup::StringUtils::DateToString(date);
}

namespace Soup
{

  namespace Utils
  {

#if defined(SP_WINDOWS)
    static WORD LogColorToWindowsLogColor(LogColor color)
    {
      switch (color)
      {
        case LogColor::Default:
        case LogColor::White:
          return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        case LogColor::Black:
          return 0;
        case LogColor::Red:
          return FOREGROUND_RED;
        case LogColor::Green:
          return FOREGROUND_GREEN;
        case LogColor::Blue:
          return FOREGROUND_BLUE;
        default:
          SP_ASSERT(false, "LogColor {} is not supported!", (uint32_t)color);
          return 0;
      }
    }
#elif defined(SP_LINUX)
    static std::string LogColorToLinuxColorCode(LogColor color)
    {
      switch (color)
      {
        case LogColor::Default:
          return "\033[0m";
        case LogColor::White:
          return "\033[37m";
        case LogColor::Black:
          return "\033[30m";
        case LogColor::Red:
          return "\033[31m";
        case LogColor::Green:
          return "\033[32m";
        case LogColor::Blue:
          return "\033[34m";
        default:
          SP_ASSERT(false, "LogColor {} is not supported!", (uint32_t)color);
          return 0;
      }
    }
#else
#error "LogCore.cpp: Log color conversion is not supported on this platform!"
#endif

  }

  void LogCore::SetStdCoutForegroundColor(LogColor color)
  {
#if defined(SP_WINDOWS)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = Utils::LogColorToWindowsLogColor(color);
    SetConsoleTextAttribute(hConsole, wColor);
#elif defined(SP_LINUX)
    std::cout << Utils::LogColorToLinuxColorCode(color);
#endif
  }

}
