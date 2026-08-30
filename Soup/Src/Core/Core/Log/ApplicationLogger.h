#pragma once

#include <spdlog/spdlog.h>

namespace Soup
{

  class ApplicationLogger
  {
  public:
    static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

    static void Init();
    static void Shutdown();

  private:
    static inline std::shared_ptr<spdlog::logger> s_Logger = nullptr;
  };

}

#ifdef SP_DEBUG
#define SP_LOG_TRACE(...) Soup::ApplicationLogger::GetLogger()->trace(__VA_ARGS__)
#define SP_LOG_INFO(...) Soup::ApplicationLogger::GetLogger()->info(__VA_ARGS__)
#define SP_LOG_WARN(...) Soup::ApplicationLogger::GetLogger()->warn(__VA_ARGS__)
#define SP_LOG_ERROR(...) Soup::ApplicationLogger::GetLogger()->error(__VA_ARGS__)
#else
#define SP_LOG_TRACE(...)
#define SP_LOG_INFO(...)
#define SP_LOG_WARN(...)
#define SP_LOG_ERROR(...)
#endif
