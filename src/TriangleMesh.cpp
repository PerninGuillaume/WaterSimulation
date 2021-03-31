#include <utility>

#include "TriangleMesh.hh"


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
      //TODO add texture_coordinates to the triangle
      //But where to add them ?
    }
    k += faceIndex[i];
  }
}
/*std::optional<double> TriangleMesh::is_intersecting(const Rayon &ray) {

  Rayon ray_translated(ray.direction, ray.origin);
  //Translation for reflection/refraction
  ray_translated.origin = ray.origin + ray.direction * epsilon;
  std::optional<double> t_min;
  for (auto &triangle : triangles) {
    auto t = triangle.is_intersecting(ray_translated);
    if (t) {
      if (t > this->epsilon && (!t_min || t < t_min.value())) {
        t_min = t;
      }
    }
  }
  return t_min;
}

Vector3 TriangleMesh::normal_at_point(const Point3 &point, const Rayon &ray) {
  triangles[0].normal_at_point(point, ray);//TODO don't work
  return Vector3();
}
Caracteristics TriangleMesh::texture_at_point(const Point3 &point) {
  return texture_material->caracteristics_point(point);
}
*/