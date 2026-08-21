#include "Input.h"

#include "Core/Application.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Soup
{

  bool Input::IsKeyPressed(KeyCode keyCode)
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance().GetWindow()->GetWindowHandle();
    int32_t state = glfwGetKey(libWindow, (uint32_t)keyCode);

    return state == GLFW_PRESS;
  }

  bool Input::IsMouseButtonPressed(MouseButton button)
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance().GetWindow()->GetWindowHandle();
    int32_t state = glfwGetMouseButton(libWindow, (uint32_t)button);

    return state == GLFW_PRESS;
  }

  glm::ivec2 Input::GetWindowSize()
  {
    glm::ivec2 dimension;

    auto libWindow = (GLFWwindow*)Application::GetInstance().GetWindow()->GetWindowHandle();
    glfwGetWindowSize(libWindow, &dimension.x, &dimension.y);

    return dimension;
  }

  glm::ivec2 Input::GetMousePosition()
  {
    auto libWindow = (GLFWwindow*)Application::GetInstance().GetWindow()->GetWindowHandle();
    glm::dvec2 position;
    glfwGetCursorPos(libWindow, &position.x, &position.y);

    glm::ivec2 mousePosition = { (int32_t)position.x, (int32_t)position.y };
    return mousePosition;
  }

}
