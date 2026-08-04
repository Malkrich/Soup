#pragma once

#include "Core/Event/Event.h"
#include "Core/Time.h"

namespace Soup
{

  class Layer
  {
  public:
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}

    virtual void OnEvent(Event& e) {}
    virtual void OnUpdate(DeltaTime dt) {}
    virtual void OnGuiRender() {}
  };

}
