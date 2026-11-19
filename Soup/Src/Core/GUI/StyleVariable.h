#pragma once

#include <imgui.h>

namespace Soup
{

  enum class StyleVarType
  {
    Float,
    Float2
  };

  using StyleVarValue = std::variant<float, ImVec2>;

  /**
   * @brief Abstraction around style variables.
   * This class is a helper for GUI graphics styling.
   */
  class StyleVariable
  {
  public:
    StyleVariable(ImGuiStyleVar name, float value);
    StyleVariable(ImGuiStyleVar name, ImVec2 value);

    /**
     * Make the style var active.
     */
    void Push() const;

    /**
     * Remove the style var from the stack.
     */
    void Pop() const;

  private:
    ImGuiStyleVar m_Name;
    StyleVarType m_Type;
    StyleVarValue m_Value;
  };

  /**
   * @brief Handle a list of style variable.
   * In this context, all variables are pushed and popped at the same time.
   */
  class StyleVariableList
  {
  public:
    StyleVariableList() = default;

    /**
     * Insert a style variable in the variable list.
     */
    template <typename T>
    void Add(ImGuiStyleVar name, T value)
    {
      m_Variables.emplace_back(name, value);
    }

    /**
     * Make all variables active.
     */
    void Push();

    /**
     * Remove all registered variables from the stack.
     */
    void Pop();

  private:
    std::vector<StyleVariable> m_Variables;
  };

  /**
   * @brief Make a style variable active within a scope.
   * Once the class is destroyed the style var is automatically popped from the stack.
   */
  template <typename T>
  class ScopedStyleVar
  {
  public:
    ScopedStyleVar(ImGuiStyleVar name, T value)
      : m_StyleVar(name, value)
    {
      m_StyleVar.Push();
    }
    ~ScopedStyleVar() { m_StyleVar.Pop(); }

  private:
    StyleVariable m_StyleVar;
  };

}
