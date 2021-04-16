#include "Moteur.hh"
#include <optional>
#include <math.h>
#include <iostream>

#define PI 3.14159265

Camera create_standard_camera() {
  Point3 center(10,10,5);
  Point3 spotted_point(0,0,0);
  Vector3 up(0,0,1);
  float alpha = 60.64;//For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  return camera;
}

void simple_ray_casting() {
    Scene scene = Scene(create_standard_camera(), 1);
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
    Scene scene = Scene(create_standard_camera(), 5);
    Caracteristics caracteristics_red(Pixel(255,0,0), 0.05, 0.5, 2);
    Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.5, 2);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.5, 0.5, 2);

    auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_red), Point3(5,-5,0), 1.0);
    auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(5, 5, 0), 1.0);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(50,0,0), Vector3(-1,0,0));
    scene.add_object({sphere1, sphere2, plane});
    auto light = std::make_shared<Point_Light>(Point3(5,0,10), 3);
    scene.add_light(light);
    Image image = scene.raycasting();
    image.save_as_ppm("images/ray_diffuse_casting.ppm");
}

void sphere_on_simple_plane() {
    Scene scene = Scene(create_standard_camera(), 5);
    Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
    Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.1, 0.3, 1);
    auto sphere = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(4,0,2), 1);
    auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
    scene.add_object({sphere, plane});
    auto light = std::make_shared<Point_Light>(Point3(2,0,0), 10);
    scene.add_light(light);
    Image image = scene.raycasting();
    image.save_as_ppm("images/sphere_on_blue_plane.ppm");
}

void triangle_on_plane() {
  Scene scene = Scene(create_standard_camera(), 5);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0.3, 1);
  auto triangle1 = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_green),
          Point3(4,-1,1), Point3(4.5,-1,-1), Point3(3,1,0));
  auto triangle2 = std::make_shared<Triangle>(std::make_shared<Uniform_Texture>(caracteristics_green),
          Point3(4,-1,1), Point3(4.5,-1,-1), Point3(3,1,-2));
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object({triangle1, triangle2, plane});
  auto light = std::make_shared<Point_Light>(Point3(2,0,0), 1000);
  scene.add_light(light);
  Image image = scene.raycasting();
  image.save_as_ppm("images/triangle_on_plane.ppm");
}

void smooth_triangle_on_plane() {
  Scene scene = Scene(create_standard_camera(), 1);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0, 0);
  Point3 A(2,1,-0.5);//All those points are at the boundaries of the center_sphere of radius 1
  Point3 B(2,0,0.5);
  Point3 C(2,-1,-0.5);
  Point3 center_sphere(3,0,-0.5);
  auto triangle1 = std::make_shared<SmoothTriangle>(std::make_shared<Uniform_Texture>(caracteristics_green),
                                              A, B, C, A - center_sphere, B - center_sphere, C - center_sphere);
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object({triangle1, plane});
  auto light = std::make_shared<Point_Light>(Point3(0,0,3), 1000);
  auto light2 = std::make_shared<Point_Light>(Point3(2,0,-0.5), 500);
  scene.add_light(light);
  //scene.add_light(light2);
  Image image = scene.raycasting();
  image.save_as_ppm("images/smooth_triangle_on_plane.ppm");
}

void two_spheres_on_plane() {
  Scene scene = Scene(create_standard_camera(), 5);
  scene.msaa_samples = 4;
  Caracteristics caracteristics_blue(Pixel(0, 0, 70), 0.1, 0.5, 1);
  Caracteristics caracteristics_yellow(Pixel(255, 255, 0), 0.6, 0.3, 100);
  Caracteristics caracteristics_grey(Pixel(127, 128, 137), 0.2, 0.3, 1);

  auto sphere1 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_grey), Point3(4,3,2), 1);
  auto sphere2 = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_yellow), Point3(4,-3,2), 1);
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  scene.add_object({sphere1, sphere2, plane});
  auto light = std::make_shared<Point_Light>(Point3(4,0,2), 500);
  scene.add_light(light);

  Image image = scene.raycasting();
  image.save_as_ppm("images/two_spheres_on_plane.ppm");
}



