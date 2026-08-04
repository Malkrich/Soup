#include "Log.h"

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Soup
{

  void Log::Init()
  {
    s_Logger = spdlog::stdout_color_mt("Soup");
    spdlog::set_level(spdlog::level::trace);
    s_Logger->set_pattern("%^[%T] %n %l : %v%$");
  }

}
