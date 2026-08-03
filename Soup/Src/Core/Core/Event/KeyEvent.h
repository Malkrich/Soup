#pragma once

#include "Event.h"

namespace Soup
{

  class KeyEvent : public Event
  {
  public:
    uint32_t GetKeyCode() const { return m_KeyCode; }

    WRITE_EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

  protected:
    KeyEvent(uint32_t keyCode)
      : m_KeyCode(keyCode)
    {
    }

  private:
    uint32_t m_KeyCode;
  };

  class KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(uint32_t keyCode)
      : KeyEvent(keyCode)
    {
    }

    WRITE_EVENT_TYPE(EventType::KeyPressed)
  };

  class KeyReleasedEvent : public KeyEvent
  {
  public:
    KeyReleasedEvent(uint32_t keyCode)
      : KeyEvent(keyCode)
    {
    }

    WRITE_EVENT_TYPE(EventType::KeyReleased)
  };

}