void sphere_anti_aliased() {
  Scene scene = Scene(create_standard_camera(), 1);
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
  //TODO why is there a blue line on top of the spheres; maybe because of reflection
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,-1,3.13)), scene.camera.center), 2) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,-1,3.24)), scene.camera.center), 2) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,-1,3.4)), scene.camera.center), 2) << '\n';
  Image image = scene.raycasting();
  image.save_as_ppm("images/anti_aliased.ppm");
}

void simple_plane() {
  Scene scene = Scene(create_standard_camera(), 5);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.5, 0.5, 2);
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(10,0,0), Vector3(-1,0,0));
  scene.add_object(plane);
  auto light = std::make_shared<Point_Light>(Point3(5,0,0), 10);
  scene.add_light(light);
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
  Scene scene = Scene(create_standard_camera(), 1);
  scene.msaa_samples = 1;
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0, 1);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Blob blob = Blob(Point3(4.5,0.2,-0.3), 12, 0.1, std::vector<Point3>{Point3(4,1.2,1), Point3(4, -1.2, 1)}, 1, std::make_shared<Uniform_Texture>(caracteristics_green));

  blob.marching_cubes(scene);
  std::cout << scene.objects.size() << '\n';
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(5,0,0), Vector3(-1,0,0));
  //scene.add_object(plane);
  auto light = std::make_shared<Point_Light>(Point3(2,0,0), 1000);
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,1.2,-0.01)), scene.camera.center), 1) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,1.2,-0.02)), scene.camera.center), 1) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,1.2,-0.1)), scene.camera.center), 1) << '\n';
  //TODO debug this thing, we have green then dark then green
  //for (double i = 0; i < 0.2; i += 0.01) {
  //  Point3 arrival(4,1.2, 0 - i);
  //  std::cout << arrival << " : ";
  //  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, arrival), scene.camera.center), 1) << '\n';
  //}
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

void displacement() {
  Point3 center(0, 0, 2);
  Point3 spotted_point(4, 0, 0);
  Vector3 up(0, 0, 1);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 2);
  Point3 A(2, -3, -1);
  Point3 B(2, 3, -1);
  Point3 C(4,3,-1);
  Point3 D(4, -3, -1);
  Caracteristics caracteristics_blue(Pixel(120, 120, 120), 0.8, 0.4, 0);
  Caracteristics caracteristics_green(Pixel(0, 0, 120), 0.8, 0.8, 0);
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_blue);
  rectangle_displaced_by_noise(scene, A, B, C , D, 4, 4, texture, true, false);

  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(0,0,-2), Vector3(0,0,1));
  auto sphere = std::make_shared<Sphere>(std::make_shared<Uniform_Texture>(caracteristics_green), Point3(3.5,0,-1), 0.5);
  //auto triangle = std::make_shared<SmoothTriangle>(texture, Point3(2,0,1), Point3(4,3,1), Point3(4,0,1),
  //                                                 Vector3(-0.379586,0.925,0), Vector3(0.3431,0.9392,0), Vector3(0.492,0.8765,0));
  //scene.add_object(triangle);
  auto light = std::make_shared<Point_Light>(Point3(3, 0, 3), 1000);
  scene.add_light(light);
  //scene.add_object(sphere);
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(3,1.8,-1)), scene.camera.center), 2) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(3,1.9,-1)), scene.camera.center), 2) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(3,2.14,-1)), scene.camera.center), 2) << '\n';
  //for (double i = 0; i < 0.5; i += 0.01) {
  //  Point3 arrival(3,1.8 + i, -1);
  //  std::cout << arrival << " : ";
  //  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, arrival), scene.camera.center), 2) << '\n';
  //}
  Image image = scene.raycasting();
  image.save_as_ppm("images/displacement.ppm");
}

