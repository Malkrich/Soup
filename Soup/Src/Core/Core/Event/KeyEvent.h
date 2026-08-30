#pragma once

#include "Core/KeyCodes.h"
#include "Event.h"

namespace Soup
{

  class KeyEvent : public Event
  {
  public:
    KeyCode GetKeyCode() const { return m_KeyCode; }

    WRITE_EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

  protected:
    KeyEvent(uint32_t keyCode)
      : m_KeyCode((KeyCode)keyCode)
    {
    }

  private:
    KeyCode m_KeyCode;
  };

  class KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(uint32_t keyCode)
      : KeyEvent(keyCode)
    {
    }

    WRITE_EVENT_TYPE(EventType::KeyPressed)

    std::string ToString() const override { return std::format("Key pressed: {}", (uint32_t)GetKeyCode()); }
  };

  class KeyReleasedEvent : public KeyEvent
  {
  public:
    KeyReleasedEvent(uint32_t keyCode)
      : KeyEvent(keyCode)
    {
    }

    WRITE_EVENT_TYPE(EventType::KeyReleased)

    std::string ToString() const override { return std::format("Key released: {}", (uint32_t)GetKeyCode()); }
  };

}
