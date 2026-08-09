#include "ApplicationLogger.h"

namespace Soup
{

  void ApplicationLogger::Init()
  {
    s_AppLogger = new Logger("PalMe Core");
    s_AppLogger->SetPattern("%^[%T] %n %l : %v%$");
    s_AppLogger->SetLevel(Logger::Level::Info);
  }

  void ApplicationLogger::Shutdown()
  {
    delete s_AppLogger;
    s_AppLogger = nullptr;
  }

}
