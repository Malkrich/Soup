#pragma once

#include "Geometry/Mesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Soup
{

  class GpuMesh
  {
  public:
    template <typename VertexT, typename IndexT>
    static Ref<GpuMesh> Create(const std::vector<VertexT>& vertices, const std::vector<IndexT>& indices)
    {
      return CreateRef<GpuMesh>((uint32_t)(vertices.size() * sizeof(VertexT)), vertices.data(),
        (uint32_t)(indices.size() * sizeof(IndexT)), indices.data());
    }

    GpuMesh(Ref<Mesh> mesh);
    GpuMesh(
      uint32_t vertexBufferSize, const void* vertexBufferData, uint32_t indexBufferSize, const void* indexBufferData);

    const Ref<VertexBuffer>& GetGeometryVertexBuffer() const { return m_GeometryVbo; }
    const Ref<IndexBuffer>& GetGeometryIndexBuffer() const { return m_GeometryEbo; }

  private:
    void CreateBuffers(
      uint32_t vertexBufferSize, const void* vertexBufferData, uint32_t indexBufferSize, const void* indexBufferData);

  private:
    Ref<VertexBuffer> m_GeometryVbo;
    Ref<IndexBuffer> m_GeometryEbo;
  };

}
