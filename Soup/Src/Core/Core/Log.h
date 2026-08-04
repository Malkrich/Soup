#pragma once

// Prevents warnings from the external header
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Soup
{

  class Log
  {
  public:
    Log() = default;

    static void Init();

    static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }

  private:
    inline static std::shared_ptr<spdlog::logger> s_Logger = nullptr;
  };

}

#ifdef SP_DEBUG
#define SP_LOG_TRACE(...) Soup::Log::GetLogger()->trace(__VA_ARGS__)
#define SP_LOG_INFO(...) Soup::Log::GetLogger()->info(__VA_ARGS__)
#define SP_LOG_WARN(...) Soup::Log::GetLogger()->warn(__VA_ARGS__)
#define SP_LOG_ERROR(...) Soup::Log::GetLogger()->error(__VA_ARGS__)
#else
#define SP_LOG_TRACE(...)
#define SP_LOG_INFO(...)
#define SP_LOG_WARN(...)
#define SP_LOG_ERROR(...)
#endif
