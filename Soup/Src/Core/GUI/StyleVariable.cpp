#include "StyleVariable.h"

namespace Soup
{

  namespace Utils
  {

    template <typename T>
    static void PushStyleVar(ImGuiStyleVar name, StyleVarValue value)
    {
      T typedValue = std::get<T>(value);
      ImGui::PushStyleVar(name, typedValue);
    }

  }

  StyleVariable::StyleVariable(ImGuiStyleVar name, float value)
    : m_Name(name)
    , m_Type(StyleVarType::Float)
    , m_Value(value)
  {
  }
  StyleVariable::StyleVariable(ImGuiStyleVar name, ImVec2 value)
    : m_Name(name)
    , m_Type(StyleVarType::Float2)
    , m_Value(value)
  {
  }

  void StyleVariable::Push() const
  {
    switch (m_Type)
    {
      case StyleVarType::Float:
        Utils::PushStyleVar<float>(m_Name, m_Value);
        break;
      case StyleVarType::Float2:
        Utils::PushStyleVar<ImVec2>(m_Name, m_Value);
        break;
    }
  }

  void StyleVariable::Pop() const
  {
    ImGui::PopStyleVar();
  }

  void StyleVariableList::Push()
  {
    for (const StyleVariable& variable : m_Variables)
    {
      variable.Push();
    }
  }

  void StyleVariableList::Pop()
  {
    ImGui::PopStyleVar(m_Variables.size());
  }

}
