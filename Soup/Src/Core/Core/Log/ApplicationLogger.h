#pragma once

#include "Logger.h"

namespace Soup
{

  class ApplicationLogger
  {
  public:
    static Logger& GetLogger() { return *s_AppLogger; }

    static void Init();
    static void Shutdown();

  private:
    static inline Logger* s_AppLogger = nullptr;
  };

}

#ifdef SP_DEBUG
#define SP_LOG_TRACE(...) Soup::ApplicationLogger::GetLogger().PrintTrace(__VA_ARGS__)
#define SP_LOG_INFO(...) Soup::ApplicationLogger::GetLogger().PrintInfo(__VA_ARGS__)
#define SP_LOG_WARN(...) Soup::ApplicationLogger::GetLogger().PrintWarning(__VA_ARGS__)
#define SP_LOG_ERROR(...) Soup::ApplicationLogger::GetLogger().PrintError(__VA_ARGS__)
#else
#define SP_LOG_TRACE(...)
#define SP_LOG_INFO(...)
#define SP_LOG_WARN(...)
#define SP_LOG_ERROR(...)
#endif
