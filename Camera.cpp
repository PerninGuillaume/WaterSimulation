#include "Camera.hh"
#include <cmath>

Camera::Camera(Point3 center, Point3 spotted_point, Vector3 up, float alpha, float beta, float zmin)
    : center(center)
    , spotted_point(spotted_point)
    , up(up.normalize())
    , alpha(alpha * M_PI / 180.0)
    , beta(beta * M_PI / 180.0)
    , zmin(zmin)
    , center_to_spotted_point((Vector3(this->center, this->spotted_point)).normalize())
    , center_image_plane(center_to_spotted_point * zmin + this->center)
    , side((center_to_spotted_point.vector_product(up)).normalize())
    {}

std::vector<Point3> Camera::pixels_location(int width, int heigth) {
  float half_image_plane_height = zmin * std::tan(beta); //tan takes its input in radian so beta and alpha should be
  //converted beforehand
  float half_image_plane_width = zmin * std::tan(alpha);
  Point3 first_pixel = half_image_plane_height * up - half_image_plane_width * side + center_image_plane;
  std::vector<Point3> pixels_location{first_pixel};

  float unit_x = (half_image_plane_width / (float)width) * 2.0;
  float unit_y = (half_image_plane_height / (float)heigth) * 2.0;
  this->unit_x_vector = side * unit_x;
  this->unit_y_vector = up * unit_y;
  Point3 current_pixel_height = first_pixel;
  for (int i = 0; i < heigth; ++i) {
      Point3 current_pixel = current_pixel_height;
      for (int j = 0; j < width; ++j) {
          pixels_location.push_back(current_pixel);
          current_pixel = current_pixel + unit_x_vector;
      }
      current_pixel_height = current_pixel_height - unit_y_vector;
  }
  return pixels_location;
}
