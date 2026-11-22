#include "MeshFactory.h"

#include "Mesh.h"
#include "Metric/World.h"

namespace Soup::MeshFactory
{

  Ref<Mesh> CreateCube()
  {
    const std::vector<Vertex> vertices = {
      // Top
      { { -1.0f, 1.0f, -1.0f }, World::GetUpVector() }, // 0
      { { 1.0f, 1.0f, -1.0f }, World::GetUpVector() },  // 1
      { { -1.0f, 1.0f, 1.0f }, World::GetUpVector() },  // 2
      { { 1.0f, 1.0f, 1.0f }, World::GetUpVector() },   // 3

      // Bottom
      { { -1.0f, -1.0f, -1.0f }, -World::GetUpVector() }, // 4
      { { 1.0f, -1.0f, -1.0f }, -World::GetUpVector() },  // 5
      { { -1.0f, -1.0f, 1.0f }, -World::GetUpVector() },  // 6
      { { 1.0f, -1.0f, 1.0f }, -World::GetUpVector() },   // 7

      // Front
      { { -1.0f, 1.0f, 1.0f }, World::GetFrontVector() },  // 8
      { { 1.0f, 1.0f, 1.0f }, World::GetFrontVector() },   // 9
      { { -1.0f, -1.0f, 1.0f }, World::GetFrontVector() }, // 10
      { { 1.0f, -1.0f, 1.0f }, World::GetFrontVector() },  // 11

      // Back
      { { -1.0f, 1.0f, -1.0f }, -World::GetFrontVector() },  // 12
      { { 1.0f, 1.0f, -1.0f }, -World::GetFrontVector() },   // 13
      { { -1.0f, -1.0f, -1.0f }, -World::GetFrontVector() }, // 14
      { { 1.0f, -1.0f, -1.0f }, -World::GetFrontVector() },  // 15

      // Left
      { { -1.0f, 1.0f, 1.0f }, -World::GetRightVector() },   // 16
      { { -1.0f, 1.0f, -1.0f }, -World::GetRightVector() },  // 17
      { { -1.0f, -1.0f, 1.0f }, -World::GetRightVector() },  // 18
      { { -1.0f, -1.0f, -1.0f }, -World::GetRightVector() }, // 19

      // Right
      { { 1.0f, 1.0f, 1.0f }, World::GetRightVector() },  // 20
      { { 1.0f, 1.0f, -1.0f }, World::GetRightVector() }, // 21
      { { 1.0f, -1.0f, 1.0f }, World::GetRightVector() }, // 22
      { { 1.0f, -1.0f, -1.0f }, World::GetRightVector() } // 23
    };

    const std::vector<TriangleFace> indices = { // Top
      { 0, 1, 2 }, { 2, 3, 1 },

      // Bottom
      { 4, 5, 6 }, { 6, 7, 5 },

      // Front
      { 8, 9, 10 }, { 10, 11, 9 },

      // Back
      { 12, 13, 14 }, { 14, 15, 13 },

      // Left
      { 16, 17, 18 }, { 18, 19, 17 },

      // Right
      { 20, 21, 22 }, { 22, 23, 21 }
    };

    return CreateRef<Mesh>(vertices, indices);
  }
}
