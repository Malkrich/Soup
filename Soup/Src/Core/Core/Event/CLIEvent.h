#pragma once

#include "Event.h"

namespace Soup
{

  class CLICommandEnteredEvent : public Event
  {
  public:
    CLICommandEnteredEvent(const std::string& line)
      : m_Line(line)
    {
    }

    const std::string& GetLine() const { return m_Line; }

    WRITE_EVENT_TYPE(EventType::CLICommandEnteredEvent);
    WRITE_EVENT_CATEGORY(EventCategory::EventCategoryCLI);

  public:
    std::string m_Line;
  };

}
