#pragma once

#include <glm/glm.hpp>

#include <filesystem>

namespace Soup
{

  enum class ModelFileFormat
  {
    None = 0,
    Obj,
    LDrawDat
  };

  struct Vertex
  {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Normal = { 0.0f, 0.0f, 0.0f };

    Vertex() = default;
    Vertex(const Vertex&) = default;
    Vertex(const glm::vec3& position, const glm::vec3& normal)
      : Position(position)
      , Normal(normal)
    {
    }
  };

  struct TriangleFace
  {
    TriangleFace(uint32_t i0, uint32_t i1, uint32_t i2)
      : m_Indices{ i0, i1, i2 }
    {
    }
    TriangleFace()
      : m_Indices{ 0, 0, 0 }
    {
    }

    void AddOffset(uint32_t offset)
    {
      for (uint32_t& i : m_Indices)
        i += offset;
    }

    uint32_t& operator[](uint32_t i)
    {
      SP_ASSERT(i < m_Indices.size(), "Index of a triangle has to be less than 3.");
      return m_Indices[i];
    }
    uint32_t operator[](uint32_t i) const
    {
      SP_ASSERT(i < m_Indices.size(), "Index of a triangle has to be less than 3.");
      return m_Indices[i];
    }

  private:
    std::array<uint32_t, 3> m_Indices;
  };

  class Mesh
  {
  public:
    Mesh() = default;

    bool IsEmpty() const { return m_Vertices.empty(); }
    const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
    const std::vector<TriangleFace>& GetConnectivities() const { return m_Connectivities; }

    // Data modifications
    void SetData(const std::vector<Vertex>& vertices, const std::vector<TriangleFace> indices);

    void AddTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void AddTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::mat4& transform);
    void AddTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

    void AddQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void AddQuad(
      const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& transform);
    void AddQuad(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    // Geometry modifications
    void Transform(const glm::mat4& transform);
    void Scale(float scaleFactor);
    void Scale(const glm::vec3& scaleVector);

  private:
    template <uint32_t VertexCount, uint32_t TriangleCount>
    void AddGeometry(const std::array<Vertex, VertexCount>& vertices)
    {
      SP_ASSERT(TriangleCount <= 2, "Only triangle and quand are the accepted geometry to be added in Mesh!");

      // Geometry
      m_Vertices.reserve(m_Vertices.size() + VertexCount);
      for (uint32_t i = 0; i < VertexCount; i++)
        m_Vertices.push_back(vertices[i]);

      // Connectivities
      // Triangle 1
      m_Connectivities.reserve(m_Connectivities.size() + TriangleCount);
      TriangleFace t1 = { 0, 1, 2 };
      t1.AddOffset(m_LastIndex);
      m_Connectivities.push_back(t1);
      // Triangle 2 -> if adding a quad
      if constexpr (TriangleCount > 1)
      {
        TriangleFace t2 = { 2, 3, 0 };
        t2.AddOffset(m_LastIndex);
        m_Connectivities.push_back(t2);
      }

      m_LastIndex += VertexCount;
    }

  private:
    std::vector<Vertex> m_Vertices;
    std::vector<TriangleFace> m_Connectivities;
    uint32_t m_LastIndex = 0;
  };

}
