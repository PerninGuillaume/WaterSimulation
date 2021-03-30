#include "Scene.hh"
#include <iostream>
#include <cmath>
#include <random>

#include "Vector3.hh"

Scene::Scene(Camera camera, unsigned int max_bounces)
    : camera(camera)
    , max_bounces(max_bounces)
{}

PointIntersection::PointIntersection()
    : is_intersecting(false)
    , intersecting_object(nullptr)
    , intersection_point(Point3())
    , caracteristics(Caracteristics())
{}

PointIntersection::PointIntersection(bool is_intersecting, std::shared_ptr<Object> intersecting_object,
                                     Point3 intersection_point, Caracteristics caracteristics)
    : is_intersecting(is_intersecting)
    , intersecting_object(intersecting_object)
    , intersection_point(intersection_point)
    , caracteristics(caracteristics){}


void Scene::add_object(const std::vector<std::shared_ptr<Object>>& objects_to_add) {
  objects.insert(objects.end(), objects_to_add.begin(), objects_to_add.end());
}

Scene& Scene::add_object(std::shared_ptr<Object> object) {
  objects.push_back(object);
  return *this;
}

Scene& Scene::add_light(std::shared_ptr<Light> light) {
  lights.push_back(light);
  return *this;
}

void Scene::set_epsilon(double epsilon) {
  this->epsilon = epsilon;
}

bool Scene::is_hidden(const Rayon& ray, double max_t) {
  if (!shadow) {
    return false;
  }

  std::shared_ptr<Object> intersecting_object;
  for (const auto& object : this->objects) {
    if (object->texture_material->caracteristics.index_refraction.has_value()) {
      continue; //We can reach the light eventhough we intersect with a transparant object
    }
    std::optional<double> t = object->is_intersecting(ray);
    if (t) {
      //With t > this->epsilon, and epsilon > 0 we are sure we won't find an intersection behind ourselves
      //with t < max_t, we won't find an intersection behind a light when we want to know if we are in the shadows
      if (t > this->epsilon) {
        if (t > max_t - this->epsilon) {
          continue;
        }
        return true;
      }
    }
  }
  return false;
}

Pixel Scene::diffuse_light(const Point3& intersection_point, const Vector3& normal, const Caracteristics& caracteristics) {
  Pixel diffuse_intensity(0,0,0);
  for (const auto& light : this->lights) {
    Vector3 point_to_light_vector = Vector3(intersection_point, light->origin);
    double point_to_light_norm = point_to_light_vector.norm();
    Vector3 point_to_light = point_to_light_vector.normalize();
    if (is_hidden(Rayon(point_to_light, intersection_point),  point_to_light_norm)) {continue;}

    diffuse_intensity += (caracteristics.pixel * caracteristics.kd * light->colors)
                         * normal.scalar_product(point_to_light, true);
  }
  return diffuse_intensity;
}

Pixel Scene::specular_light(const Point3& intersection_point, const Vector3& reflected_vector, const Caracteristics& caracteristics) {
  Pixel specular_intensity(0,0,0);
  for (const auto& light : this->lights) {
    Vector3 point_to_light_vector = Vector3(intersection_point, light->origin);
    double point_to_light_norm = point_to_light_vector.norm();
    Vector3 point_to_light = point_to_light_vector.normalize();
    if (is_hidden(Rayon(point_to_light, intersection_point),  point_to_light_norm)) {continue;}

    specular_intensity += caracteristics.ks
                          * std::pow(reflected_vector.scalar_product(point_to_light, true), caracteristics.ns) * light->colors;
    //TODO try to remove this, added a 10 times coefficient otherwise we did not see the specular light
  }
  return specular_intensity;
}

double Scene::fresnel(const Vector3& incident, const Vector3& normal, double index_refraction) {
  double cos_i = normal.scalar_product(incident);
  double ni = 1;
  double nt = index_refraction;
  if (cos_i > 0) { //Glass to air
    std::swap(ni, nt);
  }
  else {
    cos_i = -1.0 * cos_i;
  }
  double sin_out = (ni / nt) * std::sqrt(1 - std::pow(cos_i, 2));
  if (sin_out >= 1) { //Total internal reflection
    return 1.0;
  }
  //double cos_t = std::sqrt(1 - std::pow(sin_out, 2));
  //Wikipedia formula
  //double Rs = ((ni * cos_i) - (nt * cos_t)) / ((ni * cos_i) + (nt * cos_t));
  //double Rp = ((ni * cos_t) - (nt * cos_i)) / ((ni * cos_t) + (nt * cos_i));
  //return 0.5 * (Rs * Rs + Rp * Rp);

  //Schlik's approximation
  double R0 = std::pow((ni - nt) / (ni + nt), 2);
  return R0 + (1 - R0) * std::pow(1 - cos_i, 5);

}

