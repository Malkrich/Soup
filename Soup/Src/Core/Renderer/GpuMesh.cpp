#include "GpuMesh.h"

namespace Soup
{

  GpuMesh::GpuMesh(Ref<Mesh> mesh)
  {
    const std::vector<Vertex>& vertices = mesh->GetVertices();
    const std::vector<TriangleFace>& indices = mesh->GetConnectivities();

    CreateBuffers(
      vertices.size() * sizeof(Vertex), vertices.data(), indices.size() * sizeof(TriangleFace), indices.data());
  }

  GpuMesh::GpuMesh(
    uint32_t vertexBufferSize, const void* vertexBufferData, uint32_t indexBufferSize, const void* indexBufferData)
  {
    CreateBuffers(vertexBufferSize, vertexBufferData, indexBufferSize, indexBufferData);
  }

  void GpuMesh::CreateBuffers(
    uint32_t vertexBufferSize, const void* vertexBufferData, uint32_t indexBufferSize, const void* indexBufferData)
  {
    m_GeometryVbo = VertexBuffer::Create(vertexBufferSize, vertexBufferData);
    m_GeometryEbo = IndexBuffer::Create(indexBufferSize, indexBufferData);
  }

}
