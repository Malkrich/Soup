#pragma once

#include "Core/MouseButtons.h"
#include "Event.h"

namespace Soup
{

  class MouseMovedEvent : public Event
  {
  public:
    MouseMovedEvent(uint32_t posX, uint32_t posY)
      : m_PosX(posX)
      , m_PosY(posY)
    {
    }

    inline uint32_t GetPosX() const { return m_PosX; }
    inline uint32_t GetPosY() const { return m_PosY; }

    WRITE_EVENT_TYPE(EventType::MouseMoved)
    WRITE_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)

  private:
    uint32_t m_PosX, m_PosY;
  };

  class MouseButtonEvent : public Event
  {
  public:
    inline MouseButton GetMouseButton() const { return m_Button; }

    WRITE_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

  protected:
    MouseButtonEvent(int button)
      : m_Button((MouseButton)button)
    {
    }

  private:
    MouseButton m_Button;
  };

  class MousePressedEvent : public MouseButtonEvent
  {
  public:
    MousePressedEvent(int32_t button)
      : MouseButtonEvent(button)
    {
    }

    WRITE_EVENT_TYPE(EventType::MousePressed)
  };

  class MouseReleasedEvent : public MouseButtonEvent
  {
  public:
    MouseReleasedEvent(int32_t button)
      : MouseButtonEvent(button)
    {
    }

    WRITE_EVENT_TYPE(EventType::MouseReleased)
  };

  class MouseScrolledEvent : public Event
  {
  public:
    MouseScrolledEvent(double offsetX, double offsetY)
      : m_OffsetX(offsetX)
      , m_OffsetY(offsetY)
    {
    }

    inline double GetOffsetX() const { return m_OffsetX; }
    inline double GetOffsetY() const { return m_OffsetY; }

    WRITE_EVENT_TYPE(EventType::MouseScrolled)
    WRITE_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)

  private:
    double m_OffsetX, m_OffsetY;
  };

}