PointIntersection Scene::find_intersection(Rayon ray) {
  // for reflection and refraction instead of discarding small t, translate the origin of the ray along the ray normal
  ray.origin = ray.origin + ray.direction * epsilon;
  std::optional<double> t_min;
  std::shared_ptr<Object> intersecting_object;
  for (const auto& object : this->objects) {
    std::optional<double> t = object->is_intersecting(ray);
    if (t) {
      //With t > this->epsilon, and epsilon > 0 we are sure we won't find an intersection behind ourselves
      if (t > this->epsilon && (!t_min || t < t_min.value())) {
        t_min = t;
        intersecting_object = object;
      }
      //if (!t_min || t < t_min.value()) {
      //  t_min = t;
      //  intersecting_object = object;
      //}
    }
  }
  if (intersecting_object == nullptr) {
    return PointIntersection();
  }
  Point3 intersection_point = ray.origin + ray.direction * t_min.value();
  Caracteristics caracteristics = intersecting_object->texture_at_point(intersection_point);
  return PointIntersection(true, intersecting_object, intersection_point, caracteristics);
}


//TODO do not clamp before the end, use reinhard function to clamp or gamma
Pixel Scene::raycast(const Rayon& ray, unsigned int bounces) {
  if (bounces == 0) {
    return Pixel(0,0,0);
  }
  PointIntersection struct_intersection = this->find_intersection(ray);
  if (!struct_intersection.is_intersecting) {
    return Pixel(0, 0, 0);
  }
  Pixel result(0,0,0);
  Caracteristics caracteristics = struct_intersection.caracteristics;
  bool transparent = refraction && caracteristics.index_refraction.has_value();
  auto intersection_point = struct_intersection.intersection_point;
  auto intersecting_object = struct_intersection.intersecting_object;

  Vector3 normal = intersecting_object->normal_at_point(intersection_point, ray);
  Vector3 incident_vector = (Vector3(ray.origin, intersection_point)).normalize();
  Vector3 reflected_vector = reflection_vector(incident_vector, normal);

  if (transparent) {
    double kr = this->fresnel(incident_vector, normal, caracteristics.index_refraction.value());
    Pixel refrac(0,0,0);
    if (kr < 1.0) {
      auto refraction = refraction_vector(incident_vector, normal, caracteristics.index_refraction.value());
      //We should not be in the case of TIR because kr < 1.0
      refrac = this->raycast(Rayon(refraction.value(), intersection_point), bounces - 1);
    }
    Pixel reflex = caracteristics.ks * this->raycast(Rayon(reflected_vector, intersection_point), bounces - 1);
    result += reflex * kr + refrac * (1.0 - kr);
  }
  else {
    if (diffusion) {
      result += this->diffuse_light(intersection_point, normal, caracteristics);
    }
    if (specularity) {
      result += this->specular_light(intersection_point, reflected_vector, caracteristics);
    }
    if (reflection) {
      result += caracteristics.ks * this->raycast(Rayon(reflected_vector, intersection_point), bounces - 1);
    }
  }
  return result;
}

Image Scene::raycasting() {
  Image image(width, height);
  auto pixels_location = this->camera.pixels_location(width, height);
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_real_distribution<> distr(-0.5, 0.5); // define the range
  int loading = 0;
  int nb_pixels = height * width;
  int displayed = 0;
  for (const auto& pixel_location : pixels_location) {
    if (this->msaa_samples == 1) {
      Rayon ray(Vector3(this->camera.center, pixel_location).normalize(), this->camera.center);
      auto pixel = this->raycast(ray, this->max_bounces);
      image.pixels.push_back(pixel);
      continue;
    }
    double red = 0.0, green = 0.0, blue = 0.0;
    for (int i = 0; i < this->msaa_samples; ++i) {
      auto random_location = pixel_location + distr(gen) * this->camera.unit_x_vector + distr(gen) * this->camera.unit_y_vector;
      Rayon ray(Vector3(this->camera.center, random_location).normalize(), this->camera.center);
      auto pixel = this->raycast(ray, this->max_bounces);
      red += pixel.x;
      green += pixel.y;
      blue += pixel.z;
    }
    red /= this->msaa_samples;
    green /= this->msaa_samples;
    blue /= this->msaa_samples;
    image.pixels.emplace_back(red, green, blue);
    ++loading;
    int percentage = 100 * loading / nb_pixels;
    if (percentage > displayed) {
      std::cout << ' ' << displayed << ' ' << std::flush;
      ++displayed;
    }
  }
  return image;
}