void perlin_noise_2d() {
  PerlinNoise perlinNoise(12);
  unsigned imageWidth = 512;
  unsigned imageHeight = 512;
  double frequency = 0.05;
  Image image(imageWidth, imageHeight);
  for (unsigned i = 0; i < imageHeight; ++i) {
    for (unsigned j = 0; j < imageWidth; ++j) {
      double noise_value = (perlinNoise.eval(Point3(j, i, 0) * frequency) + 1) * 0.5;
      noise_value *= 255;
      std::cout << noise_value << '\n';
      image.pixels.emplace_back(Pixel(noise_value, noise_value, noise_value));
    }
  }
  image.save_as_ppm("images/noise.ppm");
}

void displacement_texture() {
  Point3 center(0, 0, 2);
  Point3 spotted_point(4, 0, 0);
  Vector3 up(0, 0, 1);
  float alpha = 45.0;
  float beta = 45.0;
  float zmin = 1.0; //Why changing this parameter does not affect the output image
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  Scene scene = Scene(camera, 5);
  scene.msaa_samples = 4;
  Point3 A(2, -3, -1);
  Point3 B(2, 3, -1);
  Point3 C(4,3,-1);
  Point3 D(4, -3, -1);
  Caracteristics caracteristics_blue(Pixel(120, 120, 120), 0.8, 0.8, 0);
  auto texture = std::make_shared<Image_Texture>(caracteristics_blue, "images/water_texture.ppm");
  rectangle_displaced_by_noise(scene, A, B, C , D, 4, 4, texture, true, true);
  //auto triangle = std::make_shared<SmoothTriangle>(texture, Point3(2,0,-1.13284), Point3(4,3,-0.9516), Point3(4,0,-1.075809),
  //                                                 Vector3(-0.379586,0.925,0), Vector3(0.3431,0.9392,0), Vector3(0.492,0.8765,0));
  //scene.add_object(triangle);
  auto light = std::make_shared<Point_Light>(Point3(3, 0, 3), 1000);
  scene.add_light(light);
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(2.42,0.5,-1)), scene.camera.center), 1) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(2.41,0.5,-1)), scene.camera.center), 1) << '\n';
  //for (double i = 0; i < 2; i += 0.01) {
  //  Point3 arrival(4 - i,0.5, -1);
  //  std::cout << arrival << " : ";
  //  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, arrival), scene.camera.center), 1) << '\n';
  //}
  Image image = scene.raycasting();
  image.save_as_ppm("images/displacement_texture.ppm");
}

void obj() {
  Scene scene = Scene(create_standard_camera(), 2);
  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.2, 0.5, 1);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0.6, 1 );
  auto plane = std::make_shared<Plane>(std::make_shared<Uniform_Texture>(caracteristics_blue), Point3(0,0,-1), Vector3(0,0,1));
  scene.add_object(plane);
  auto light = std::make_shared<Point_Light>(Point3(2,0,2), 1000);
  auto light_2 = std::make_shared<Point_Light>(Point3(2,4.5,2), 1000);
  auto light_3 = std::make_shared<Point_Light>(Point3(2,-4.5,2), 1000);
  scene.add_light(light);
  scene.add_light(light_2);
  //scene.add_light(light_3);
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_green);
  create_mesh_from_obj(scene, texture, "images/geometry/smooth_cylinder.obj");
  /*std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,0,-0.73)), scene.camera.center), 2) << '\n';
  std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, Point3(4,0,-1.01)), scene.camera.center), 2) << '\n';
  for (double i = 0; i < 2; i += 0.01) {
    Point3 arrival(4,0, 0.5 - i);
    std::cout << arrival << " : ";
    std::cout << scene.raycast(Rayon(Vector3(scene.camera.center, arrival), scene.camera.center), 2) << '\n';
  }*/
  Image image = scene.raycasting();
  image.save_as_ppm("images/obj.ppm");
  //Y forward Z up triangulisation
}

