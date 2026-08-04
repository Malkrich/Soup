#include "Renderer/GpuBufferStructs.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/TextureSpecifications.h"
#include "Renderer/TextureUtils.h"

#include "OpenGLError.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLPipeline.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLTextureCubemap.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Soup
{

  void OpenGLRendererAPI::Init()
  {
    // Shaders
    m_ShaderLib = CreateRef<ShaderLibrary>();
    std::filesystem::path shaderBaseDir = "./data/Shaders/";
    // Meshes
    m_ShaderLib->Load(shaderBaseDir / "PBRMesh.glsl");
    m_ShaderLib->Load(shaderBaseDir / "WireframeMesh.glsl");
    m_ShaderLib->Load(shaderBaseDir / "PBRLegoMesh.glsl");
    m_ShaderLib->Load(shaderBaseDir / "SolidLegoMesh.glsl");

    // IBL
    m_ShaderLib->Load(shaderBaseDir / "EquirectangularToCubemap.glsl");
    m_ShaderLib->Load(shaderBaseDir / "IrradianceMap.glsl");
    m_ShaderLib->Load(shaderBaseDir / "PreFilteredMap.glsl");
    m_ShaderLib->Load(shaderBaseDir / "BrdfLUTMap.glsl");
    m_ShaderLib->Load(shaderBaseDir / "Skybox.glsl");

    // Misc
    m_ShaderLib->Load(shaderBaseDir / "Light.glsl");
    m_ShaderLib->Load(shaderBaseDir / "Line.glsl");

    // Full screen quad
    const std::vector<glm::vec2> fullScreenQuadVertices = { glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f),
      glm::vec2(1.0f, 1.0f), glm::vec2(-1.0f, 1.0f) };
    const std::vector<uint32_t> fullScreenQuadIndices = { 0, 1, 2, 2, 3, 0 };
    m_FullScreenQuad = GpuMesh::Create(fullScreenQuadVertices, fullScreenQuadIndices);

    // Skybox cube mesh
    const std::vector<glm::vec3> skyboxCubeMeshVertices = { glm::vec3(-1.0f, -1.0f, -1.0f),
      glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
      glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f),
      glm::vec3(1.0f, 1.0f, 1.0f) };
    const std::vector<uint32_t> skyboxCubeMeshIndices = { 1, 0, 2, 2, 0, 3, 5, 6, 7, 7, 4, 5, 3, 0, 5, 5, 4, 3, 4, 7, 2,
      2, 3, 4, 1, 2, 7, 7, 6, 1, 0, 1, 6, 6, 5, 0 };
    m_SkyboxCubeMesh = GpuMesh::Create(skyboxCubeMeshVertices, skyboxCubeMeshIndices);

    // Cubemap camera data
    m_CubemapCaptureProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    m_CubemapCaptureViewMatrices = {
      { CubemapFace::PositiveX,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)) },
      { CubemapFace::NegativeX,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)) },
      { CubemapFace::PositiveY,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) },
      { CubemapFace::NegativeY,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)) },
      { CubemapFace::PositiveZ,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) },
      { CubemapFace::NegativeZ,
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) }
    };

