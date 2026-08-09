#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Soup
{

  struct RendererData
  {
    RendererAPI::API RendererAPIType = RendererAPI::API::OpenGL;
    Ref<RendererAPI> RendererAPI = nullptr;
  };

  static RendererData* s_RendererData;

  void Renderer::Init()
  {
    s_RendererData = new RendererData();
    switch (s_RendererData->RendererAPIType)
    {
      case RendererAPI::API::None:
        break;
        SP_ASSERT(false, "RendererAPI::API::None is not implemented yey!");
      case RendererAPI::API::OpenGL:
        s_RendererData->RendererAPI = CreateRef<OpenGLRendererAPI>();
        break;
    }

    s_RendererData->RendererAPI->Init();
  }

  void Renderer::Shutdown()
  {
    s_RendererData->RendererAPI->Shutdown();
    delete s_RendererData;
    s_RendererData = nullptr;
  }

  RendererAPI::API Renderer::GetCurrentAPI()
  {
    return s_RendererData->RendererAPIType;
  }

  Ref<ShaderLibrary> Renderer::GetShaderLibrary()
  {
    return s_RendererData->RendererAPI->GetShaderLibrary();
  }

  void Renderer::BeginRenderPass(Ref<RenderPass> renderPass)
  {
    s_RendererData->RendererAPI->BeginRenderPass(renderPass);
  }

  void Renderer::EndRenderPass()
  {
    s_RendererData->RendererAPI->EndRenderPass();
  }

  RendererEnvironment Renderer::CreateEnvironment(
    Ref<Texture2D> hdriTexture, const EnvironmentCreateInfo& environmentCreateInfo)
  {
    return s_RendererData->RendererAPI->CreateEnvironment(hdriTexture, environmentCreateInfo);
  }

  void Renderer::RenderSkybox(Ref<TextureCubemap> skyboxTexture)
  {
    s_RendererData->RendererAPI->RenderSkybox(skyboxTexture);
  }

  void Renderer::RenderMesh(
    const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo)
  {
    s_RendererData->RendererAPI->RenderMesh(mesh, meshDataUbo, materialDataUbo);
  }

  void Renderer::RenderMeshInstances(Ref<GpuMesh> mesh, Ref<VertexBuffer> instanceVertexBuffer, uint32_t instanceCount)
  {
    s_RendererData->RendererAPI->RenderMeshInstances(mesh, instanceVertexBuffer, instanceCount);
  }

  void Renderer::RenderLines(const std::vector<Line>& lines, const glm::vec3& color)
  {
    s_RendererData->RendererAPI->RenderLines(lines, color);
  }

  void Renderer::RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors)
  {
    s_RendererData->RendererAPI->RenderLines(lines, colors);
  }

}
