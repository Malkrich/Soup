#include "SceneRenderer.h"

#include "Asset/AssetManager.h"
#include "Geometry/Mesh.h"
#include "Geometry/MeshFactory.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "VertexBuffer.h"

namespace Soup
{
  SceneRenderer::SceneRenderer(uint32_t viewportWidth, uint32_t viewportHeight)
  {
    Init(viewportWidth, viewportHeight);
  }

  void SceneRenderer::Init(uint32_t viewportWidth, uint32_t viewportHeight)
  {
    // Rendered image
    {
      Texture2DSpecifications textureSpecs;
      textureSpecs.Width = viewportWidth;
      textureSpecs.Height = viewportHeight;
      textureSpecs.Format = TextureFormat::RGBA;
      m_RenderedImage = Texture2D::Create(textureSpecs);
    }

    // Depth attachment
    Ref<Texture2D> depthAttachment = nullptr;
    {
      Texture2DSpecifications textureSpecs;
      textureSpecs.Width = viewportWidth;
      textureSpecs.Height = viewportHeight;
      textureSpecs.Format = TextureFormat::Depth;
      depthAttachment = Texture2D::Create(textureSpecs);
    }

    // Entity id attachment
    {
      Texture2DSpecifications entityIdImageSpecs;
      entityIdImageSpecs.Width = viewportWidth;
      entityIdImageSpecs.Height = viewportHeight;
      entityIdImageSpecs.Format = TextureFormat::R8;
      m_EntityIdImage = Texture2D::Create(entityIdImageSpecs);
    }

    // Camera
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_CameraData";
      uboSpecs.BindingPoint = 0;
      m_CameraDataUbo = UniformBuffer::Create(uboSpecs, sizeof(CameraDataUbo));
    }

