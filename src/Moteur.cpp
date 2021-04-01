#include "Moteur.hh"
#include <optional>
#include <iostream>
#include <cassert>

void simple_ray_casting() {
    Point3 center(0,0,0);
    Point3 spotted_point(2,0,0);
    Vector3 up(0,0,1);
    float alpha = 45.0;
    float beta = 45.0;
    float zmin = 1;
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    Scene scene = Scene(camera, 1);
    Caracteristics caracteristics_red(Pixel(255,0,0), 0.05, 0.5, 2);
    Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.5, 2);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.5, 0.5, 2);

    auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_red), Point3(10,0,0), 1.0);
    auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(10, 5, 0), 1.0);
    auto sphere3 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(8, 0, 0), 0.5);
    scene.add_object({sphere1, sphere2, sphere3});
    Image image = scene.raycasting();
    image.save_as_ppm("images/simple_ray_casting.ppm");
}

void intermediate() {
    //Image pp3_image("images/p3.ppm");
    //pp3_image.save_as_ppm("images/result_p3.ppm");
    Point3 center(0,0,0);
    Point3 spotted_point(2,0,0);
    Vector3 up(0,0,1);
    float alpha = 70.0;
    float beta = 45.0;
    float zmin = 1.0; //Why changing this parameter does not affect the output image
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    Scene scene = Scene(camera, 5);
    Caracteristics caracteristics_red(Pixel(255,0,0), 0.05, 0.5, 2);
    Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.5, 2);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.5, 0.5, 2);

    auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_red), Point3(5,-5,0), 1.0);
    auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(5, 5, 0), 1.0);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(50,0,0), Vector3(-1,0,0));
    scene.add_object({sphere1, sphere2, plane});
    auto light = std::make_shared<Point_Light>(Point3(5,0,10), 3);
    scene.add_light(light);
    scene.raycast(Rayon(Vector3(1,0,0), center), scene.max_bounces);
    Image image = scene.raycasting();
    image.save_as_ppm("images/ray_diffuse_casting.ppm");
}

void sphere_on_simple_plane() {
    Point3 center(0,0,0);
    Point3 spotted_point(2,0,0);
    Vector3 up(0,0,1);
    float alpha = 45.0;
    float beta = 45.0;
    float zmin = 1.0; //Why changing this parameter does not affect the output image
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    Scene scene = Scene(camera, 5);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
    Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.3, 1);
    auto sphere = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(4,0,2), 1);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
    scene.add_object({sphere, plane});
    auto light = std::make_shared<Point_Light>(Point3(2,0,0), 10);
    scene.add_light(light);
    //scene.raycast(Rayon(Vector3(1,0,0), center), scene.max_bounces);
  std::cout << scene.raycast(Rayon(Vector3(4,0,3.5), center), 1);
    Image image = scene.raycasting();
    image.save_as_ppm("images/sphere_on_blue_plane.ppm");
}

void triangle_on_plane() {
  Point3 center(0,0,0);
  Point3 spotted_point(2,0,0);
  Vector3 up(0,0,1);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0.3, 1);
  auto triangle1 = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_green),
          Point3(4,-1,1), Point3(4.5,-1,-1), Point3(3,1,0));
  auto triangle2 = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_green),
          Point3(4,-1,1), Point3(4.5,-1,-1), Point3(3,1,-2));
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object({triangle1, triangle2, plane});
  auto light = std::make_shared<Point_Light>(Point3(2,0,0), 10);
  scene.add_light(light);
  scene.raycast(Rayon(Vector3(1,0,0), center), scene.max_bounces);
  scene.raycast(Rayon(Vector3(4,0,0), center), 1);
  Image image = scene.raycasting();
  image.save_as_ppm("images/triangle_on_plane.ppm");
}

