#include "Logger.h"

// Prevents warnings from the external header
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Soup
{

  namespace Utils
  {

    static spdlog::level::level_enum LevelToSpdLevel(Logger::Level level)
    {
      switch (level)
      {
        case Logger::Level::Trace:
          return spdlog::level::trace;
        case Logger::Level::Info:
          return spdlog::level::info;
        case Logger::Level::Warning:
          return spdlog::level::warn;
        case Logger::Level::Error:
          return spdlog::level::err;
        default:
          SP_LOG_WARN("Unrecognized level {}", (uint32_t)level);
          return spdlog::level::info;
      }
    }

  }

  Logger::Logger(const std::string& loggerName)
  {
    m_Logger = spdlog::stdout_color_mt(loggerName);
    SetLevel(Level::Trace);
  }

  void Logger::SetLevel(Level level)
  {
    m_CurrentLevel = level;
    spdlog::level::level_enum spdLevel = Utils::LevelToSpdLevel(level);
    m_Logger->set_level(spdLevel);
  }

  void Logger::SetPattern(const std::string& pattern)
  {
    m_Logger->set_pattern(pattern);
  }

  void Logger::PrintImpl(const std::string& string, Level level)
  {
    switch (level)
    {
      case Level::Trace:
        m_Logger->trace("{}", string);
        break;
      case Level::Info:
        m_Logger->info("{}", string);
        break;
      case Level::Warning:
        m_Logger->warn("{}", string);
        break;
      case Level::Error:
        m_Logger->error("{}", string);
        break;
    }
  }

}