    // Mesh
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_MeshData";
      uboSpecs.BindingPoint = 2;
      m_MeshDataUbo = UniformBuffer::Create(uboSpecs, sizeof(MeshDataUbo));
    }

    // Wireframe Material
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_WireframeMaterialData";
      uboSpecs.BindingPoint = 3;
      m_WireframeMaterialUbo = UniformBuffer::Create(uboSpecs, sizeof(WireframeMaterialUbo));
    }

    // Solid Material
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_SolidMaterialData";
      uboSpecs.BindingPoint = 3;
      m_SolidMaterialUbo = UniformBuffer::Create(uboSpecs, sizeof(SolidMaterialUbo));
    }

    // PBR Material
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_PBRMaterialData";
      uboSpecs.BindingPoint = 3;
      m_PBRMaterialUbo = UniformBuffer::Create(uboSpecs, sizeof(PBRMaterialUbo));
    }

    // Skybox
    {
      UniformBufferSpecifications uboSpecs;
      uboSpecs.BlockName = "u_SkyboxData";
      uboSpecs.BindingPoint = 1;
      m_SkyboxDataUbo = UniformBuffer::Create(uboSpecs, sizeof(SkyboxDataUbo));
    }

    // Lights data
    {
      ShaderStorageBufferSpecifications ssboSpecs;
      ssboSpecs.BlockName = "s_LightsData";
      ssboSpecs.BindingPoint = 1;
      m_LightsDataSsbo = ShaderStorageBuffer::Create(ssboSpecs, sizeof(LightsDataSsbo));
    }

    // Light mesh
    {
      std::vector<LightVertex> lightMeshVertices;
      std::vector<TriangleFace> lightMeshIndices;
      {
        Ref<Mesh> lightSourceMesh = MeshFactory::CreateCube();
        lightSourceMesh->Scale(0.01f);

        // Vertices
        const auto& sourceMeshVertices = lightSourceMesh->GetVertices();
        lightMeshVertices.reserve(sourceMeshVertices.size());
        for (const auto& sourceVertex : sourceMeshVertices)
          lightMeshVertices.emplace_back(sourceVertex.Position);

        // Indices
        lightMeshIndices = lightSourceMesh->GetConnectivities();
      }
      m_LightMesh = GpuMesh::Create(lightMeshVertices, lightMeshIndices);
    }

    m_LightInstanceVbo = VertexBuffer::Create();
    m_InstanceVbo = VertexBuffer::Create();

    const glm::vec4 backgroundClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };

    // Skybox render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_RenderedImage;
      frameBufferSpecs.Attachments[0].ClearAttachment = true;
      frameBufferSpecs.Attachments[0].ClearColor = backgroundClearColor;
      frameBufferSpecs.Attachments[1].ExistingAttachment = depthAttachment;
      frameBufferSpecs.Attachments[1].ClearAttachment = true;
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = {
        { "a_Position", BufferElementType::Float3 },
      };
      pipelineSpecs.CubemapSeamless = true;
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("Skybox");
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "SkyboxRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_SkyboxRenderPass = RenderPass::Create(renderPassSpecs);

      m_SkyboxRenderPass->SetInput("u_CameraData", m_CameraDataUbo);
      m_SkyboxRenderPass->SetInput("u_SkyboxData", m_SkyboxDataUbo);
    }

    // Entity outline render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_RenderedImage;
      frameBufferSpecs.Attachments[0].ClearAttachment = true;
      frameBufferSpecs.Attachments[0].ClearColor = backgroundClearColor;
      frameBufferSpecs.Attachments[1].ClearAttachment = true;
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 },
        { "a_Normal", BufferElementType::Float3 } };
      pipelineSpecs.Polygon = PolygonMode::Line;
      pipelineSpecs.FaceCulling = FaceCullingMode::Front;
      pipelineSpecs.LineWidth = m_RendererSettings.OutlineWidth;
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("WireframeMesh");
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "EntityOutlineRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_EntityOutlineRenderPass = RenderPass::Create(renderPassSpecs);

      // Outline
      m_EntityOutlineRenderPass->SetInput("u_CameraData", m_CameraDataUbo);
    }

    // PBR render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::R8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_EntityOutlineRenderPass->GetOutput(0);
      frameBufferSpecs.Attachments[1].ExistingAttachment = m_EntityIdImage;
      frameBufferSpecs.Attachments[1].ClearColor = glm::vec4(-1.0f);
      frameBufferSpecs.Attachments[1].ClearAttachment = true;
      frameBufferSpecs.Attachments[2].ExistingAttachment = depthAttachment;
      frameBufferSpecs.Attachments[2].ClearAttachment = true;
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 },
        { "a_Normal", BufferElementType::Float3 } };
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("PBRMesh");
      pipelineSpecs.CubemapSeamless = true;
      pipelineSpecs.EnableFaceCulling = false;
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "MeshRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_MeshRenderPass = RenderPass::Create(renderPassSpecs);

      // Camera
      m_MeshRenderPass->SetInput("u_CameraData", m_CameraDataUbo);
      m_MeshRenderPass->SetInput("s_LightsData", m_LightsDataSsbo);
    }

    // Instance render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::R8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_MeshRenderPass->GetOutput(0);
      frameBufferSpecs.Attachments[1].ExistingAttachment = m_MeshRenderPass->GetOutput(1);
      frameBufferSpecs.Attachments[2].ExistingAttachment = m_MeshRenderPass->GetDepthOutput();
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 },
        { "a_Normal", BufferElementType::Float3 } };
      pipelineSpecs.InstanceLayout = { { "a_EntityID", BufferElementType::Int },
        { "a_Albedo", BufferElementType::Float4 }, { "a_Roughness", BufferElementType::Float },
        { "a_Metalness", BufferElementType::Float }, { "a_Transform", BufferElementType::Mat4 } };
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("SolidInstanceMesh");
      pipelineSpecs.CubemapSeamless = true;
      pipelineSpecs.EnableFaceCulling = false;
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "InstanceRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_InstanceRenderPass = RenderPass::Create(renderPassSpecs);

      // Camera
      m_InstanceRenderPass->SetInput("u_CameraData", m_CameraDataUbo);
      m_InstanceRenderPass->SetInput("s_LightsData", m_LightsDataSsbo);
    }

    // Light render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::R8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_InstanceRenderPass->GetOutput(0);
      frameBufferSpecs.Attachments[1].ExistingAttachment = m_InstanceRenderPass->GetOutput(1);
      frameBufferSpecs.Attachments[2].ExistingAttachment = m_InstanceRenderPass->GetDepthOutput();
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 } };
      pipelineSpecs.InstanceLayout = { { "a_EntityID", BufferElementType::Int } };
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("Light");
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "LightsRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_LightsRenderPass = RenderPass::Create(renderPassSpecs);
    }

    // Lines render pass
    {
      FrameBufferSpecifications frameBufferSpecs;
      frameBufferSpecs.Width = viewportWidth;
      frameBufferSpecs.Height = viewportHeight;
      frameBufferSpecs.Attachments = { TextureFormat::RGBA8, TextureFormat::Depth };
      frameBufferSpecs.Attachments[0].ExistingAttachment = m_LightsRenderPass->GetOutput(0);
      frameBufferSpecs.Attachments[1].ExistingAttachment = m_LightsRenderPass->GetDepthOutput();
      Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(frameBufferSpecs);

      PipelineSpecifications pipelineSpecs;
      pipelineSpecs.GeometryLayout = { { "a_Position", BufferElementType::Float3 },
        { "a_Color", BufferElementType::Float3 } };
      pipelineSpecs.ShaderProgram = Renderer::GetShaderLibrary()->Get("Line");
      Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpecs);

      RenderPassSpecifications renderPassSpecs;
      renderPassSpecs.Name = "LinesRenderPass";
      renderPassSpecs.FrameBufferObject = frameBuffer;
      renderPassSpecs.PipelineObject = pipeline;
      m_LinesRenderPass = RenderPass::Create(renderPassSpecs);

      // Camera
      m_LinesRenderPass->SetInput("u_CameraData", m_CameraDataUbo);
    }

    // Lines
    m_OriginLines = { Line({ 0.0f, 0.0f, 0.0f }, { 0.1f, 0.0f, 0.0f }),
      Line({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.0f }), Line({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.1f }) };

    // Origin
    m_OriginLineColors = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
    SP_ASSERT(
      m_OriginLines.size() == m_OriginLineColors.size(), "OriginLines and OriginLineColors must be the same length!");

    // Grid
    m_GridLines = GenerateGrid(m_RendererSettings.GridBound, m_RendererSettings.GridStep);
  }

  void SceneRenderer::ResizeViewport(uint32_t width, uint32_t height)
  {
    m_EntityOutlineRenderPass->GetFrameBuffer()->Resize(width, height);
    m_MeshRenderPass->GetFrameBuffer()->Resize(width, height);
    m_InstanceRenderPass->GetFrameBuffer()->Resize(width, height);
    m_LightsRenderPass->GetFrameBuffer()->Resize(width, height);
    m_LinesRenderPass->GetFrameBuffer()->Resize(width, height);
    m_SkyboxRenderPass->GetFrameBuffer()->Resize(width, height);
  }

  void SceneRenderer::Render(Ref<Scene> scene)
  {
    size_t lightCount = scene->GetEntityCount<TransformComponent, LightComponent>();
    SceneLightsData lightsData;
    lightsData.PointLights.reserve(lightCount);
    lightsData.PointLightIDs.reserve(lightCount);
    scene->EntityView<TransformComponent, LightComponent>(
      [&](Entity entity)
      {
        uint32_t entityId = (uint32_t)entity;
        const glm::vec3& color = entity.GetComponent<LightComponent>().Color;
        const glm::vec3& position = entity.GetComponent<TransformComponent>().Translation;

        lightsData.PointLights.emplace_back(position, color);
        lightsData.PointLightIDs.emplace_back(entityId);
      });

    Ref<Camera> camera = scene->GetActiveCamera();
    if (camera->GetType() == CameraType::Perspective)
    {
      const PerspectiveCamera& perspectiveCamera = *std::dynamic_pointer_cast<PerspectiveCamera>(camera);
      SetSceneEnvironment(perspectiveCamera, lightsData);
    }

    scene->EntityView<TransformComponent, MeshComponent, MaterialComponent>(
      [&](Entity entity)
      {
        uint32_t entityId = (uint32_t)entity;
        const MeshComponent& mesh = entity.GetComponent<MeshComponent>();
        const TransformComponent& transform = entity.GetComponent<TransformComponent>();
        const MaterialComponent& material = entity.GetComponent<MaterialComponent>();
        SubmitMesh(mesh, transform, material, entityId);
      });

    scene->EntityView<TransformComponent, MeshInstanceComponent, MaterialComponent>(
      [&](Entity entity)
      {
        uint32_t entityId = (uint32_t)entity;
        const MeshInstanceComponent& meshInstance = entity.GetComponent<MeshInstanceComponent>();
        const TransformComponent& transform = entity.GetComponent<TransformComponent>();
        const MaterialComponent& material = entity.GetComponent<MaterialComponent>();
        SubmitMeshInstance(meshInstance, transform, material, entityId);
      });

    Render();
  }

  void SceneRenderer::SetSceneEnvironment(const PerspectiveCamera& camera, const SceneLightsData& lightsData)
  {
    // Camera
    m_cameraDataUboStruct.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    m_cameraDataUboStruct.View = camera.GetViewMatrix();
    m_cameraDataUboStruct.Projection = camera.GetProjectionMatrix();
    m_cameraDataUboStruct.Position = camera.GetPosition();
    m_CameraDataUbo->SetData(&m_cameraDataUboStruct);

    // Point Lights
    m_LightInstancesStruct = lightsData.PointLightIDs;
    if (m_LightsDataSsboStruct.PointLights.size() != lightsData.PointLights.size())
    {
      m_LightsDataSsboStruct.PointLights.resize(lightsData.PointLights.size());
    }
    for (size_t i = 0; i < lightsData.PointLights.size(); i++)
    {
      const PointLight& pointLight = lightsData.PointLights[i];
      m_LightsDataSsboStruct.PointLights[i] = LightsDataSsbo::PointLightElement(pointLight.Position, pointLight.Color);
    }
    Buffer lightsDataSsboBuffer = m_LightsDataSsboStruct.CreateBuffer();
    if (lightsDataSsboBuffer.Size != m_LightsDataSsbo->GetSize())
      m_LightsDataSsbo->Resize(lightsDataSsboBuffer.Size);
    m_LightsDataSsbo->SetData(lightsDataSsboBuffer.Data);
    lightsDataSsboBuffer.Release();

    // Skybox
    m_SkyboxDataUboStruct.MipFactor = m_RendererSettings.SkyboxMipFactor;
    m_SkyboxDataUbo->SetData(&m_SkyboxDataUboStruct);

    // Outline
    m_EntityOutlineRenderPass->GetPipeline()->GetSpecifications().LineWidth = m_RendererSettings.OutlineWidth;
  }

  void SceneRenderer::CreateEnvironment(Ref<Texture2D> hdriTexture, const EnvironmentCreateInfo& environmentCreateInfo)
  {
    m_Environment = Renderer::CreateEnvironment(hdriTexture, environmentCreateInfo);

    m_MeshRenderPass->SetInput(0, m_Environment.IrradianceMap);
    m_MeshRenderPass->SetInput(1, m_Environment.PreFilteredEnvMap);
    m_MeshRenderPass->SetInput(2, m_Environment.BrdfLUTTexture);

    m_InstanceRenderPass->SetInput(0, m_Environment.IrradianceMap);
    m_InstanceRenderPass->SetInput(1, m_Environment.PreFilteredEnvMap);
    m_InstanceRenderPass->SetInput(2, m_Environment.BrdfLUTTexture);
  }

  void SceneRenderer::SubmitMeshInstance(const MeshInstanceComponent& meshInstance, const TransformComponent& transform,
    const MaterialComponent& material, uint32_t entityID)
  {
    std::vector<InstanceVertex>& instances = m_MeshInstanceSubmissions[meshInstance.MeshHandle];

    InstanceVertex instanceVertex;
    instanceVertex.EntityID = entityID;
    instanceVertex.Albedo = material.Material.Albedo;
    instanceVertex.Metalness = material.Material.Metalness;
    instanceVertex.Roughness = material.Material.Roughness;
    instanceVertex.Transform = transform.GetTransform();
    instances.push_back(instanceVertex);

    AddMeshSource(meshInstance.MeshHandle);
  }

  void SceneRenderer::SubmitMesh(const MeshComponent& mesh, const TransformComponent& transform,
    const MaterialComponent& material, uint32_t entityID)
  {
    glm::mat4 transformMatrix = transform.GetTransform();
    m_MeshSubmissions.emplace_back(mesh.MeshHandle, transformMatrix, material.Material, entityID);

    AddMeshSource(mesh.MeshHandle);
  }

  void SceneRenderer::AddMeshSource(AssetHandle assetHandle)
  {
    auto it = m_MeshSources.find(assetHandle);
    if (it == m_MeshSources.end())
    {
      Ref<Mesh> meshSource = AssetManager::GetAsset<Mesh>(assetHandle);
      Ref<GpuMesh> gpuMesh = CreateRef<GpuMesh>(meshSource);
      m_MeshSources.insert({ assetHandle, gpuMesh });
    }
  }

  void SceneRenderer::Render()
  {
    // Skybox pass
    {
      if (m_RendererSettings.RenderingPreset == RendererPreset::Lighted)
      {
        Ref<TextureCubemap> skyboxTexture = nullptr;
        switch (m_RendererSettings.Background)
        {
          case RendererBackground::EnvironmentMap:
            skyboxTexture = m_Environment.EnvironmentMap;
            break;
          case RendererBackground::IrradianceMap:
            skyboxTexture = m_Environment.IrradianceMap;
            break;
          case RendererBackground::PreFilteredMap:
            skyboxTexture = m_Environment.PreFilteredEnvMap;
            break;
          default:
            break;
        }
        if (skyboxTexture)
        {
          Renderer::BeginRenderPass(m_SkyboxRenderPass);
          Renderer::RenderSkybox(skyboxTexture);
          Renderer::EndRenderPass();
        }
      }
    }

    // Outline pass
    {
      m_EntityOutlineRenderPass->GetFrameBuffer()->SetClearColorAttachment(
        0, m_RendererSettings.RenderingPreset != RendererPreset::Lighted);

      Renderer::BeginRenderPass(m_EntityOutlineRenderPass);
      AssetHandle meshHandle = 0;
      if (m_SelectedEntity && m_SelectedEntity.HasComponent<TransformComponent>())
      {
        if (m_SelectedEntity.HasComponent<MeshComponent>())
        {
          meshHandle = m_SelectedEntity.GetComponent<MeshComponent>().MeshHandle;
        }
        else if (m_SelectedEntity.HasComponent<MeshInstanceComponent>())
        {
          meshHandle = m_SelectedEntity.GetComponent<MeshInstanceComponent>().MeshHandle;
        }
      }

      auto it = m_MeshSources.find(meshHandle);
      if (it != m_MeshSources.end())
      {
        m_MeshDataUboStructs.Transform = m_SelectedEntity.GetComponent<TransformComponent>().GetTransform();
        m_MeshDataUboStructs.EntityID = 0;
        m_MeshDataUbo->SetData(&m_MeshDataUboStructs);

        m_WireframeMaterialUboStruct.Color = m_RendererSettings.OutlineColor;
        m_WireframeMaterialUbo->SetData(&m_WireframeMaterialUboStruct);

        Renderer::RenderMesh(it->second, m_MeshDataUbo, m_WireframeMaterialUbo);
      }
      Renderer::EndRenderPass();
    }

    switch (m_RendererSettings.RenderingPreset)
    {
      case RendererPreset::Solid:
        m_MeshRenderPass->GetPipeline()->GetSpecifications().ShaderProgram =
          Renderer::GetShaderLibrary()->Get("SolidMesh");
        m_InstanceRenderPass->GetPipeline()->GetSpecifications().ShaderProgram =
          Renderer::GetShaderLibrary()->Get("SolidInstanceMesh");
        break;
      case RendererPreset::Lighted:
        m_MeshRenderPass->GetPipeline()->GetSpecifications().ShaderProgram =
          Renderer::GetShaderLibrary()->Get("PBRMesh");
        m_InstanceRenderPass->GetPipeline()->GetSpecifications().ShaderProgram =
          Renderer::GetShaderLibrary()->Get("PBRInstanceMesh");
        break;
    }

    // Mesh pass
    {
      Renderer::BeginRenderPass(m_MeshRenderPass);
      for (const MeshSubmission& meshSubmission : m_MeshSubmissions)
      {
        m_MeshDataUbo->SetData(&meshSubmission.MeshDataUboStruct);

        Ref<UniformBuffer> materialUbo = nullptr;
        switch (m_RendererSettings.RenderingPreset)
        {
          case RendererPreset::Solid:
            m_SolidMaterialUboStruct.Color = meshSubmission.Material.Albedo;
            m_SolidMaterialUbo->SetData(&m_SolidMaterialUboStruct);
            materialUbo = m_SolidMaterialUbo;
            break;
          case RendererPreset::Lighted:
            m_PBRMaterialUboStruct.Albedo = meshSubmission.Material.Albedo;
            m_PBRMaterialUboStruct.Metalness = meshSubmission.Material.Metalness;
            m_PBRMaterialUboStruct.Roughness = meshSubmission.Material.Roughness;
            m_PBRMaterialUbo->SetData(&m_PBRMaterialUboStruct);
            materialUbo = m_PBRMaterialUbo;
            break;
        }
        SP_ASSERT(materialUbo, "Material UBO not found for mesh rendering");

        auto it = m_MeshSources.find(meshSubmission.MeshHandle);
        SP_ASSERT(it != m_MeshSources.end(), "Could not find mesh source for handle {}", meshSubmission.MeshHandle);
        Renderer::RenderMesh(it->second, m_MeshDataUbo, materialUbo);
      }
      Renderer::EndRenderPass();
    }

    // Mesh Instance pass
    {
      Renderer::BeginRenderPass(m_InstanceRenderPass);

      for (const auto& [meshHandle, instanceVertices] : m_MeshInstanceSubmissions)
      {
        m_InstanceVbo->SetData(instanceVertices.size() * sizeof(InstanceVertex), instanceVertices.data());

        auto it = m_MeshSources.find(meshHandle);
        SP_ASSERT(it != m_MeshSources.end(), "Could not find mesh source for handle {}", meshHandle);
        Renderer::RenderMeshInstances(it->second, m_InstanceVbo, instanceVertices.size());
      }
      Renderer::EndRenderPass();
    }

    // Lights pass
    if (!m_LightInstancesStruct.empty())
    {
      Renderer::BeginRenderPass(m_LightsRenderPass);

      m_LightInstanceVbo->SetData(m_LightInstancesStruct.size() * sizeof(uint32_t), m_LightInstancesStruct.data());
      Renderer::RenderMeshInstances(m_LightMesh, m_LightInstanceVbo, m_LightInstancesStruct.size());

      Renderer::EndRenderPass();
    }

    // Grid and origin pass
    {
      Renderer::BeginRenderPass(m_LinesRenderPass);
      // Origin
      Renderer::RenderLines(m_OriginLines, m_OriginLineColors);
      // Grid
      Renderer::RenderLines(m_GridLines, m_RendererSettings.GridColor);
      Renderer::EndRenderPass();
    }

    // TODO: optimize memory management across frames
    m_MeshSubmissions.clear();
    m_MeshInstanceSubmissions.clear();
  }

  std::vector<Line> SceneRenderer::GenerateGrid(float gridBound, float gridStep)
  {
    uint32_t lineCount = (uint32_t)(gridBound / gridStep) + 1;
    std::vector<Line> gridLines;
    gridLines.resize(4 * lineCount);
    for (uint32_t lineIdx = 0; lineIdx < lineCount; lineIdx++)
    {
      // From -X to X
      float lineAxisPosition = (float)lineIdx * gridStep;
      Line lineXPos = { { -gridBound, 0.0f, lineAxisPosition }, { gridBound, 0.0f, lineAxisPosition } };
      Line lineYPos = { { lineAxisPosition, 0.0f, -gridBound }, { lineAxisPosition, 0.0f, gridBound } };
      Line lineXNeg = { { -gridBound, 0.0f, -lineAxisPosition }, { gridBound, 0.0f, -lineAxisPosition } };
      Line lineYNeg = { { -lineAxisPosition, 0.0f, -gridBound }, { -lineAxisPosition, 0.0f, gridBound } };
      gridLines[4 * lineIdx + 0] = lineXPos;
      gridLines[4 * lineIdx + 1] = lineYPos;
      gridLines[4 * lineIdx + 2] = lineXNeg;
      gridLines[4 * lineIdx + 3] = lineYNeg;
    }

    return gridLines;
  }
}
