#include "OpenGLPipeline.h"

#include <glad/glad.h>

namespace Soup
{

  namespace Utils
  {

    static GLenum InternalPolygonModeToOpenGLPolygonMode(PolygoneMode mode)
    {
      switch (mode)
      {
        case PolygoneMode::Fill:
          return GL_FILL;
        case PolygoneMode::Line:
          return GL_LINE;
        case PolygoneMode::Point:
          return GL_POINT;
      }

      SP_ASSERT(false, "Unknown polygon mode!");
      return GL_FILL;
    }

    static GLenum InternalCullModeToOpenGLCullMode(FaceCullingMode mode)
    {
      switch (mode)
      {
        case FaceCullingMode::Front:
          return GL_FRONT;
        case FaceCullingMode::Back:
          return GL_BACK;
        case FaceCullingMode::FrontAndBack:
          return GL_FRONT_AND_BACK;
      }

      SP_ASSERT(false, "Unknown face culling mode!");
      return GL_FRONT;
    }

    static GLenum InternalFaceWindingModeToOpenGLWindingMode(FaceWindingMode mode)
    {
      switch (mode)
      {
        case FaceWindingMode::Clockwise:
          return GL_CW;
        case FaceWindingMode::CounterClockwise:
          return GL_CCW;
      }

      SP_ASSERT(false, "Unknown face winding mode!");
      return GL_CCW;
    }

    static GLenum InternalDepthFunctionToOpenGLDepthFunction(DepthFunctionMode function)
    {
      switch (function)
      {
        case DepthFunctionMode::Less:
          return GL_LESS;
        case DepthFunctionMode::LessOrEqual:
          return GL_LEQUAL;
      }

      SP_ASSERT(false, "Unknown depth function!");
      return GL_LESS;
    }

  }

  OpenGLPipeline::OpenGLPipeline(const PipelineSpecifications& specs)
    : m_Specs(specs)
  {
  }

  void OpenGLPipeline::SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer)
  {
    SetInputAssembly(geometryVertexBuffer, nullptr, nullptr);
  }

  void OpenGLPipeline::SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer)
  {
    SetInputAssembly(geometryVertexBuffer, geometryIndexBuffer, nullptr);
  }

  void OpenGLPipeline::SetInputAssembly(Ref<VertexBuffer> geometryVertexBuffer, Ref<IndexBuffer> geometryIndexBuffer,
    Ref<VertexBuffer> instanceVertexBuffer)
  {
    // Geoemtry vertex buffer
    m_GeometryVertexBuffer = geometryVertexBuffer;
    if (m_GeometryVertexBuffer)
    {
      m_GeometryVertexBuffer->SetGeometryLayout(m_Specs.GeometryLayout);
    }
    // Instance vertex buffer
    m_InstanceVertexBuffer = instanceVertexBuffer;
    if (m_InstanceVertexBuffer)
    {
      m_InstanceVertexBuffer->SetInstanceLayout(m_Specs.InstanceLayout);
    }

    // Index buffer
    m_GeometryIndexBuffer = geometryIndexBuffer;
  }

  void OpenGLPipeline::Bind(uint32_t viewportWidth, uint32_t viewportHeight)
  {
    // Shader
    m_Specs.Shader->Bind();

    // Viewport and scissors
    SetViewport(0, 0, viewportWidth, viewportHeight);

    // Geoemtry
    SetPolygonMode(m_Specs.Polygone);
    EnableFaceCulling(m_Specs.EnableFaceCulling);
    SetFaceCullingMode(m_Specs.FaceCulling);
    SetFaceWindingMode(m_Specs.FaceWinding);
    SetLineWidth(m_Specs.LineWidth);

    // Frame
    EnableDepthTesting(m_Specs.EnableDepthTesting);
    SetDepthFunction(m_Specs.DepthFunction);

    // Cubemap
    EnableCubemapSeamless(m_Specs.CubemapSeamless);
  }

  Ref<OpenGLVertexArray> OpenGLPipeline::CreateVertexArray()
  {
    Ref<OpenGLVertexArray> vertexArray = CreateRef<OpenGLVertexArray>();

    // Geometry vertex buffer
    if (m_GeometryVertexBuffer)
    {
      vertexArray->AddVertexBuffer(m_GeometryVertexBuffer);
    }
    // Instance vertex buffer
    if (m_InstanceVertexBuffer)
    {
      vertexArray->AddVertexBuffer(m_InstanceVertexBuffer);
    }

    // Index buffer
    if (m_GeometryIndexBuffer)
    {
      vertexArray->SetIndexBuffer(m_GeometryIndexBuffer);
    }

    vertexArray->Bake();
    return vertexArray;
  }

  void OpenGLPipeline::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  {
    glViewport(x, y, width, height);
  }

  void OpenGLPipeline::SetPolygonMode(PolygoneMode mode)
  {
    GLenum glMode = Utils::InternalPolygonModeToOpenGLPolygonMode(mode);
    glPolygonMode(GL_FRONT_AND_BACK, glMode);
  }

  void OpenGLPipeline::EnableFaceCulling(bool enable)
  {
    if (enable)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
  }

  void OpenGLPipeline::SetFaceCullingMode(FaceCullingMode mode)
  {
    GLenum glMode = Utils::InternalCullModeToOpenGLCullMode(mode);
    glCullFace(glMode);
  }

  void OpenGLPipeline::SetFaceWindingMode(FaceWindingMode mode)
  {
    GLenum glMode = Utils::InternalFaceWindingModeToOpenGLWindingMode(mode);
    glFrontFace(glMode);
  }

  void OpenGLPipeline::SetLineWidth(float lineWidth)
  {
    glLineWidth(lineWidth);
  }

  void OpenGLPipeline::EnableDepthTesting(bool enable)
  {
    if (enable)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }

  void OpenGLPipeline::SetDepthFunction(DepthFunctionMode function)
  {
    GLenum glFunction = Utils::InternalDepthFunctionToOpenGLDepthFunction(function);
    glDepthFunc(glFunction);
  }

  void OpenGLPipeline::EnableCubemapSeamless(bool enable)
  {
    if (enable)
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    else
      glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  }

}
