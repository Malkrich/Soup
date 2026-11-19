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
      m_Logger->info(formattedString, std::forward<Args>(args)...);
    }
    void PrintLine(const std::string& message) { m_Logger->info("{}", message); }

    template <typename... Args>
    void PrintError(std::format_string<Args...> formattedString, Args&&... args)
    {
      m_Logger->error(formattedString, std::forward<Args>(args)...);
    }

    void BreakLine() { PrintLine(""); }

  private:
    std::string GetInputImpl(const std::string& string);

  private:
    std::shared_ptr<spdlog::logger> m_Logger = nullptr;
  };

}
