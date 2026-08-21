#pragma once

#include "IndexBuffer.h"
#include "Shader/Shader.h"
#include "VertexBuffer.h"

namespace Soup
{

  enum class PolygonMode
  {
    Fill,
    Line,
    Point
  };

  enum class FaceCullingMode
  {
    Front,
    Back,
    FrontAndBack
  };

  enum class FaceWindingMode
  {
    Clockwise,
    CounterClockwise
  };

  enum class DepthFunctionMode
  {
    Always,
    Less,
    LessOrEqual
  };

  struct PipelineSpecifications
  {
    // ---------------------------------------------
    // -------------- Input assembly ---------------
    // ---------------------------------------------
    Layout GeometryLayout;
    Layout InstanceLayout;

    // ---------------------------------------------
    // -------------- Pipeline config --------------
    // ---------------------------------------------
    Ref<Shader> ShaderProgram = nullptr;
    // Geometry
    PolygonMode Polygon = PolygonMode::Fill;
    bool EnableFaceCulling = true;
    FaceCullingMode FaceCulling = FaceCullingMode::Back;
    FaceWindingMode FaceWinding = FaceWindingMode::CounterClockwise;
    float LineWidth = 1.0f;

    // Frame
    bool EnableDepthTesting = true;
    DepthFunctionMode DepthFunction = DepthFunctionMode::LessOrEqual;

    // Cubemap
    bool CubemapSeamless = false;
  };

  class Pipeline
  {
  public:
    static Ref<Pipeline> Create(const PipelineSpecifications& specs);

    virtual ~Pipeline() = default;

    virtual PipelineSpecifications& GetSpecifications() = 0;

    virtual void SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer) = 0;
    virtual void SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer) = 0;
    virtual void SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer,
      Ref<VertexBuffer> instanceVertexBuffer) = 0;
  };

}
