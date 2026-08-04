#include "FrameBuffer.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLFrameBuffer.h"

namespace Soup
{

  Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecifications& specs)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFrameBuffer>(specs);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
