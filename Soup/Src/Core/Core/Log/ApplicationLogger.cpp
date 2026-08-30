#include "ApplicationLogger.h"

// Prevents warnings from the external header
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Soup
{

  void ApplicationLogger::Init()
  {
    s_Logger = spdlog::stdout_color_mt("Soup");
    s_Logger->set_pattern("%^[%T] %n %l : %v%$");
    s_Logger->set_level(spdlog::level::info);
  }

  void ApplicationLogger::Shutdown()
  {
    s_Logger = nullptr;
  }

}