void create_sky_in_scene(Scene& scene) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0, 0);
  const std::string filename = "images/muntain_scene/sky.ppm";
  const std::string filename_blue = "images/muntain_scene/blue_sky.ppm";


  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{20,-20,-20}, {20, -20, 20}, {-20, -20, 20}, {-20, -20, -20}};
  std::vector<Vector3> normals = {{0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}};
  std::vector<Point3> textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);

  points = {{20,20,-20}, {20, 20, 20}, {-20, 20, 20}, {-20, 20, -20}};
  normals = {{0,-1,0}, {0,-1,0}, {0,-1,0}, {0,-1,0}};
  textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);

  points = {{20, -20, 20}, {-20, -20, 20}, {-20, 20, 20}, {20, 20, 20}};
  normals = {{0,0,-1}, {0,0,-1}, {0,0,-1}, {0,0,-1}};
  textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);

  points = {{20, -20, -20}, {-20, -20, -20}, {-20, 20, -20}, {20, 20, -20}};
  normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);

  points = {{20,-20,-20}, {20, -20, 20}, {20, 20, 20}, {20, 20, -20}};
  normals = {{-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}};
  textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);

  points = {{-20,-20,-20}, {-20, -20, 20}, {-20, 20, 20}, {-20, 20, -20}};
  normals = {{1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}};
  textureCoordinates = {{0,1,0}, {0,0,0}, {1,0,0}, {1,1,0}};
  triangleMesh(scene, std::make_shared<Image_Texture>(caracteristics_blue, filename), faceIndex, vertexIndices
               , points, normals, textureCoordinates);
}

void create_muntain_water_in_scene(Scene& scene, float water_amplitude, float water_z) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.1, 0.3, 0.4);//, 1.33);
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_blue);
  //Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0.8, 0);
  //auto texture = std::make_shared<Image_Texture>(caracteristics_blue, "images/muntain_scene/TEX_water.ppm");

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{15, -15, water_z}, {0, -15, water_z}, {0, 15, water_z}, {15, 15, water_z}};
  std::vector<Vector3> normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  //triangleMesh(scene, texture, faceIndex, vertexIndices, points, normals, textureCoordinates);
  rectangle_displaced_by_noise(scene, points[0], points[1], points[2], points[3], 40, 40, texture, false, true, water_amplitude);
}

void muntain(Camera camera, int image_num, float water_amplitude = 0.3f, float water_z = 0.5f) {
  Scene scene = Scene(camera, 5);
  Caracteristics caracteristics_green(Pixel(0, 255, 0), 0.4, 0, 1);
  
  //lights
  auto light = std::make_shared<Point_Light>(Point3(2,0.5,2), 1000);
  auto light_2 = std::make_shared<Point_Light>(Point3(2,4.5,2), 1000);
  auto light_3 = std::make_shared<Point_Light>(Point3(2,-4.5,2), 1000);
  auto light_4 = std::make_shared<Point_Light>(Point3(18,0,18), 1000);
  scene.add_light(light);
  scene.add_light(light_2);
  scene.add_light(light_3);
  scene.add_light(light_4);

  //sky, water and muntain
  create_sky_in_scene(scene);
  create_muntain_water_in_scene(scene, water_amplitude, water_z);
  auto texture = std::make_shared<Image_Texture>(caracteristics_green, "images/muntain_scene/TEX_muntain.ppm");
  create_mesh_from_obj(scene, texture, "images/muntain_scene/OBJ_muntain_1000.obj");
  
  Image image = scene.raycasting();
  image.save_as_ppm("images/muntain_water" + std::to_string(image_num) + ".ppm");
}

void muntain_up_views() {
  Point3 spotted_point(10,0,0);
  Vector3 up(0,0,1);
  float alpha = 60.64;//For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 1;
  for (float f=0.0; f<5; f+=0.1) {
    Point3 center(0, 0, f);
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    muntain(camera, image_num);
    image_num++;
  }
}

void muntain_dezoom_views() {
  Point3 spotted_point(20,0,1);
  Vector3 up(0,0,1);
  float alpha = 60.64;//For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  float circle_radius = 6;
  int image_num = 1;
  for (float angle=0.0; angle < PI / 2; angle+=0.2) {
    Point3 center(7 - sin(angle) * circle_radius, 0, 7 - cos(angle) * circle_radius);
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    muntain(camera, image_num);
    image_num++;
  }
}

