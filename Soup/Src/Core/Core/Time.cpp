#include "Time.h"

#include <GLFW/glfw3.h>

namespace Soup
{

  float Time::GetTime()
  {
    return (float)glfwGetTime();
  }

}
