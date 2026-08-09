#pragma once

#include "GpuMesh.h"
#include "Primitives.h"
#include "RenderPass.h"
#include "Renderer/Shader/ShaderLibrary.h"
#include "RendererAPI.h"
#include "RendererMaterial.h"
#include "Shader/Shader.h"
#include "Texture2D.h"
#include "TextureCubemap.h"

#include <glm/glm.hpp>

namespace Soup
{

  class Renderer
  {
  public:
    static void Init();
    static void Shutdown();

    static RendererAPI::API GetCurrentAPI();

    // Read only
    static Ref<ShaderLibrary> GetShaderLibrary();

    // Render pass
    static void BeginRenderPass(Ref<RenderPass> renderPass);
    static void EndRenderPass();

    // Environment
    static RendererEnvironment CreateEnvironment(
      Ref<Texture2D> hdriTexture, const EnvironmentCreateInfo& environmentCreateInfo);
    static void RenderSkybox(Ref<TextureCubemap> skyboxTexture);

    // Meshes
    static void RenderMesh(
      const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo);
    static void RenderMeshInstances(Ref<GpuMesh> mesh, Ref<VertexBuffer> instanceVertexBuffer, uint32_t instanceCount);

    // Lines
    static void RenderLines(const std::vector<Line>& lines, const glm::vec3& color);
    static void RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors);
  };

}
