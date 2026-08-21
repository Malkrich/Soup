#pragma once

#include "Renderer/Pipeline.h"

#include "OpenGLVertexArray.h"

namespace Soup
{

  class OpenGLPipeline : public Pipeline
  {
  public:
    OpenGLPipeline(const PipelineSpecifications& specs);

    virtual PipelineSpecifications& GetSpecifications() override { return m_Specs; }

    virtual void SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer) override;
    virtual void SetInputAssembly(
      Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer) override;
    virtual void SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer,
      Ref<VertexBuffer> instanceVertexBuffer) override;

    void Bind(uint32_t width, uint32_t height);

    Ref<OpenGLVertexArray> CreateVertexArray();

  private:
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    void SetPolygonMode(PolygonMode mode);
    void EnableFaceCulling(bool enable);
    void SetFaceCullingMode(FaceCullingMode mode);
    void SetFaceWindingMode(FaceWindingMode mode);
    void SetLineWidth(float lineWidth);

    void EnableDepthTesting(bool enable);
    void SetDepthFunction(DepthFunctionMode function);

    void EnableCubemapSeamless(bool enabled);

  private:
    PipelineSpecifications m_Specs;

    Ref<VertexBuffer> m_GeometryVertexBuffer = nullptr;
    Ref<VertexBuffer> m_InstanceVertexBuffer = nullptr;
    Ref<IndexBuffer> m_GeometryIndexBuffer = nullptr;
  };

}
