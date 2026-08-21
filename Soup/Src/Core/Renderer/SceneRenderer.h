#pragma once

#include "GpuBufferStructs.h"
#include "GpuMesh.h"
#include "Lights.h"
#include "PerspectiveCamera.h"
#include "Primitives.h"
#include "RendererAPI.h"
#include "RendererMaterial.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Texture2D.h"
#include "UniformBuffer.h"

#include <glm/glm.hpp>

namespace Soup
{

  enum class RendererPreset
  {
    Solid,
    Lighted
  };

  struct MeshSubmissionData
  {
    Ref<GpuMesh> Mesh = nullptr;
    glm::mat4 Transform = glm::mat4(1.0f);
    RendererMaterial Material;
    int EntityID = -1;

    MeshSubmissionData() = default;
    MeshSubmissionData(Ref<GpuMesh> mesh, const glm::mat4& transform, const RendererMaterial& material, int entityID)
      : Mesh(mesh)
      , Transform(transform)
      , Material(material)
      , EntityID(entityID)
    {
    }
  };

  struct DirectionalLightData
  {
    bool Enable = false;
    glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
  };

  struct SceneLightsData
  {
    std::vector<PointLight> PointLights;
    std::vector<uint32_t> PointLightIDs;

    // Not implemented yet
    DirectionalLightData DirectionalLight;
  };

  enum class RendererBackground
  {
    ClearColor = 0,
    EnvironmentMap,
    IrradianceMap,
    PreFilteredMap
  };

  class SceneRenderer
  {
  public:
    struct Settings
    {
      // Global
      RendererPreset RenderingPreset = RendererPreset::Lighted;

      // Grid
      float GridBound = 1.0f;
      float GridStep = 0.1f;
      glm::vec3 GridColor = glm::vec3(0.0f, 0.0f, 0.0f);

      // Wireframe
      float OutlineWidth = 5.0f;
      glm::vec3 OutlineColor = { 0.05f, 0.8f, 0.05f };

      // Skybox
      RendererBackground Background = RendererBackground::EnvironmentMap;
      float SkyboxMipFactor = 0.0f;

      Settings() = default;
    };

  public:
    SceneRenderer(uint32_t viewportWidth, uint32_t viewportHeight);

    // Viewport and entities
    Ref<Texture2D> GetRenderedImage() const { return m_RenderedImage; }
    int32_t GetEntityIDAt(uint32_t mouseX, uint32_t mouseY) const;
    void ResizeViewport(uint32_t width, uint32_t height);

    /**
     * Render scene content.
     */
    void Render(const PerspectiveCamera& camera, Ref<Scene> scene);

    // Set the IBL environment texture
    void SetEnvironment(Ref<Texture2D> hdriTexture, const EnvironmentCreateInfo& environmentCreateInfo);

    // Environment
    void SetSceneEnvironment(const PerspectiveCamera& camera, const SceneLightsData& env);

    Settings& GetRendererSettings() { return m_RendererSettings; }

    void SetSelectedEntity(Entity e) { m_SelectedEntity = e; }

    void SetActiveCamera(const Camera& camera);

  private:
    void Init(uint32_t viewportWidth, uint32_t viewportHeight);

    /**
     * Render all submissions.
     */
    void Render();

    /**
     * Submit single mesh to render.
     */
    void SubmitMesh(const MeshComponent& mesh, const TransformComponent& transform, const MaterialComponent& material,
      uint32_t entityID);

    /**
     * Submit an instance of the given mesh.
     * The transforms are accumulated to a vector associated to the mesh pointer.
     */
    void SubmitMeshInstance(
      Ref<GpuMesh> mesh, const TransformComponent& transform, const MaterialComponent& material, uint32_t entityID);

    std::vector<Line> GenerateGrid(float gridBound, float gridStep);

  private:
    // Renderer internal
    Settings m_RendererSettings;
    // Selected entity
    Entity m_SelectedEntity;

    // Environmnent
    RendererEnvironment m_Environment;

    // Render passes
    Ref<Texture2D> m_RenderedImage = nullptr;
    Ref<RenderPass> m_EntityOutlineRenderPass = nullptr;
    Ref<RenderPass> m_PbrRenderPass = nullptr;
    Ref<RenderPass> m_InstanceRenderPass = nullptr;
    Ref<RenderPass> m_LightsRenderPass = nullptr;
    Ref<RenderPass> m_LinesRenderPass = nullptr;
    Ref<RenderPass> m_SkyboxRenderPass = nullptr;

    // Camera
    CameraDataUbo m_cameraDataUboStruct;
    Ref<UniformBuffer> m_CameraDataUbo = nullptr;

    // Mesh data
    MeshDataUbo m_MeshDataUboStructs;
    Ref<UniformBuffer> m_MeshDataUbo = nullptr;

    // Materials
    PBRMaterialUbo m_PBRMaterialUboStruct;
    Ref<UniformBuffer> m_PBRMaterialUbo = nullptr;
    WireframeMaterialUbo m_WireframeMaterialUboStruct;
    Ref<UniformBuffer> m_WireframeMaterialUbo = nullptr;

    // Meshes
    std::vector<MeshSubmissionData> m_MeshSubmissions;
    std::unordered_map<Ref<GpuMesh>, std::vector<InstanceVertex>> m_InstanceSubmissions;
    Ref<VertexBuffer> m_InstanceVbo = nullptr;

    // Skybox
    SkyboxDataUbo m_SkyboxDataUboStruct;
    Ref<UniformBuffer> m_SkyboxDataUbo = nullptr;

    // Lights
    Ref<GpuMesh> m_LightMesh = nullptr;
    LightsDataSsbo m_LightsDataSsboStruct;
    Ref<ShaderStorageBuffer> m_LightsDataSsbo = nullptr;
    std::vector<uint32_t> m_LightInstancesStruct;
    Ref<VertexBuffer> m_LightInstanceVbo = nullptr;

    // Lines
    // Origin helper
    std::vector<Line> m_OriginLines;
    // Grid
    std::vector<glm::vec3> m_OriginLineColors;
    std::vector<Line> m_GridLines;
  };

}
