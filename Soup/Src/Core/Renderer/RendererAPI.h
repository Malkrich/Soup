#pragma once

#include "GpuMesh.h"
#include "Primitives.h"
#include "RenderPass.h"
#include "RendererMaterial.h"
#include "Shader/Shader.h"
#include "Shader/ShaderLibrary.h"
#include "Texture2D.h"
#include "TextureCubemap.h"

namespace Soup
{

  struct EnvironmentCreateInfo
  {
    TextureFormat CubeTexturesFormat = TextureFormat::RGBFloat32;
    uint32_t EnvironmentMapDimXY = 256;
    uint32_t IrradianceMapDimXY = 128;
    uint32_t PreFilteredMapDimXY = 64;

    TextureFormat BrdfLUTTextureFormat = TextureFormat::RGFloat32;
    uint32_t BrdfLUTMapDimXY = 512;

    EnvironmentCreateInfo() = default;
  };

  struct RendererEnvironment
  {
    Ref<TextureCubemap> EnvironmentMap = nullptr;
    Ref<TextureCubemap> IrradianceMap = nullptr;
    Ref<TextureCubemap> PreFilteredEnvMap = nullptr;
    Ref<Texture2D> BrdfLUTTexture = nullptr;
  };

  class RendererAPI
  {
  public:
    enum class API
    {
      None = 0,
      OpenGL,
    };

  public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual API GetAPI() const = 0;

    virtual Ref<ShaderLibrary> GetShaderLibrary() = 0;

    // Render pass scope
    virtual void BeginRenderPass(Ref<RenderPass> renderPass) = 0;
    virtual void EndRenderPass() = 0;

    // Generic
    virtual void RenderFullScreenQuad(
      Ref<Texture2D> textureOutput, Ref<Shader> fullScreenQuadShader, uint32_t width, uint32_t height) = 0;
    void RenderFullScreenQuad(Ref<Texture2D> textureOutput, Ref<Shader> fullScreenQuadShader, uint32_t dimensions);

    // Environment
    virtual RendererEnvironment CreateEnvironment(
      Ref<Texture2D> equirectangularTexture, const EnvironmentCreateInfo& environmentCreateInfo) = 0;
    virtual void RenderSkybox(Ref<TextureCubemap> cubemap) = 0;

    // Meshes
    virtual void RenderMesh(
      const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo) = 0;
    virtual void RenderMeshInstances(
      Ref<GpuMesh> mesh, Ref<VertexBuffer> instancesVertexBuffer, uint32_t instanceCount) = 0;

    // Lines
    virtual void RenderLines(const std::vector<Line>& lines, const glm::vec3& color) = 0;
    virtual void RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors) = 0;
  };

}
