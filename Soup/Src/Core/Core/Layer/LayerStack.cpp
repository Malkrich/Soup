#include "LayerStack.h"

namespace Soup
{

  void LayerStack::PushLayer(Ref<Layer> layer)
  {
    m_LayerStack.emplace(m_LayerStack.begin() + m_CurrentLayerPushIndex, layer);
    layer->OnAttach();
  }

  void LayerStack::PushOverlay(Ref<Layer> overlay)
  {
    m_LayerStack.push_back(overlay);
    overlay->OnAttach();
  }

}
