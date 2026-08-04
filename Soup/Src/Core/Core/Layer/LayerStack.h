#pragma once

#include "Layer.h"

namespace Soup
{

  class LayerStack
  {
  public:
    LayerStack() = default;

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);

    std::vector<Ref<Layer>>::iterator begin() { return m_LayerStack.begin(); }
    std::vector<Ref<Layer>>::iterator end() { return m_LayerStack.end(); }

  private:
    std::vector<Ref<Layer>> m_LayerStack;
    uint32_t m_CurrentLayerPushIndex = 0;
  };

}
