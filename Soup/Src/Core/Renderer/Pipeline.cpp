#include "Pipeline.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLPipeline.h"

namespace Soup
{

  Ref<Pipeline> Pipeline::Create(const PipelineSpecifications& specs)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLPipeline>(specs);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
