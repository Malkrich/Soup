#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Soup
{

  struct RendererData
  {
    RendererAPI::API RendererAPIType = RendererAPI::API::None;
    Ref<RendererAPI> RendererAPIInstance = nullptr;
  };

  static RendererData* s_RendererData;

  void Renderer::Init(RendererAPI::API api)
  {
    s_RendererData = new RendererData();
    s_RendererData->RendererAPIType = api;
  }

  void Renderer::InitAPI()
  {
    switch (s_RendererData->RendererAPIType)
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "RendererAPI::API::None is not implemented yey!");
        break;
      case RendererAPI::API::OpenGL:
        s_RendererData->RendererAPIInstance = CreateRef<OpenGLRendererAPI>();
        break;
    }

    s_RendererData->RendererAPIInstance->Init();
  }

  void Renderer::Shutdown()
  {
    delete s_RendererData;
    s_RendererData = nullptr;
  }

  RendererAPI::API Renderer::GetCurrentAPI()
  {
    return s_RendererData->RendererAPIType;
  }

  Ref<ShaderLibrary> Renderer::GetShaderLibrary()
  {
    return s_RendererData->RendererAPIInstance->GetShaderLibrary();
  }

  void Renderer::BeginRenderPass(Ref<RenderPass> renderPass)
  {
    s_RendererData->RendererAPIInstance->BeginRenderPass(renderPass);
  }

  void Renderer::EndRenderPass()
  {
    s_RendererData->RendererAPIInstance->EndRenderPass();
  }

  RendererEnvironment Renderer::CreateEnvironment(
    Ref<Texture2D> hdriTexture, const EnvironmentCreateInfo& environmentCreateInfo)
  {
    return s_RendererData->RendererAPIInstance->CreateEnvironment(hdriTexture, environmentCreateInfo);
  }

  void Renderer::RenderSkybox(Ref<TextureCubemap> skyboxTexture)
  {
    s_RendererData->RendererAPIInstance->RenderSkybox(skyboxTexture);
  }

  void Renderer::RenderMesh(
    const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo)
  {
    s_RendererData->RendererAPIInstance->RenderMesh(mesh, meshDataUbo, materialDataUbo);
  }

  void Renderer::RenderMeshInstances(Ref<GpuMesh> mesh, Ref<VertexBuffer> instanceVertexBuffer, uint32_t instanceCount)
  {
    s_RendererData->RendererAPIInstance->RenderMeshInstances(mesh, instanceVertexBuffer, instanceCount);
  }

  void Renderer::RenderLines(const std::vector<Line>& lines, const glm::vec3& color)
  {
    s_RendererData->RendererAPIInstance->RenderLines(lines, color);
  }

  void Renderer::RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors)
  {
    s_RendererData->RendererAPIInstance->RenderLines(lines, colors);
  }

}
