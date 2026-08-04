#pragma once

#include "Event.h"

namespace Soup
{

  class WindowCloseEvent : public Event
  {
  public:
    WindowCloseEvent() = default;

    WRITE_EVENT_TYPE(EventType::WindowClose)
    WRITE_EVENT_CATEGORY(EventCategoryApplication)
  };

  class WindowResizeEvent : public Event
  {
  public:
    WindowResizeEvent(uint32_t width, uint32_t height)
      : m_Width(width)
      , m_Height(height)
    {
    }

    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }

    WRITE_EVENT_TYPE(EventType::WindowResize)
    WRITE_EVENT_CATEGORY(EventCategoryApplication)

  private:
    uint32_t m_Width, m_Height;
  };

}
