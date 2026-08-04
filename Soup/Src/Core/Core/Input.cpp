#include "Input.h"

#include "Core/Application.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Soup
{

  bool Input::IsKeyPressed(int32_t keyCode)
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
    int32_t state = glfwGetKey(libWindow, keyCode);

    return state == GLFW_PRESS;
  }

  bool Input::IsMouseButtonPressed(int32_t button)
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
    int32_t state = glfwGetMouseButton(libWindow, button);

    return state == GLFW_PRESS;
  }

  glm::ivec2 Input::GetWindowSize()
  {
    glm::ivec2 dimension;

    auto libWindow = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
    glfwGetWindowSize(libWindow, &dimension.x, &dimension.y);

    return dimension;
  }

  glm::ivec2 Input::GetMousePosition()
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
    glm::dvec2 position;
    glfwGetCursorPos(libWindow, &position.x, &position.y);

    glm::ivec2 mousePosition = { (int32_t)position.x, (int32_t)position.y };
    return mousePosition;
  }

}
