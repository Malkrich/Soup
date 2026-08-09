#include "LogCore.h"

#include "Utils/StringUtils.h"

#ifdef SP_WINDOWS
#include <Windows.h>
#else
#error "LogCore is not supported on this platform!"
#endif

std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& date)
{
  return os << Soup::StringUtils::DateToString(date);
}

namespace Soup
{

  namespace Utils
  {

#ifdef SP_WINDOWS
    static WORD LogColorToWindowsLogColor(LogColor color)
    {
      switch (color)
      {
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
#else
#error "LogCore.cpp: Log color conversion is not supported on this platform!"
#endif

  }

  void LogCore::SetStdCoutForegroundColor(LogColor color)
  {
#ifdef SP_WINDOWS
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = Utils::LogColorToWindowsLogColor(color);
    SetConsoleTextAttribute(hConsole, wColor);
#endif
  }

}
