#include "CLIConsole.h"

#include "Core/Log/LogCore.h"

// Prevents warnings from the external header
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Soup
{

  CLIConsole::CLIConsole(const std::string& consoleName)
  {
    m_Logger = spdlog::stdout_color_mt("Soup");
    m_Logger->set_level(spdlog::level::info);
    m_Logger->set_pattern("%^%v%$");
  }

  std::string CLIConsole::GetInputImpl(const std::string& string)
  {
    std::string line;
    LogCore::SetStdCoutForegroundColor(LogColor::Green);
    std::cout << string && std::getline(std::cin, line);
    LogCore::SetStdCoutForegroundColor(LogColor::Default);
    return line;
  }

}
