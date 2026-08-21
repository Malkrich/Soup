#pragma once

#include "KeyCodes.h"
#include "MouseButtons.h"

#include <glm/glm.hpp>

namespace Soup
{

  class Input
  {
  public:
    static bool IsKeyPressed(KeyCode keyCode);
    static bool IsMouseButtonPressed(MouseButton mouseButton);

    /** Return the current window size from GLFW */
    static glm::ivec2 GetWindowSize();
    /** Return the current mouse position from GLFW */
    static glm::ivec2 GetMousePosition();
  };

}
