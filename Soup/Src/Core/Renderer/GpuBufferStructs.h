#pragma once

#include "Core/Core.h"
#include "Core/Memory/Buffer.h"
#include "Core/Memory/BufferStreamWriter.h"
#include "RendererMaterial.h"

#include <glm/glm.hpp>

namespace Soup
{

  // -------------------------------------------
  // ----------- Vertex Buffers ----------------
  // -------------------------------------------
  struct LightVertex
  {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };

    LightVertex(const glm::vec3& position)
      : Position(position)
    {
    }
  };

  struct LineVertex
  {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Color = { 1.0f, 1.0f, 1.0f };

    LineVertex() = default;
    LineVertex(const glm::vec3& position, const glm::vec3 color)
      : Position(position)
      , Color(color)
    {
    }
  };

  struct LegoPartInstanceVertex
  {
    int EntityID = -1;
    RendererMaterial Material;
    glm::mat4 Transform = glm::mat4(1.0f);
  };

  // -------------------------------------------
  // ----------- Unifrom Blocks ----------------
  // -------------------------------------------
  // Camera
  struct CameraDataUbo
  {
    glm::mat4 ViewProjectionMatrix;
    glm::mat4 View;
    glm::mat4 Projection;
    glm::vec3 Position;
  };

  // Geometry
  struct MeshDataUbo
  {
    glm::mat4 Transform = glm::mat4(1.0f);
    int EntityID = -1;
  };

  // Materials
  struct PBRMaterialUbo
  {
    RendererMaterial Material;
  };

  struct WireframeMaterialUbo
  {
    glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
  };

  // Environment
  struct SkyboxDataUbo
  {
    float MipFactor = 0.0f;
  };

  struct CubemapCameraUbo
  {
    glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
  };

  struct EnvironmentCreateInfoUbo
  {
    uint32_t EnvironmentMapDimensions = 0;
  };

  struct PreFilteredCreateInfoUbo
  {
    float Roughness = 0.0f;
  };

  // -------------------------------------------
  // ----- Shader Storage Buffer Block ---------
  // -------------------------------------------
  struct LightsDataSsbo
  {
    struct PointLightElement
    {
      glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
      SP_STRUCT_PADDING(1);
      glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
      SP_STRUCT_PADDING(1);

      PointLightElement() = default;
      PointLightElement(const glm::vec3& position, const glm::vec3& color)
        : Position(position)
        , Color(color)
      {
      }
    };

    std::vector<PointLightElement> PointLights;

    Buffer CreateBuffer()
    {
      Buffer buffer;
      BufferStreamWriter stream(buffer);
      stream.WritePrimitiveType<uint32_t>((uint32_t)PointLights.size());
      stream.WriteSpan(3, 0);
      stream.WriteVector(PointLights);
      return buffer;
    }
  };

}