void muntain_water_changing_views() {
  Point3 spotted_point(20,0,1);
  Point3 center(2, 0, 2);
  Vector3 up(0,0,1);
  float alpha = 60.64;//For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 1;
  float water_z = 0.5f;
  Camera camera(center, spotted_point, up, alpha, beta, zmin);
  for (float amplitude=0.0; amplitude < 0.5; amplitude += 0.05) {
    muntain(camera, image_num, amplitude);
    image_num++;
  }
  for (float amplitude=0.5; amplitude < 1.5; amplitude += 0.1) {
    muntain(camera, image_num, amplitude, water_z);
    water_z += 0.075;
    image_num++;
  }
}

void create_boat_water_in_scene(Scene& scene, float water_height) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.1, 0.3, 0.4);//, 1.33); // boat sinking add refraction
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_blue);
  //Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0.8, 0);
  //auto texture = std::make_shared<Image_Texture>(caracteristics_blue, "images/muntain_scene/TEX_water.ppm");

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{20, -20, water_height}, {-20, -20, water_height}, {-20, 20, water_height}, {20, 20, water_height}};
  std::vector<Vector3> normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  //triangleMesh(scene, texture, faceIndex, vertexIndices, points, normals, textureCoordinates);
  rectangle_displaced_by_noise(scene, points[0], points[1], points[2], points[3], 40, 40, texture, false, true, 0.5f);
}

void create_boat_moving_water_in_scene(Scene& scene, float x_offset) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.1, 0.3, 0.4);//, 1.33); // boat sinking add refraction
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_blue);
  //Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0.8, 0);
  //auto texture = std::make_shared<Image_Texture>(caracteristics_blue, "images/muntain_scene/TEX_water.ppm");

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{21 + x_offset, -30, 0.5}, {-21 + x_offset, -21, 0.5}, {-21 + x_offset, 21, 0.5}, {21 + x_offset, 21, 0.5}};
  std::vector<Vector3> normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  //triangleMesh(scene, texture, faceIndex, vertexIndices, points, normals, textureCoordinates);
  rectangle_displaced_by_noise(scene, points[0], points[1], points[2], points[3], 20, 20, texture, false, true, 0.2f);
}

void create_sink_boat_water_in_scene(Scene& scene, float water_height) {

  Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.1, 0.3, 0.4, 1.33); // boat sinking add refraction
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_blue);
  //Caracteristics caracteristics_blue(Pixel(0, 0, 255), 0.8, 0.8, 0);
  //auto texture = std::make_shared<Image_Texture>(caracteristics_blue, "images/muntain_scene/TEX_water.ppm");

  std::vector<int> faceIndex = {4};
  std::vector<int> vertexIndices = {0,1,2,3};
  std::vector<Point3> points = {{20, -20, water_height}, {-20, -20, water_height}, {-20, 20, water_height}, {20, 20, water_height}};
  std::vector<Vector3> normals = {{0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}};
  std::vector<Point3> textureCoordinates = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
  //triangleMesh(scene, texture, faceIndex, vertexIndices, points, normals, textureCoordinates);
  rectangle_displaced_by_noise(scene, points[0], points[1], points[2], points[3], 40, 40, texture, false, true);
}