void two_spheres_on_plane() {
  Point3 center(0,0,0);
  Point3 spotted_point(2,0,0);
  Vector3 up(0,0,1);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  Caracteristics caracteristics_blue(Pixel(0, 0, 70), 0.1, 0.5, 1);
  Caracteristics caracteristics_yellow(Pixel(255, 255, 0), 0.6, 0.3, 100);
  Caracteristics caracteristics_grey(Pixel(127, 128, 137), 0.2, 0.3, 1);

  auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_grey), Point3(4,3,2), 1);
  auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_yellow), Point3(4,-3,2), 1);
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object({sphere1, sphere2, plane});
  auto light = std::make_shared<Point_Light>(Point3(4,0,2), 500);
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(1,0,0), center), scene.max_bounces);

  Image image = scene.raycasting();
  image.save_as_ppm("images/two_spheres_on_plane.ppm");
}

void sphere_anti_aliased() {
    Point3 center(0,0,0);
    Point3 spotted_point(2,0,0);
    Vector3 up(0,0,1);
    float alpha = 45.0;
    float beta = 45.0;
    float zmin = 1.0; //Why changing this parameter does not affect the output image
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    Scene scene = Scene(camera, 4);
    scene.msaa_samples = 4;
    Caracteristics caracteristics_blue(Pixel(0, 0, 70), 0.1, 0.5, 1);
    Caracteristics caracteristics_yellow(Pixel(255, 255, 0), 0.6, 0.3, 2);
    Caracteristics caracteristics_grey(Pixel(127, 128, 137), 0.2, 0.3, 1);

    auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_grey), Point3(4,1,2), 1);
    auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_yellow), Point3(4,-1,2), 1);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
    scene.add_object({sphere2, sphere1, plane});
    auto light = std::make_shared<Point_Light>(Point3(2,0,0), 800);
    scene.add_light(light);


    Image image = scene.raycasting();
    image.save_as_ppm("images/anti_aliased.ppm");
}

void simple_plane() {
    Point3 center(0,0,0);
    Point3 spotted_point(2,0,0);
    Vector3 up(0,0,1);
    float alpha = 45.0;
    float beta = 45.0;
    float zmin = 1.0; //Why changing this parameter does not affect the output image
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    Scene scene = Scene(camera, 5);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.5, 0.5, 2);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(10,0,0), Vector3(-1,0,0));
    scene.add_object(plane);
    auto light = std::make_shared<Point_Light>(Point3(5,0,0), 10);
    scene.add_light(light);
    std::cout << scene.raycast(Rayon(Vector3(1,0,0), center), scene.max_bounces);
    Image image = scene.raycasting();
    image.save_as_ppm("images/blue_plane.ppm");
}
//TODO put two planes intersecting to see the refraction
void refraction_sphere_on_plane() {
  Point3 center(0, 0, 2);
  Point3 spotted_point(4, 0, 0);
  Vector3 up(1, 0, 2);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  scene.msaa_samples = 4;
  scene.set_epsilon(0.001);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0, 1);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.3, 1);
  Caracteristics caracteristics_red(Pixel(255, 0, 0), 0.1, 0.3, 1);
  Caracteristics caracteristics_transparent(Pixel(0, 122, 0), 0.1, 0.3, 1, 1.5);
  auto triangle = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_transparent),
                                             Point3(5,-1,1), Point3(4,-1,-1), Point3(4,1,0));
  auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_red),
                                              Point3(7,2,0), 1);
  auto circle = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_transparent),
                                          Point3(4, 0, 0), 1);
  auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_blue),
                                         Point3(3.3, 1.5, 0), 0.5);
  auto ground = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_green),
          Point3(0, 0, -1), Vector3(0, 0, 1));
  auto sky = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue),
          Point3(12,0,0), Vector3(1,0, 5));
  auto light = std::make_shared<Point_Light>(Point3(2, 0, 0), 5500);
  scene.add_object({circle, sphere1, ground, sphere2});
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(4,0,0.98), center), scene.max_bounces);
  Image image = scene.raycasting();
  image.save_as_ppm("images/refraction_sphere.ppm");
}

