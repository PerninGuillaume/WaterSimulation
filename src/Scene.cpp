#include "Scene.hh"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <utility>

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
    , intersecting_object(std::move(intersecting_object))
    , intersection_point(intersection_point)
    , caracteristics(std::move(caracteristics)){}


void Scene::add_object(const std::vector<std::shared_ptr<Object>>& objects_to_add) {
  objects.insert(objects.end(), objects_to_add.begin(), objects_to_add.end());
}

Scene& Scene::add_object(const std::shared_ptr<Object>& object) {
  objects.push_back(object);
  return *this;
}

Scene& Scene::add_light(const std::shared_ptr<Light>& light) {
  lights.push_back(light);
  return *this;
}

bool Scene::is_hidden(Rayon ray, double max_t) {
  ray.origin = ray.origin + ray.direction * epsilon;
  if (!shadow) {
    return false;
  }

  std::shared_ptr<Object> intersecting_object;
  for (const auto& object : objects) {
    if (object->texture_material->caracteristics.index_refraction.has_value()) {
      continue; //We can reach the light eventhough we intersect with a transparent object
    }
    double u, v;//We won't use those, just used by the function
    std::optional<double> t = object->is_intersecting(ray, u, v);
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

Pixel Scene::diffuse_light(const Point3& intersection_point, const Caracteristics& caracteristics,
                           const std::shared_ptr<Object>& object, double u, double v) {
  Pixel diffuse_intensity(0,0,0);
  for (const auto& light : this->lights) {
    Vector3 point_to_light_vector = Vector3(intersection_point, light->origin);
    double point_to_light_norm = point_to_light_vector.norm();
    Vector3 point_to_light = point_to_light_vector.normalize();
    if (is_hidden(Rayon(point_to_light, intersection_point),  point_to_light_norm)) {continue;}
    Vector3 normal = object->normal_at_point(intersection_point, Rayon(-point_to_light, light->origin), u, v, true);

    diffuse_intensity += (caracteristics.pixel * caracteristics.kd * light->get_intensity(intersection_point))
                         * normal.scalar_product(point_to_light, true);
  }
  return diffuse_intensity;
}

Pixel Scene::specular_light(const Point3& intersection_point, const Vector3& incident_vector, const Caracteristics& caracteristics,
                            const std::shared_ptr<Object>& object, double u, double v) {
  Pixel specular_intensity(0,0,0);
  for (const auto& light : this->lights) {
    Vector3 point_to_light_vector = Vector3(intersection_point, light->origin);
    double point_to_light_norm = point_to_light_vector.norm();
    Vector3 point_to_light = point_to_light_vector.normalize();
    if (is_hidden(Rayon(point_to_light, intersection_point),  point_to_light_norm)) {continue;}
    Vector3 normal = object->normal_at_point(intersection_point, Rayon(-point_to_light, light->origin),  u, v, true);
    Vector3 reflected_vector = reflection_vector(incident_vector, normal);

    specular_intensity += caracteristics.ks
                          * std::pow(reflected_vector.scalar_product(point_to_light, true), caracteristics.ns)
                          * light->get_intensity(intersection_point);
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

//TODO unify the method for shadow acneing between find_intersection and is_hidden
PointIntersection Scene::find_intersection(Rayon ray, double &u, double &v) {
  // for reflection and refraction instead of discarding small t, translate the origin of the ray along the ray normal
  ray.origin = ray.origin + ray.direction * epsilon;
  std::optional<double> t_min;
  std::shared_ptr<Object> intersecting_object;
  for (const auto& object : this->objects) {
    double u_tmp, v_tmp;
    std::optional<double> t = object->is_intersecting(ray, u_tmp, v_tmp);
    if (t) {
      //With t > this->epsilon, and epsilon > 0 we are sure we won't find an intersection behind ourselves
      if (t > this->epsilon && (!t_min || t < t_min.value())) {
        u = u_tmp;
        v = v_tmp;
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
  Caracteristics caracteristics = intersecting_object->texture_at_point(intersection_point, u, v);
  return PointIntersection(true, intersecting_object, intersection_point, caracteristics);
}


//TODO do not clamp before the end, use reinhard function to clamp or gamma
Pixel Scene::raycast(const Rayon& ray, unsigned int bounces) {
  if (bounces == 0) {
    return Pixel(0,0,0);
  }
  double u,v;
  PointIntersection struct_intersection = this->find_intersection(ray, u, v);
  if (!struct_intersection.is_intersecting) {
    return Pixel(0, 0, 0);
  }
  Pixel result(0,0,0);
  Caracteristics caracteristics = struct_intersection.caracteristics;
  bool transparent = refraction && caracteristics.index_refraction.has_value();
  auto intersection_point = struct_intersection.intersection_point;
  auto intersecting_object = struct_intersection.intersecting_object;

  //TODO see if this works : we take the surface normal instead of the vertex normal here to avoid issue
  //such as a reflection going behind an object because of smooth triangle normals that act as a sphere
  Vector3 normal = intersecting_object->normal_at_point(intersection_point, ray, u, v, use_vertex_normal);
  Vector3 incident_vector = (Vector3(ray.origin, intersection_point)).normalize();
  Vector3 reflected_vector = reflection_vector(incident_vector, normal);

  if (transparent) {
    double kr = this->fresnel(incident_vector, normal, caracteristics.index_refraction.value());
    Pixel refrac(0,0,0);
    if (kr < 1.0) {
      auto refraction_vec = refraction_vector(incident_vector, normal, caracteristics.index_refraction.value());
      //We should not be in the case of TIR because kr < 1.0
      refrac = this->raycast(Rayon(refraction_vec.value(), intersection_point), bounces - 1);
    }
    Pixel reflex = caracteristics.ks * this->raycast(Rayon(reflected_vector, intersection_point), bounces - 1);
    result += reflex * kr + refrac * (1.0 - kr);
  }
  else {
    if (diffusion && caracteristics.kd != 0) {
      result += this->diffuse_light(intersection_point, caracteristics, intersecting_object, u, v);
    }
    if (specularity && caracteristics.ks != 0) {
      result += this->specular_light(intersection_point, incident_vector, caracteristics, intersecting_object, u, v);
    }
    if (reflection && caracteristics.ks != 0) {
      result += 0.8 * caracteristics.ks * this->raycast(Rayon(reflected_vector, intersection_point), bounces - 1);
    }
  }
  return result;
}

void print_time(int seconds) {
  int minutes = seconds / 60;
  int hours = minutes / 60;
  if (hours != 0) {
    std::cout << hours << "h ";
  }
  if (minutes != 0) {
    std::cout << minutes % 60 << "m ";
  }
  std::cout << seconds % 60 << "s";
}

void print_remaining_time(int percentage, std::chrono::time_point<std::chrono::system_clock> start) {
  auto time_step = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(time_step - start);
  auto remaining_time = ((double)duration.count() / (double)percentage) * (100.0 - (double)percentage);
  std::cout << ' ' << percentage << ' ';
  print_time(duration.count());
  std::cout << '/';
  print_time(remaining_time);
  std::cout << " " << std::flush;
  if (percentage == 50)
    std::cout << '\n';
}

Image Scene::raycasting() {
  std::cout << "Number of objects in this scene : " << this->objects.size() << '\n';
  auto start = std::chrono::high_resolution_clock::now();
  Image image(width, height);
  auto pixels_location = this->camera.pixels_location(width, height);
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_real_distribution<> distr(-0.5, 0.5); // define the range
  //We compute one and for all the random anti aliased samples instead of every loop
  std::vector<double> anti_aliased_samples;
  for (int i = 0; i < this->msaa_samples * 2; ++i) {
    anti_aliased_samples.emplace_back(distr(gen));
  }
  int loading = 0;
  const size_t nb_pixels = height * width;
  int displayed = 0;
  #pragma omp parallel for ordered
  for (size_t index_pixel = 0; index_pixel < nb_pixels; ++index_pixel) {
    if (this->msaa_samples == 1) {
      Rayon ray(Vector3(this->camera.center, pixels_location[index_pixel]).normalize(), this->camera.center);
      auto pixel = this->raycast(ray, this->max_bounces);
      image.pixels[index_pixel] = pixel;
    } else {
      double red = 0.0, green = 0.0, blue = 0.0;
      for (int i = 0; i < this->msaa_samples * 2; i += 2) {
        auto random_location = pixels_location[index_pixel] + anti_aliased_samples[i] * this->camera.unit_x_vector
            + anti_aliased_samples[i + 1] * this->camera.unit_y_vector;
        Rayon ray(Vector3(this->camera.center, random_location).normalize(), this->camera.center);
        auto pixel = this->raycast(ray, this->max_bounces);
        red += pixel.x;
        green += pixel.y;
        blue += pixel.z;
      }
      red /= this->msaa_samples;
      green /= this->msaa_samples;
      blue /= this->msaa_samples;
      image.pixels[index_pixel] = Pixel(red, green, blue);
    }
    ++loading;
    int percentage = 100 * loading / nb_pixels;
    if (percentage > displayed) {
      print_remaining_time(percentage, start);
      ++displayed;
    }
  }
  std::cout << "\nFinished render\n";
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
  std::cout << "Execution time : ";
  print_time(duration.count());
  std::cout << " seconds\n";
  return image;
}
