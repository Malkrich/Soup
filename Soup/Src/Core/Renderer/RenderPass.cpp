#include "RenderPass.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLRenderPass.h"

namespace Soup
{

  Ref<RenderPass> RenderPass::Create(const RenderPassSpecifications& specs)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLRenderPass>(specs);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