void blob_test() {
  Point3 center(0,0,0);
  Point3 spotted_point(2,0,0);
  Vector3 up(0,0,1);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 2);
  scene.msaa_samples = 1;
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0, 1);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Blob blob = Blob(Point3(4.5,0.2,-0.3), 12, 0.3, std::vector<Point3>{Point3(4,1.2,1), Point3(4, -1.2, 1.2)}, 1, std::make_shared<Uniform_Texture>(caracteristics_green));
  blob.marching_cubes(scene);
  std::cout << scene.objects.size() << '\n';
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object(plane);
  auto light = std::make_shared<Point_Light>(Point3(2,0,0), 1000);
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(1,0.1,0), center), scene.max_bounces);
  Image image = scene.raycasting();
  image.save_as_ppm("images/blob.ppm");
}

void create_polygon_in_scene(Scene& scene) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0, 1);
  const std::string filename = "images/wood.ppm";

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{4,-1,-1}, {4, -1, 1}, {4, 1, 1}, {4, 1, -1}};
  std::vector<Vector3> normals = {{-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
}

void polygon() {
  Point3 center(0, 0, 2);
  Point3 spotted_point(4, 0, 0);
  Vector3 up(1, 0, 2);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  scene.msaa_samples = 4;
  scene.set_epsilon(0.001);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.3, 1);
  Caracteristics caracteristics_red(Pixel(255, 0, 0), 0.1, 0.3, 1);
  auto ground = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_green),
                                        Point3(0, 0, -1), Vector3(0, 0, 1));
  auto triangle = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_red),
                                             Point3(5,-1,1), Point3(4,-1,-1), Point3(4,1,0));
  auto light = std::make_shared<Point_Light>(Point3(2, 0, 0), 1000);
  scene.add_object(ground);
  create_polygon_in_scene(scene);
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(4,0,0.98), center), scene.max_bounces);
  Image image = scene.raycasting();
  image.save_as_ppm("images/polygon.ppm");
}

void create_polygon_walls_in_scene(Scene& scene) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0, 1);
  const std::string filename = "images/wood.ppm";

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{4,-1,-1}, {4, -1, 1}, {4, 1, 1}, {4, 1, -1}};
  std::vector<Vector3> normals = {{-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
  points = {{2,-1,-1}, {2, -1, 1}, {4,-1, 1}, {4, -1, -1}};
  normals = {{0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}};
  textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
  points = {{4,-1,-1}, {2, -1, -1}, {2, 1, -1}, {4, 1, -1}};
  normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
}

void create_polygon_water_in_scene(Scene& scene) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0, 0, 0.8);

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{4,-1, 0}, {2, -1, 0}, {2, 1, 0}, {4, 1, 0}};
  std::vector<Vector3> normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  triangleMesh(scene, std::make_shared<Uniform_Texture>(caracteristics_blue), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
}

void pool() {
  Point3 center(0, 0, 2);
  Point3 spotted_point(4, 0, 0);
  Vector3 up(1, 0, 2);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  scene.msaa_samples = 4;
  scene.set_epsilon(0.001);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.3, 1);
  Caracteristics caracteristics_yellow(Pixel(255, 255, 0), 1, 0, 0);
  auto ground = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_green),
                                        Point3(0, 0, -1.1), Vector3(0, 0, 1));

  auto light = std::make_shared<Point_Light>(Point3(2, 0, 0), 1000);
  auto light2 = std::make_shared<Point_Light>(Point3(0, 0, 10), 500);
  auto sphere = std::make_shared<Sphere>(std::make_shared<Procedural_Texture>(caracteristics_yellow), Point3(3,0,-0.75), 0.25);
  scene.add_light(light);
  scene.add_light(light2);
  scene.add_object({ground, sphere});


  create_polygon_walls_in_scene(scene);
  create_polygon_water_in_scene(scene);
  std::cout << scene.raycast(Rayon(Vector3(4,0,0.98), center), scene.max_bounces);
  Image image = scene.raycasting();
  image.save_as_ppm("images/pool.ppm");
}

//TODO change the two planes in refraction test
int main() {
  pool();
  //polygon();
  //refraction_sphere_on_plane();
  //blob_test();
  //triangle_on_plane();
  //simple_plane();
  //two_spheres_on_plane();
  //sphere_anti_aliased();
}