void boat(Camera camera, int image_num, float water_height=0.5f, bool sinking_boat=false, float moving_water_offset=9999.0f) {
  Scene scene = Scene(camera, 4);
  Caracteristics caracteristics_red(Pixel(255, 0, 0), 0.5, 0, 1);
  
  //lights
  auto light_1 = std::make_shared<Point_Light>(Point3(15,15,18), 1000);
  auto light_2 = std::make_shared<Point_Light>(Point3(15,-15,18), 1000);
  auto light_3 = std::make_shared<Point_Light>(Point3(-15,-15,18), 1000);
  auto light_4 = std::make_shared<Point_Light>(Point3(-15,15,18), 1000);
  auto light_5 = std::make_shared<Point_Light>(Point3(0,15,15), 1000);
  auto light_6 = std::make_shared<Point_Light>(Point3(15,-0,15), 1000);
  auto light_7 = std::make_shared<Point_Light>(Point3(0,-15,15), 1000);
  auto light_8 = std::make_shared<Point_Light>(Point3(-15,0,15), 1000);
  auto light_9 = std::make_shared<Point_Light>(Point3(0,0,15), 1000);
  scene.add_light(light_1);
  scene.add_light(light_2);
  scene.add_light(light_3);
  scene.add_light(light_4);
  scene.add_light(light_5);
  scene.add_light(light_6);
  scene.add_light(light_7);
  scene.add_light(light_8);

  //sky, water and muntain
  create_sky_in_scene(scene);
  auto texture = std::make_shared<Uniform_Texture>(caracteristics_red);

  if (sinking_boat) {
    create_sink_boat_water_in_scene(scene, water_height);
    create_mesh_from_obj(scene, texture, "images/boat_scene/OBJ_boat_sink.obj");
    Image image = scene.raycasting();
    image.save_as_ppm("images/sink_boat_" + std::to_string(image_num) + ".ppm");
  }
  else {
    if (moving_water_offset != 9999)
      create_boat_moving_water_in_scene(scene, moving_water_offset);
    else
      create_boat_water_in_scene(scene, water_height);
    create_mesh_from_obj(scene, texture, "images/boat_scene/OBJ_boat.obj");
    Image image = scene.raycasting();
    image.save_as_ppm("images/boat_" + std::to_string(image_num) + ".ppm");
  }
}

void circle_boat_views_1() {
  Point3 spotted_point(0,0,0);
  Vector3 up(0,0,1);
  float circle_radius = 18;
  float camera_z = 4;
  float alpha = 60.64; //For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 1;
  for (float angle=0.2; angle < 6.4; angle+=0.4) {
    Point3 center(cos(angle) * circle_radius, sin(angle) * circle_radius, camera_z);
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    boat(camera, image_num, 0.5);
    image_num++;
  }
}

void circle_boat_views_2() {
  Point3 spotted_point(0,0,0);
  Vector3 up(0,0,1);
  float circle_radius = 18;
  float camera_z = 4;
  float alpha = 60.64; //For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 1;
  for (float angle=0.0; angle < 6.4; angle+=0.4) {
    Point3 center(cos(angle) * circle_radius, sin(angle) * circle_radius, camera_z);
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    boat(camera, image_num, 0.5);
    image_num++;
  }
}

void sink_boat_views() {
  Point3 spotted_point(0,0,0);
  Vector3 up(0,0,1);
  float circle_radius = 18;
  float camera_z = 4;
  float alpha = 60.64; //For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 1;
  for (float z = -15.0; z<10; z+=0.5) {
    Point3 spotted_point(0,0,0);
    Vector3 up(0,0,1);
    Point3 center(cos(PI * 3/4) * circle_radius, sin(PI * 3/4) * circle_radius, camera_z);
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    boat(camera, image_num, z, true);
    image_num++;
  }
}

void boat_moving_water_views() {
  Point3 spotted_point(0,0,0);
  Vector3 up(0,0,1);
  float circle_radius = 19;
  float camera_z = 4;
  Point3 center(cos(0.8) * circle_radius, sin(0.8) * circle_radius, camera_z);
  float alpha = 60.64; //For a ratio of 16/9
  float beta = 45.0;
  float zmin = 1.0;
  int image_num = 6;
  for (float water_offset = -0.4; water_offset < 1; water_offset+=0.1) {
    Camera camera(center, spotted_point, up, alpha, beta, zmin);
    boat(camera, image_num, 0.5, false, water_offset);
    image_num++;
  }
}

//TODO change the two planes in refraction test
int main() {
  //refraction_sphere_on_plane();
  //displacement_texture();
  //displacement();
  //perlin_noise_2d();
  //polygon();
  //blob_test();
  //triangle_on_plane();
  //smooth_triangle_on_plane();
  //simple_plane();
  //two_spheres_on_plane();
  //sphere_anti_aliased();
  //obj();
  //nice_scene_different_views();
  //muntain_up_views();
  
  //muntain(create_standard_camera(), 99);
  //muntain_water_changing_views();
 
  //sink_boat_views();
  //muntain_dezoom_views();
  
  // Pierrick :
  //boat_moving_water_views();
  
  // Guillaume :
  //circle_boat_views_1();
  //circle_boat_views_2();
}


