#pragma once

#include "Renderer/RendererAPI.h"

#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"

namespace Soup
{

  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    virtual void Init() override;
    virtual void Shutdown() override {}

    virtual API GetAPI() const override { return API::OpenGL; }

    virtual Ref<ShaderLibrary> GetShaderLibrary() override { return m_ShaderLib; }

    // Render pass
    virtual void BeginRenderPass(Ref<RenderPass> renderPass) override;
    virtual void EndRenderPass() override;

    // Generic
    virtual void RenderFullScreenQuad(
      Ref<Texture2D> textureOutput, Ref<Shader> fullScreenQuadShader, uint32_t width, uint32_t height) override;

    // Environment
    virtual RendererEnvironment CreateEnvironment(
      Ref<Texture2D> equirectangularTexture, const EnvironmentCreateInfo& environmentCreateInfo) override;
    virtual void RenderSkybox(Ref<TextureCubemap> skyboxTexture) override;

    // Meshes
    virtual void RenderMesh(
      const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo) override;
    virtual void RenderMeshInstances(
      Ref<GpuMesh> mesh, Ref<VertexBuffer> instancesVertexBuffer, uint32_t instanceCount) override;

    // Lines
    virtual void RenderLines(const std::vector<Line>& lines, const glm::vec3& color) override;
    virtual void RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors) override;

  private:
    // Cubemap
    void RenderCubemapPass(Ref<RenderPass> renderPass, Ref<UniformBuffer> cubemapCameraUbo, uint32_t level = 0);
    void RenderPreFilteredCreatePass(Ref<RenderPass> renderPassCubemapFace, Ref<UniformBuffer> cubemapCameraUbo,
      Ref<UniformBuffer> preFilteredCreateInfoUbo);

    // Utils
    Ref<OpenGLVertexArray> CreateActiveVertexArray();

    // Elements
    void DrawIndexed(const Ref<OpenGLVertexArray>& vertexArray);
    void DrawInstances(const Ref<OpenGLVertexArray>& vertexArray, uint32_t instanceCount);

    // Lines
    void DrawLines(const Ref<OpenGLVertexArray>& vertexArray, uint32_t vertexCount);

  private:
    // Shaders
    Ref<ShaderLibrary> m_ShaderLib = nullptr;

    // Active render pass
    Ref<RenderPass> m_ActiveRenderPass = nullptr;

    // Environment creation
    glm::mat4 m_CubemapCaptureProjectionMatrix;
    std::map<CubemapFace, glm::mat4> m_CubemapCaptureViewMatrices;

    // Geometry
    Ref<GpuMesh> m_FullScreenQuad = nullptr;
    Ref<GpuMesh> m_SkyboxCubeMesh = nullptr;
  };

}