#ifdef SP_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif
  }

  void OpenGLRendererAPI::BeginRenderPass(Ref<RenderPass> renderPass)
  {
    m_ActiveRenderPass = renderPass;
    m_ActiveRenderPass->Begin();
  }

  void OpenGLRendererAPI::EndRenderPass()
  {
    m_ActiveRenderPass->End();
    m_ActiveRenderPass = nullptr;
  }

  void OpenGLRendererAPI::RenderFullScreenQuad(
    Ref<Texture2D> textureOutput, Ref<Shader> fullScreenQuadShader, uint32_t width, uint32_t height)
  {
    PipelineSpecifications pipelineSpecs;
    pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float2 } };
    pipelineSpecs.Shader = fullScreenQuadShader;
    Ref<OpenGLPipeline> pipeline = CreateRef<OpenGLPipeline>(pipelineSpecs);

    FrameBufferSpecifications frameBufferSpecs;
    frameBufferSpecs.Width = width;
    frameBufferSpecs.Height = height;
    frameBufferSpecs.Attachments = { textureOutput->GetSpecifications().Format };
    frameBufferSpecs.Attachments[0].ExistingAttachment = textureOutput;
    Ref<OpenGLFrameBuffer> frameBuffer = CreateRef<OpenGLFrameBuffer>(frameBufferSpecs);

    RenderPassSpecifications renderPassSpecs;
    renderPassSpecs.Name = "FullScreenQuadPass";
    renderPassSpecs.FrameBuffer = frameBuffer;
    renderPassSpecs.Pipeline = pipeline;
    Ref<RenderPass> fullScreenQuadRenderPass = RenderPass::Create(renderPassSpecs);

    pipeline->SetInputAssembly(m_FullScreenQuad->GetGeometryVertexBuffer(), m_FullScreenQuad->GetGeometryIndexBuffer());

    BeginRenderPass(fullScreenQuadRenderPass);
    Ref<OpenGLVertexArray> vao = CreateActiveVertexArray();
    DrawIndexed(vao);
    EndRenderPass();
  }

  void OpenGLRendererAPI::RenderMesh(
    const Ref<GpuMesh>& mesh, Ref<UniformBuffer> meshDataUbo, Ref<UniformBuffer> materialDataUbo)
  {
    SP_ASSERT(std::dynamic_pointer_cast<OpenGLUniformBuffer>(meshDataUbo),
      "OpenGLRendererAPI only supports OpenGLUniformBuffer");
    std::dynamic_pointer_cast<OpenGLUniformBuffer>(meshDataUbo)->Bind();

    SP_ASSERT(std::dynamic_pointer_cast<OpenGLUniformBuffer>(materialDataUbo),
      "OpenGLRendererAPI only supports OpenGLUniformBuffer");
    std::dynamic_pointer_cast<OpenGLUniformBuffer>(materialDataUbo)->Bind();

    Ref<OpenGLPipeline> pipeline = std::dynamic_pointer_cast<OpenGLPipeline>(m_ActiveRenderPass->GetPipeline());
    SP_ASSERT(pipeline, "OpenGLRendererAPI only supports OpenGLPipeline");
    pipeline->SetInputAssembly(mesh->GetGeometryVertexBuffer(), mesh->GetGeometryIndexBuffer());

    Ref<OpenGLVertexArray> vao = CreateActiveVertexArray();
    DrawIndexed(vao);
  }

  void OpenGLRendererAPI::RenderMeshInstances(
    Ref<GpuMesh> mesh, Ref<VertexBuffer> instancesVertexBuffer, uint32_t instanceCount)
  {
    m_ActiveRenderPass->GetPipeline()->SetInputAssembly(
      mesh->GetGeometryVertexBuffer(), mesh->GetGeometryIndexBuffer(), instancesVertexBuffer);

    Ref<OpenGLVertexArray> vao = CreateActiveVertexArray();
    DrawInstances(vao, instanceCount);
  }

  void OpenGLRendererAPI::RenderLines(const std::vector<Line>& lines, const glm::vec3& color)
  {
    std::vector<glm::vec3> colors(lines.size());
    std::fill(colors.begin(), colors.end(), color);
    RenderLines(lines, colors);
  }

  void OpenGLRendererAPI::RenderLines(const std::vector<Line>& lines, const std::vector<glm::vec3>& colors)
  {
    SP_ASSERT(lines.size() == colors.size(), "Lines and color data must be the same length!");

    std::vector<LineVertex> lineVertices(lines.size() * 2);
    for (uint32_t i = 0; i < lines.size(); i++)
    {
      const Line& line = lines[i];
      const glm::vec3& color = colors[i];
      lineVertices[2 * i + 0] = LineVertex(line.P0, color);
      lineVertices[2 * i + 1] = LineVertex(line.P1, color);
    }

    uint32_t vertexCount = (uint32_t)lineVertices.size();
    Ref<VertexBuffer> linesVertexBuffer = VertexBuffer::Create(vertexCount * sizeof(LineVertex), lineVertices.data());
    m_ActiveRenderPass->GetPipeline()->SetInputAssembly(linesVertexBuffer);

    Ref<OpenGLVertexArray> linesVertexArray =
      std::dynamic_pointer_cast<OpenGLPipeline>(m_ActiveRenderPass->GetPipeline())->CreateVertexArray();

    DrawLines(linesVertexArray, vertexCount);
  }

  RendererEnvironment OpenGLRendererAPI::CreateEnvironment(
    Ref<Texture2D> equirectangularTexture, const EnvironmentCreateInfo& environmentCreateInfo)
  {
    RendererEnvironment environment;

    // Cubemap camera
    Ref<UniformBuffer> cubemapCameraUbo = nullptr;
    {
      UniformBufferSpecifications cubemapCameraUboSpecs;
      cubemapCameraUboSpecs.BlockName = "u_CubemapCamera";
      cubemapCameraUboSpecs.BindingPoint = 0;
      cubemapCameraUbo = UniformBuffer::Create(cubemapCameraUboSpecs, sizeof(CubemapCameraUbo));
    }

    // Environment creation info
    Ref<UniformBuffer> environmentCreateInfoUbo = nullptr;
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_EnvironmentCreateInfo";
      uboSpecs.BindingPoint = 1;
      environmentCreateInfoUbo = UniformBuffer::Create(uboSpecs, sizeof(EnvironmentCreateInfoUbo));

      EnvironmentCreateInfoUbo uboStruct;
      uboStruct.EnvironmentMapDimensions = environmentCreateInfo.EnvironmentMapDimXY;
      environmentCreateInfoUbo->SetData(&uboStruct);
    }

    // Pre-filtered create info
    Ref<UniformBuffer> preFilteredCreateInfosUbo = nullptr;
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_PreFilteredCreateInfoUbo";
      uboSpecs.BindingPoint = 1;
      preFilteredCreateInfosUbo = UniformBuffer::Create(uboSpecs, sizeof(PreFilteredCreateInfoUbo));
    }

    // Environment map
    {
      TextureCubemapSpecifications cubemapSpecs;
      cubemapSpecs.Format = environmentCreateInfo.CubeTexturesFormat;
      cubemapSpecs.Width = environmentCreateInfo.EnvironmentMapDimXY;
      cubemapSpecs.Height = environmentCreateInfo.EnvironmentMapDimXY;
      cubemapSpecs.MinFilter = TextureFilter::LinearMipmapLinear;
      cubemapSpecs.MagFilter = TextureFilter::Linear;
      // TODO: remove levels ? or set it automatically
      cubemapSpecs.Levels = TextureUtils::ComputeMaxLevelsFromDimensions(cubemapSpecs.Width);
      environment.EnvironmentMap = TextureCubemap::Create(cubemapSpecs);
    }

    // Irradiance map
    {
      TextureCubemapSpecifications cubemapSpecs;
      cubemapSpecs.Format = environmentCreateInfo.CubeTexturesFormat;
      cubemapSpecs.Width = environmentCreateInfo.IrradianceMapDimXY;
      cubemapSpecs.Height = environmentCreateInfo.IrradianceMapDimXY;
      environment.IrradianceMap = TextureCubemap::Create(cubemapSpecs);
    }

    // Pre Filtered map
    {
      TextureCubemapSpecifications cubemapSpecs;
      cubemapSpecs.Format = environmentCreateInfo.CubeTexturesFormat;
      cubemapSpecs.Width = environmentCreateInfo.PreFilteredMapDimXY;
      cubemapSpecs.Height = environmentCreateInfo.PreFilteredMapDimXY;
      cubemapSpecs.MinFilter = TextureFilter::LinearMipmapLinear;
      cubemapSpecs.MagFilter = TextureFilter::Linear;
      // TODO: remove levels ? or set it automatically
      cubemapSpecs.Levels = TextureUtils::ComputeMaxLevelsFromDimensions(cubemapSpecs.Width);
      environment.PreFilteredEnvMap = TextureCubemap::Create(cubemapSpecs);
    }

    // Brdf LUT map
    {
      Texture2DSpecifications textureSpecs;
      textureSpecs.Format = environmentCreateInfo.BrdfLUTTextureFormat;
      textureSpecs.WrappingModeU = TextureWrapMode::ClampToEdge;
      textureSpecs.WrappingModeV = TextureWrapMode::ClampToEdge;
      environment.BrdfLUTTexture = Texture2D::Create(textureSpecs, environmentCreateInfo.BrdfLUTMapDimXY);
    }

    // Cubemap faces render pass
    Ref<RenderPass> cubemapFacesRenderPass = nullptr;
    {
      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 } };
      pipelineSpecs.Shader = m_ShaderLib->Get("EquirectangularToCubemap");
      pipelineSpecs.DepthFunction = DepthFunctionMode::LessOrEqual;
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = environmentCreateInfo.EnvironmentMapDimXY;
      frameBufferSpecs.Height = environmentCreateInfo.EnvironmentMapDimXY;
      frameBufferSpecs.Attachments = { environmentCreateInfo.CubeTexturesFormat, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = environment.EnvironmentMap;
      frameBufferSpecs.Attachments[0].Cubemap = true;
      frameBufferSpecs.Attachments[0].Resizable = false;
      frameBufferSpecs.Attachments[1].ClearAttachemnt = true;
      Ref<FrameBuffer> cubemapFacesFrameBuffer = FrameBuffer::Create(frameBufferSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "CubemapRenderPass";
      renderPassSpecs.Pipeline = pipeline;
      renderPassSpecs.FrameBuffer = cubemapFacesFrameBuffer;
      cubemapFacesRenderPass = RenderPass::Create(renderPassSpecs);
    }

    // Equirectangular to cubemap pass
    {
      cubemapFacesRenderPass->SetInput(0, equirectangularTexture);

      RenderCubemapPass(cubemapFacesRenderPass, cubemapCameraUbo);

      // Populate environment map with environment map data
      environment.EnvironmentMap->RegenerateMipmaps();
    }

    // Irradiance map pass
    {
      cubemapFacesRenderPass->GetFrameBuffer()->SetColorAttachment(0, environment.IrradianceMap);
      cubemapFacesRenderPass->GetFrameBuffer()->Resize(environmentCreateInfo.IrradianceMapDimXY);

      cubemapFacesRenderPass->SetInput(0, environment.EnvironmentMap);
      cubemapFacesRenderPass->SetInput("u_EnvironmentCreateInfo", environmentCreateInfoUbo);
      Ref<Shader> irradianceMapShader = m_ShaderLib->Get("IrradianceMap");
      cubemapFacesRenderPass->GetPipeline()->GetSpecifications().Shader = irradianceMapShader;

      RenderCubemapPass(cubemapFacesRenderPass, cubemapCameraUbo);

      cubemapFacesRenderPass->RemoveUniformInput("u_EnvironmentCreateInfo");
    }

    // Pre-filtered cubemap pass
    {
      cubemapFacesRenderPass->GetFrameBuffer()->SetColorAttachment(0, environment.PreFilteredEnvMap);
      cubemapFacesRenderPass->GetFrameBuffer()->Resize(environmentCreateInfo.PreFilteredMapDimXY);

      Ref<Shader> preFilteredMapShader = m_ShaderLib->Get("PreFilteredMap");
      cubemapFacesRenderPass->GetPipeline()->GetSpecifications().Shader = preFilteredMapShader;

      RenderPreFilteredCreatePass(cubemapFacesRenderPass, cubemapCameraUbo, preFilteredCreateInfosUbo);
    }

    // BRDF LUT pass
    {
      Ref<Shader> brdfLUTMapShader = m_ShaderLib->Get("BrdfLUTMap");
      RenderFullScreenQuad(environment.BrdfLUTTexture, brdfLUTMapShader, environmentCreateInfo.BrdfLUTMapDimXY,
        environmentCreateInfo.BrdfLUTMapDimXY);
    }

    return environment;
  }

  void OpenGLRendererAPI::RenderCubemapPass(
    Ref<RenderPass> renderPass, Ref<UniformBuffer> cubemapCameraUbo, uint32_t level)
  {
    SP_ASSERT(std::dynamic_pointer_cast<TextureCubemap>(
                renderPass->GetFrameBuffer()->GetColorAttachmentSpecs(0).ExistingAttachment),
      "Color attachment 0 has to be a cubemap!");

    renderPass->GetPipeline()->SetInputAssembly(
      m_SkyboxCubeMesh->GetGeometryVertexBuffer(), m_SkyboxCubeMesh->GetGeometryIndexBuffer());

    BeginRenderPass(renderPass);
    Ref<OpenGLVertexArray> meshVao = CreateActiveVertexArray();
    for (const auto& [cubemapFace, viewMatrix] : m_CubemapCaptureViewMatrices)
    {
      CubemapCameraUbo cubemapCameraUboStruct;
      cubemapCameraUboStruct.ViewProjectionMatrix = m_CubemapCaptureProjectionMatrix * viewMatrix;
      cubemapCameraUbo->SetData(&cubemapCameraUboStruct);

      std::dynamic_pointer_cast<OpenGLUniformBuffer>(cubemapCameraUbo)->Bind();

      m_ActiveRenderPass->GetFrameBuffer()->AttachCubemapFace(0, cubemapFace, level);
      DrawIndexed(meshVao);
    }
    EndRenderPass();
  }

  void OpenGLRendererAPI::RenderPreFilteredCreatePass(
    Ref<RenderPass> renderPass, Ref<UniformBuffer> cubemapCameraUbo, Ref<UniformBuffer> preFilteredCreateInfoUbo)
  {
    uint32_t initialDimensions = renderPass->GetFrameBuffer()->GetWidth();
    uint32_t levelCount = TextureUtils::ComputeMaxLevelsFromDimensions(initialDimensions);
    for (uint32_t level = 0; level < levelCount; level++)
    {
      // Resize frame buffer
      uint32_t mipmapDimXY = TextureUtils::ComputeWidthOrHeightFromMipLevel(level, initialDimensions);
      renderPass->GetFrameBuffer()->Resize(mipmapDimXY);

      // Set pre-filtered create info data
      PreFilteredCreateInfoUbo preFilteredCreateInfoUboStruct;
      preFilteredCreateInfoUboStruct.Roughness = (float)level / (float)(levelCount - 1);
      preFilteredCreateInfoUbo->SetData(&preFilteredCreateInfoUboStruct);

      std::dynamic_pointer_cast<OpenGLUniformBuffer>(preFilteredCreateInfoUbo)->Bind();

      RenderCubemapPass(renderPass, cubemapCameraUbo, level);
    }
  }

  void OpenGLRendererAPI::RenderSkybox(Ref<TextureCubemap> skyboxTexture)
  {
    SP_ASSERT(std::dynamic_pointer_cast<OpenGLTextureCubemap>(skyboxTexture),
      "OpenGLRendererAPI only supports OpenGLTextureCubemap!");
    std::dynamic_pointer_cast<OpenGLTextureCubemap>(skyboxTexture)->Bind(0);

    m_ActiveRenderPass->GetPipeline()->SetInputAssembly(
      m_SkyboxCubeMesh->GetGeometryVertexBuffer(), m_SkyboxCubeMesh->GetGeometryIndexBuffer());
    Ref<OpenGLVertexArray> vao = CreateActiveVertexArray();
    DrawIndexed(vao);
  }

  Ref<OpenGLVertexArray> OpenGLRendererAPI::CreateActiveVertexArray()
  {
    SP_ASSERT(m_ActiveRenderPass, "A render pass must be active!");
    return std::dynamic_pointer_cast<OpenGLPipeline>(m_ActiveRenderPass->GetPipeline())->CreateVertexArray();
  }

  void OpenGLRendererAPI::DrawIndexed(const Ref<OpenGLVertexArray>& vertexArray)
  {
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    vertexArray->Unbind();
  }

  void OpenGLRendererAPI::DrawInstances(const Ref<OpenGLVertexArray>& vertexArray, uint32_t instanceCount)
  {
    vertexArray->Bind();
    glDrawElementsInstanced(
      GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
    vertexArray->Unbind();
  }

  void OpenGLRendererAPI::DrawLines(const Ref<OpenGLVertexArray>& vertexArray, uint32_t vertexCount)
  {
    vertexArray->Bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
    vertexArray->Unbind();
  }
}
