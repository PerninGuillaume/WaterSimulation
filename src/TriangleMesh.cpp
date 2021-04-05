#include <utility>

#include "TriangleMesh.hh"


//Function for smooth triangle with textures
void triangleMesh(Scene& scene, std::shared_ptr<Texture_Material> texture_material, const std::vector<int> &faceIndex
                           , const std::vector<int> &vertexIndices, std::vector<Point3> points
                           , const std::vector<Vector3> &normals, const std::vector<Point3>& textureCoordinates)
{
  //or store a list of created triangle ?
  for (size_t i = 0, k = 0; i < faceIndex.size(); ++i) {
    for (int j = 0; j < faceIndex[i] - 2; ++j) {
      int index_1 = k;
      int index_2 = k + j + 1;
      int index_3 = k + j + 2;
      scene.add_object(std::make_shared<SmoothTriangle>(texture_material
          , points[vertexIndices[index_1]], points[vertexIndices[index_2]], points[vertexIndices[index_3]]
          , normals[index_1], normals[index_2], normals[index_3]
          , textureCoordinates[index_1], textureCoordinates[index_2], textureCoordinates[index_3]));
    }
    k += faceIndex[i];
  }
}


//Function for normal triangle and no textures
void triangleMesh(Scene& scene, std::shared_ptr<Texture_Material> texture_material, const std::vector<int> &faceIndex
        , const std::vector<int> &vertexIndices, std::vector<Point3> points)
{
  //or store a list of created triangle ?
  for (size_t i = 0, k = 0; i < faceIndex.size(); ++i) {
    for (int j = 0; j < faceIndex[i] - 2; ++j) {
      int index_1 = k;
      int index_2 = k + j + 1;
      int index_3 = k + j + 2;
      scene.add_object(std::make_shared<Triangle>(texture_material
              , points[vertexIndices[index_1]], points[vertexIndices[index_2]], points[vertexIndices[index_3]]));
    }
    k += faceIndex[i];
  }
}

void rectangle_displaced_by_noise(Scene& scene, const Point3& A, const Point3& B, const Point3& C, const Point3& D,
                                  unsigned widthDivisions, unsigned heightDivisions, std::shared_ptr<Texture_Material> textureMaterial) {

  // A ----- B
  // |       |
  // |       |
  // D ----- C
  Vector3 AB = Vector3(A, B);
  Vector3 AD = Vector3(A, D);
  Vector3 AC = Vector3(A, C);
  Vector3 normal = AB.vector_product(AC);
  if (std::abs(normal.scalar_product(AD)) > scene.epsilon) {
    std::cerr << "The 4 points are not in the same plane\n";
    return;
  }
  unsigned nb_faces = widthDivisions * heightDivisions;
  std::vector<int> faceIndex(static_cast<int>(nb_faces), 4);
  Vector3 width_delta = AB / widthDivisions;
  Vector3 height_delta = AD / heightDivisions;
  double width_delta_2D = 1.0 / widthDivisions;
  double height_delta_2D = 1.0 / heightDivisions;

  std::vector<Point3> points;
  std::vector<int> vertexIndices;
  std::vector<Point3> textureCoordinates;

  Point3 current_point = A, current_height = A;
  for (unsigned i = 0; i <= heightDivisions; ++i) {
    for (unsigned j = 0; j <= widthDivisions; ++j) {
      points.emplace_back(current_point);
      textureCoordinates.emplace_back(Point3(j * width_delta_2D, i * height_delta_2D, 0));
      current_point += width_delta;
    }
    current_height += height_delta;
    current_point = current_height;
  }

  for (unsigned i = 0; i < heightDivisions; ++i) {
    for (unsigned j = 0; j < widthDivisions; ++j) {
      vertexIndices.emplace_back(i * (widthDivisions + 1) + j);
      vertexIndices.emplace_back(i * (widthDivisions + 1) + j + 1);
      vertexIndices.emplace_back((i + 1) * (widthDivisions + 1) + j + 1);
      vertexIndices.emplace_back((i + 1) * (widthDivisions + 1) + j);
    }
  }
  PerlinNoise perlinNoise(12);
  double frequency = 1.f;
  /*for (unsigned i = 0; i < heightDivisions; ++i) {
    for (unsigned j = 0; j < widthDivisions; ++j) {
      perlinNoise.eval(Point3(j, i, 0) * frequency); //TODO if we had a 2d eval function, we could call this one

    }
  }
   */
  unsigned numVertices = (heightDivisions + 1) * (widthDivisions + 1);
  for (unsigned i = 0; i < numVertices; ++i) {
    unsigned x = textureCoordinates[i].x * widthDivisions;
    unsigned y = textureCoordinates[i].y * heightDivisions;
    points[i] += normal * perlinNoise.eval(Point3(x, y, 0) * frequency);
    // Displace along the normal according to the noise generated at the 2D coordinates
  }

  //std::vector<int> vertexIndices = {0,1,2,3};
  //std::vector<Point3> points = {{4,-1,-1}, {4, -1, 1}, {4, 1, 1}, {4, 1, -1}};
  //std::vector<Vector3> normals = {{-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}};
  //std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::move(textureMaterial), faceIndex, vertexIndices
          , points);
}