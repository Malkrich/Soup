#pragma once

#include "Core/Layer/Layer.h"

namespace Soup
{

  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& e) override;

    void SetBlockEvent(bool blockEvents) { m_BlockEvents = blockEvents; }

    void Begin();
    void End();

  private:
    bool m_BlockEvents = true;

    float m_Time = 0.0f;
  };

}
