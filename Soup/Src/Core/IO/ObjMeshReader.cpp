#include "ObjMeshReader.h"

#include "tiny_obj_loader.h"

namespace Soup
{

  bool ObjMeshReader::Read(const std::filesystem::path& filePath)
  {
    tinyobj::basic_attrib_t<> attributes;
    std::vector<tinyobj::basic_shape_t<>> shapes;
    // std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    tinyobj::OptLoadConfig config;
    config.triangulate = true;
    config.num_threads = -1; // -1 = hardware_concurrency, 0/1 = single-threaded.
                             // Effective only with TINYOBJLOADER_USE_MULTITHREADING.

    bool loaderSuccess = tinyobj::LoadObjOpt(
      &attributes, &shapes, nullptr, &warning, &error, filePath.generic_string().c_str(), nullptr, config);

    if (!warning.empty())
    {
      SP_LOG_WARN("Obj import warning:");
      SP_LOG_WARN("{}", warning);
    }

    if (!loaderSuccess)
    {
      SP_LOG_ERROR("Obj import error:");
      SP_LOG_ERROR("{}", error);
      return false;
    }

    std::vector<Vertex> vertices;
    std::vector<TriangleFace> indices;

    vertices.reserve(attributes.vertices.size());

    SP_ASSERT(shapes.size() == 1, "ObjReader only support one shape!");
    indices.reserve(shapes[0].mesh.num_face_vertices.size());
    for (const tinyobj::basic_shape_t<>& shape : shapes)
    {
      // Loop over faces(polygon)
      size_t indexOffset = 0;
      for (unsigned int face : shape.mesh.num_face_vertices)
      {
        size_t vertexCount = size_t(face);
        SP_ASSERT(vertexCount == 3, "ObjReader only supports triangle faces!");

        // Loop over vertices in the face.
        for (size_t vertexIdx = 0; vertexIdx < vertexCount; vertexIdx++)
        {
          // access to vertex
          tinyobj::index_t idx = shape.mesh.indices[indexOffset + vertexIdx];

          glm::vec3 position(0.0f);
          position.x = attributes.vertices[3 * size_t(idx.vertex_index) + 0];
          position.y = attributes.vertices[3 * size_t(idx.vertex_index) + 1];
          position.z = attributes.vertices[3 * size_t(idx.vertex_index) + 2];

          glm::vec3 normal(0.0f, 0.0f, 1.0f);
          // Check if `normal_index` is zero or positive. negative = no normal data
          if (idx.normal_index >= 0)
          {
            normal.x = attributes.normals[3 * size_t(idx.normal_index) + 0];
            normal.y = attributes.normals[3 * size_t(idx.normal_index) + 1];
            normal.z = attributes.normals[3 * size_t(idx.normal_index) + 2];
          }

          vertices.emplace_back(position, normal);
        }
        TriangleFace triangleFace;
        triangleFace[0] = 0;
        triangleFace[1] = 1;
        triangleFace[2] = 2;
        triangleFace.AddOffset(indexOffset);
        indices.push_back(triangleFace);

        indexOffset += vertexCount;
      }
    }

    m_Mesh = CreateRef<Mesh>();
    m_Mesh->SetData(vertices, indices);

    return true;
  }

}
