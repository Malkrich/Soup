#include "CLIConsole.h"

#include "Core/Log/LogCore.h"

namespace Soup
{

  CLIConsole::CLIConsole(const std::string& consoleName)
    : m_Logger(consoleName)
  {
    m_Logger.SetLevel(Logger::Level::Info);
    m_Logger.SetPattern("%^%v%$");
  }

  std::string CLIConsole::GetInputImpl(const std::string& string)
  {
    std::string line;
    LogCore::SetStdCoutForegroundColor(LogColor::Green);
    std::cout << string && std::getline(std::cin, line);
    LogCore::SetStdCoutForegroundColor(LogColor::White);
    return line;
  }

}
