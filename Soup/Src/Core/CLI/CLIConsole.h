#pragma once

namespace Soup
{

  class CLIConsole
  {
  public:
    CLIConsole(const std::string& consoleName);

    template <typename... Args>
    std::string GetInput(std::format_string<Args...> formattedString, Args&&... args)
    {
      std::string prefix = std::format(formattedString, std::forward<Args>(args)...);
      return GetInputImpl(prefix);
    }

    template <typename... Args>
    void PrintLine(std::format_string<Args...> formattedString, Args&&... args)
    {
      m_Logger.PrintInfo(formattedString, std::forward<Args>(args)...);
    }
    void PrintLine(const std::string& message) { m_Logger.PrintInfo("{}", message); }

    template <typename... Args>
    void PrintError(std::format_string<Args...> formattedString, Args&&... args)
    {
      m_Logger.PrintError(formattedString, std::forward<Args>(args)...);
    }

    void BreakLine() { m_Logger.BreakLine(); }

  private:
    std::string GetInputImpl(const std::string& string);

  private:
    Logger m_Logger;
  };

}
