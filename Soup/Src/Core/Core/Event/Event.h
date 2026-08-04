#pragma once

#include <cstdint>
#include <functional>

namespace Soup
{

#define WRITE_EVENT_TYPE(type)                                                                                         \
  virtual EventType GetType() const override                                                                           \
  {                                                                                                                    \
    return type;                                                                                                       \
  }                                                                                                                    \
  static EventType GetTypeStatic()                                                                                     \
  {                                                                                                                    \
    return type;                                                                                                       \
  }

#define WRITE_EVENT_CATEGORY(category)                                                                                 \
  virtual uint32_t GetCategoryFlags() const override                                                                   \
  {                                                                                                                    \
    return category;                                                                                                   \
  }

  enum class EventType
  {
    None = 0,

    WindowClose,
    WindowResize,

    MouseMoved,
    MousePressed,
    MouseReleased,
    MouseScrolled,

    KeyPressed,
    KeyReleased
  };

  enum EventCategory : uint32_t
  {
    EventCategoryNone = 0,
    EventCategoryApplication = 1 << 0,
    EventCategoryInput = 1 << 1,
    EventCategoryKeyboard = 1 << 2,
    EventCategoryMouse = 1 << 3,
    EventCategoryMouseButton = 1 << 4
  };

  class Event
  {
  public:
    virtual EventType GetType() const = 0;
    virtual uint32_t GetCategoryFlags() const = 0;

    bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

    bool IsHandled() const { return m_IsHandled; }
    void SetHandled(bool handled) { m_IsHandled = handled; }

  private:
    bool m_IsHandled = false;

    friend class EventDispatcher;
  };

  class EventDispatcher
  {
  public:
    typedef std::function<void(Event&)> EventCallbackFn;

  public:
    EventDispatcher(Event& e)
      : m_Event(e)
    {
    }

    template <typename T>
    void Dispatch(std::function<bool(T&)> callbackFunction)
    {
      if (!m_Event.m_IsHandled && m_Event.GetType() == T::GetTypeStatic())
      {
        m_Event.m_IsHandled = callbackFunction(*((T*)&m_Event));
      }
    }

  private:
    Event& m_Event;
  };

}
