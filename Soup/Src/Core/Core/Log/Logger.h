#pragma once

namespace spdlog
{
  class logger;
}

namespace Soup
{

  class Logger
  {
  public:
    enum class Level
    {
      Trace,
      Info,
      Warning,
      Error
    };

  public:
    Logger(const std::string& loggerName);

    void SetLevel(Level level);

    void SetPattern(const std::string& pattern);

    ///@{
    /**
     * Log a line at the specified level.
     */
    template <typename... Args>
    void PrintTrace(std::format_string<Args...> formattedString, Args&&... args)
    {
      std::string line = std::format(formattedString, std::forward<Args>(args)...);
      PrintImpl(line, Level::Trace);
    }
    template <typename... Args>
    void PrintInfo(std::format_string<Args...> formattedString, Args&&... args)
    {
      std::string line = std::format(formattedString, std::forward<Args>(args)...);
      PrintImpl(line, Level::Info);
    }
    template <typename... Args>
    void PrintWarning(std::format_string<Args...> formattedString, Args&&... args)
    {
      std::string line = std::format(formattedString, std::forward<Args>(args)...);
      PrintImpl(line, Level::Warning);
    }
    template <typename... Args>
    void PrintError(std::format_string<Args...> formattedString, Args&&... args)
    {
      std::string line = std::format(formattedString, std::forward<Args>(args)...);
      PrintImpl(line, Level::Error);
    }
    ///@}

    void BreakLine() { PrintImpl("", m_CurrentLevel); }

  private:
    void PrintImpl(const std::string& string, Level level);

  private:
    Level m_CurrentLevel;
    std::shared_ptr<spdlog::logger> m_Logger = nullptr;
  };

}
