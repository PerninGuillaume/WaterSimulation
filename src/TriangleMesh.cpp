#include <utility>
#include <stdexcept>

#include "TriangleMesh.hh"


//Function for smooth triangle with textures
void triangleMesh(Scene& scene, const std::shared_ptr<Texture_Material>& texture_material, const std::vector<int> &faceIndex
                           , const std::vector<int> &vertexIndices, const std::vector<Point3>& points
                           , const std::vector<Vector3> &normals, const std::vector<Point3>& textureCoordinates)
{
  //or store a list of created triangle ?
  for (size_t i = 0, k = 0; i < faceIndex.size(); ++i) {
    for (int j = 0; j < faceIndex[i] - 2; ++j) {
      int index_1 = k;
      int index_2 = k + j + 1;
      int index_3 = k + j + 2;
      Point3 A = points[vertexIndices[index_1]];
      Point3 B = points[vertexIndices[index_2]];
      Point3 C = points[vertexIndices[index_3]];
      Vector3 normalA = normals[vertexIndices[index_1]];
      Vector3 normalB = normals[vertexIndices[index_2]];
      Vector3 normalC = normals[vertexIndices[index_3]];
      Vector3 faceNormal = Vector3(A, B).vector_product(Vector3(A,C));
      if (normalA == Vector3()) //not initialized
        normalA = faceNormal;
      if (normalB == Vector3()) //not initialized
        normalB = faceNormal;
      if (normalC == Vector3()) //not initialized
        normalC = faceNormal;
      scene.add_object(std::make_shared<SmoothTriangle>(texture_material
          , A, B, C
          , normalA, normalB, normalC
          , textureCoordinates[vertexIndices[index_1]], textureCoordinates[vertexIndices[index_2]], textureCoordinates[vertexIndices[index_3]]));
    }
    k += faceIndex[i];
  }
}


//Function for normal triangle and no textures
void triangleMesh(Scene& scene, const std::shared_ptr<Texture_Material>& texture_material, const std::vector<int> &faceIndex
        , const std::vector<int> &vertexIndices, const std::vector<Point3>& points)
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

void create_base_rectangle(const Point3& A, const Point3& B, const Point3& C, const Point3& D,
                           unsigned widthDivisions, unsigned heightDivisions, std::vector<int>& faceIndex,
                           std::vector<Point3>& points, std::vector<int>& vertexIndices,
                           std::vector<Point3>& textureCoordinates, Vector3& normal) {
  // A ----- B
  // |       |
  // |       |
  // D ----- C
  Vector3 AB = Vector3(A, B);
  Vector3 AD = Vector3(A, D);
  Vector3 AC = Vector3(A, C);
  normal = AB.vector_product(AC);
  if (std::abs(normal.scalar_product(AD)) > 0.01) {
    throw std::runtime_error("The 4 points are not in the same plane\n");
  }
  normal.normalize(); //So it will be a good vector in the function calling this one
  unsigned nb_faces = widthDivisions * heightDivisions;
  faceIndex = std::vector<int>(static_cast<int>(nb_faces), 4);
  Vector3 width_delta = AB / widthDivisions;
  Vector3 height_delta = AD / heightDivisions;
  double width_delta_2D = 1.0 / widthDivisions;
  double height_delta_2D = 1.0 / heightDivisions;

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
}

void rectangle_displaced_by_noise(Scene& scene, const Point3& A, const Point3& B, const Point3& C, const Point3& D,
                                  unsigned widthDivisions, unsigned heightDivisions, std::shared_ptr<Texture_Material> textureMaterial,
                                  bool analytical_normals, bool smooth) {


  std::vector<int> faceIndex;
  std::vector<Point3> points;
  std::vector<int> vertexIndices;
  std::vector<Point3> textureCoordinates;
  Vector3 normal;
  create_base_rectangle(A, B, C, D, widthDivisions, heightDivisions, faceIndex, points, vertexIndices,
                        textureCoordinates, normal);
  PerlinNoise perlinNoise(12);
  double frequency = 1.25f;
  double amplitude = 0.5f;
  unsigned numVertices = (heightDivisions + 1) * (widthDivisions + 1);
  std::vector<Vector3> normals(numVertices, Vector3());
  //Compute displacements
  for (unsigned i = 0; i < numVertices; ++i) {
    //unsigned x = textureCoordinates[i].x * widthDivisions;
    //unsigned y = textureCoordinates[i].y * heightDivisions;

    if (analytical_normals) {

      Vector3 derivatives;
      //double value = perlinNoise.eval(Point3(x, y, 0) * frequency, derivatives) * amplitude;//TODO if we had a 2d eval function, we could call this one
      //double value = perlinNoise.eval(Point3(points[i].x + 0.5, points[i].y + 0.5, 0), derivatives);//TODO if we had a 2d eval function, we could call this one
      //double value = perlinNoise.eval(points[i] + 0.5, derivatives);//TODO if we had a 2d eval function, we could call this one
      double value = perlinNoise.eval_2d(points[i], derivatives) * amplitude;
      points[i] += normal * value;
      // Displace along the normal according to the noise generated at the 2D coordinates
      normals[i] = (Vector3(-derivatives.x, -derivatives.y, 1).normalize());
    }
    else {
      //double value = perlinNoise.eval(Point3(x, y, 0) * frequency) * amplitude;//TODO if we had a 2d eval function, we could call this one
      double value = perlinNoise.eval(points[i] * frequency) * amplitude;//TODO if we had a 2d eval function, we could call this one
      points[i] += normal * value;
    }
  }
  unsigned nb_faces = widthDivisions * heightDivisions;

  if (!analytical_normals && smooth) {
    //Compute normals
    for (unsigned i = 0, off = 0; i < nb_faces; ++i) {
      unsigned nverts = faceIndex[i];
      Point3 point = points[vertexIndices[off]];
      Point3 point_x = points[vertexIndices[off + 1]];
      Point3 point_y = points[vertexIndices[off + nverts - 1]];
      Vector3 tangent = point_x - point;
      Vector3 bitangent = point_y - point;
      Vector3 normal_point = bitangent.vector_product(tangent).normalize();
      normals[vertexIndices[off]] = normal_point;
      off += nverts;

    }
  }

  if (smooth)
    triangleMesh(scene, std::move(textureMaterial), faceIndex, vertexIndices
        , points, normals, textureCoordinates);
  else
    triangleMesh(scene, std::move(textureMaterial), faceIndex, vertexIndices
        , points);
}