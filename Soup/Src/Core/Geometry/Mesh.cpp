#include "Mesh.h"

#include "Utils/MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Soup
{
  namespace Utils
  {
    static glm::vec3 ComputeNormal(
      const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::mat4& geometryTransform)
    {
      glm::vec3 normal = glm::cross(p2 - p0, p1 - p0);

      glm::mat3 normalTransform = MathUtils::ComputeNormalTransform(geometryTransform);

      return glm::normalize(normalTransform * normal);
    }
  }

  static std::unordered_map<std::string, ModelFileFormat> s_FileFormatMap = { { ".obj", ModelFileFormat::Obj },
    { ".dat", ModelFileFormat::LDrawDat } };

  static ModelFileFormat ModelFileFormatFromExtension(const std::string& extension)
  {
    if (s_FileFormatMap.find(extension) == s_FileFormatMap.end())
      return ModelFileFormat::None;

    return s_FileFormatMap.at(extension);
  }

  void Mesh::SetData(const std::vector<Vertex>& vertices, const std::vector<TriangleFace> connectivities)
  {
    m_Vertices = vertices;
    m_Connectivities = connectivities;
    m_LastIndex = vertices.size();
  }

  void Mesh::AddTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
  {
    AddGeometry<3, 1>({ v0, v1, v2 });
  }

  void Mesh::AddTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::mat4& transform)
  {
    glm::vec3 tp0 = transform * glm::vec4(p0, 1.0f);
    glm::vec3 tp1 = transform * glm::vec4(p1, 1.0f);
    glm::vec3 tp2 = transform * glm::vec4(p2, 1.0f);

    // Normal calculation
    glm::vec3 normal = Utils::ComputeNormal(p0, p1, p2, transform);

    Vertex v0 = { tp0, normal };
    Vertex v1 = { tp1, normal };
    Vertex v2 = { tp2, normal };

    AddTriangle(v0, v1, v2);
  }

  void Mesh::AddTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
  {
    AddTriangle(p0, p1, p2, glm::mat4(1.0f));
  }

  void Mesh::AddQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3)
  {
    AddGeometry<4, 2>({ v0, v1, v2, v3 });
  }

  void Mesh::AddQuad(
    const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::mat4& transform)
  {
    glm::vec3 tp0 = transform * glm::vec4(p0, 1.0f);
    glm::vec3 tp1 = transform * glm::vec4(p1, 1.0f);
    glm::vec3 tp2 = transform * glm::vec4(p2, 1.0f);
    glm::vec3 tp3 = transform * glm::vec4(p3, 1.0f);

    // normal calculation
    glm::vec3 normal = Utils::ComputeNormal(p0, p1, p2, transform);

    Vertex v0 = { tp0, normal };
    Vertex v1 = { tp1, normal };
    Vertex v2 = { tp2, normal };
    Vertex v3 = { tp3, normal };

    AddQuad(v0, v1, v2, v3);
  }

  void Mesh::AddQuad(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
  {
    AddQuad(p0, p1, p2, p3, glm::mat4(1.0f));
  }

  void Mesh::Transform(const glm::mat4& transform)
  {
    glm::mat3 normalTransform = MathUtils::ComputeNormalTransform(transform);

    for (Vertex& v : m_Vertices)
    {
      v.Position = transform * glm::vec4(v.Position, 1.0f);
      v.Normal = glm::normalize(normalTransform * v.Normal);
    }
  }

  void Mesh::Scale(float scaleFactor)
  {
    Scale(glm::vec3(scaleFactor));
  }

  void Mesh::Scale(const glm::vec3& scaleVector)
  {
    glm::mat4 scaleTransform = glm::scale(glm::mat4(1.0), scaleVector);
    Transform(scaleTransform);
  }

}
