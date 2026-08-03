#pragma once

#include <glm/glm.hpp>

namespace Soup
{

  class Input
  {
  public:
    static bool IsKeyPressed(int32_t keyCode);
    static bool IsMouseButtonPressed(int32_t mouseButton);

    /** Return the current window size from GLFW */
    static glm::ivec2 GetWindowSize();
    /** Return the current mouse position from GLFW */
    static glm::ivec2 GetMousePosition();
  };

}
