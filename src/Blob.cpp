#include "Blob.hh"
#include <array>

Blob::Blob(Point3 center, double e, double d, std::vector<Point3> blobs_origin, double threshold
, std::shared_ptr<Texture_Material> texture_material)
  : center(center)
  , e(e)
  , d(d)
  , blobs_origin(blobs_origin)
  , threshold(threshold)
  , texture_material(texture_material) {}

double Blob::potential(const Point3& point) {
  double value = 0;
  for (const auto& blob : blobs_origin) {
    Vector3 distance(blob, point);
    value += 1.0 / (distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
  }
  return value;
}

bool Blob::satisfy_threshold(double value) {
  return value >= threshold;
}

bool Blob::is_in_blob(const Point3& point) {
  return this->potential(point) >= threshold;
}

std::array<bool, 8> Blob::are_in_blob(const std::array<double, 8>& potentials) {
  std::array<bool, 8> in_blob;
  for (int index = 0; index < 8; ++index) {
    in_blob[index] = satisfy_threshold(potentials[index]);
  }
  return in_blob;
}

std::array<double, 8> Blob::give_potentials(const std::array<Point3, 8>& boundaries) {
  std::array<double, 8> potentials = {};
  for (int index = 0; index < 8; ++index) {
    potentials[index] = potential(boundaries[index]);
  }
  return potentials;
}

int Blob::give_index(const std::array<double, 8>& potentials_value) {
  auto potentials = are_in_blob(potentials_value);
  int index = 0;

  if (potentials[0]) index |= 1;
  if (potentials[1]) index |= 2;
  if (potentials[2]) index |= 4;
  if (potentials[3]) index |= 8;
  if (potentials[4]) index |= 16;
  if (potentials[5]) index |= 32;
  if (potentials[6]) index |= 64;
  if (potentials[7]) index |= 128;

  return index;
}

Vector3 Blob::normal_at_point(const Point3& point) {
  Vector3 normal(0,0,0);
  for (const auto& blob_origin : blobs_origin) {
    normal += Vector3(blob_origin, point).normalize();
  }
  return normal.normalize();
}

void Blob::add_triangles(Scene& scene, int index, const Point3& cube, const std::array<Point3, 12>& edges_position) {
  //std::array<Point3, 12> edges_position = {
  //    cube + Point3(d / 2, 0, 0), cube + Point3(d, d / 2, 0), cube + Point3(d / 2 , d, 0), cube + Point3(0, d / 2, 0)
  //  , cube + Point3(d / 2, 0, -d), cube + Point3(d, d / 2, -d), cube + Point3(d / 2 , d, -d), cube + Point3(0, d / 2, -d)
  //  , cube + Point3(0, 0, -d / 2), cube + Point3(d, 0, -d / 2), cube + Point3(d, d, -d / 2), cube + Point3(0, d, -d / 2)};
  auto triangles_edges = triangle_configurations[index];
  int i = 0;
  while (triangles_edges[i] != -1) {
    Point3 A = edges_position[triangles_edges[i]];
    Point3 B = edges_position[triangles_edges[i + 1]];
    Point3 C = edges_position[triangles_edges[i + 2]];
    if (smooth_triangle) {
      scene.add_object(std::make_shared<SmoothTriangle>(texture_material, A, B, C, normal_at_point(A), normal_at_point(B), normal_at_point(C)));
    } else {
      scene.add_object(std::make_shared<Triangle>(texture_material, A, B, C));
    }
    i += 3;
  }
}

Point3 Blob::interpolated_value(const Point3& A, const Point3& B, double A_potential, double B_potential) const {
  if (A_potential < B_potential) {
    double coeff = (this->threshold - A_potential) / (B_potential - A_potential);
    Point3 P = Vector3(A, B) * coeff + A;
    return P;
  }
  else {
    double coeff = (this->threshold - B_potential) / (A_potential - B_potential);
    Point3 P = Vector3(B, A) * coeff + B;
    return P;
  }
}

std::array<Point3, 12> Blob::edges_position(const std::array<Point3, 8>& boundaries, const std::array<double, 8>& potentials) const {
  //The vertices are grouped by two
  std::array<int, 24> vertices = {0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7};
  std::array<Point3, 12> edges = {};
  for (int index = 0; index < 24; index += 2) {
    edges[index / 2] = interpolated_value(boundaries[vertices[index]], boundaries[vertices[index + 1]],
                                          potentials[vertices[index]], potentials[vertices[index + 1]]);
  }
  return edges;
}

void Blob::marching_cubes(Scene& scene) {

  int number_cubes = this->e / this->d;
  Point3 origin = this->center + Point3(-1 * this->e / 2, - 1 * this->e / 2, this->e / 2);
  Point3 current = origin;
  for (int x = 0; x < number_cubes; ++x) {
    for (int y = 0; y < number_cubes; ++y) {
      for (int z = 0; z < number_cubes; ++z) {
        std::array<Point3, 8> boundaries = {
            current, current + Point3(d, 0, 0), current + Point3(d, d, 0), current + Point3(0, d, 0)
            , current + Point3(0, 0, -d), current + Point3(d, 0, -d), current + Point3(d, d, -d), current + Point3(0, d, -d)};
        auto potentials = give_potentials(boundaries);
        auto edges = edges_position(boundaries, potentials);
        int index = give_index(give_potentials(boundaries));
        add_triangles(scene, index, current, edges);

        current += Point3(0, 0, -d);
      }
      current.z = origin.z;
      current += Point3(0, d, 0);
    }
    current.y = origin.y;
    current += Point3(d, 0, 0);
  }
}