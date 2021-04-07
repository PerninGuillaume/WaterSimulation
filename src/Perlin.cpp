#include <random>
#include "Perlin.hh"

PerlinNoise::PerlinNoise(int seed) {
  std::mt19937 gen(seed);
  std::uniform_real_distribution<> dis;
  dis(gen);
  for (unsigned i = 0; i < tableSize; ++i) {
    //Generate random vector for each of the tableSize elements
    gradients[i] = Vector3( 2 * dis(gen) - 1, 2 * dis(gen) - 1, 2 * dis(gen) - 1).normalize();
  }

}
//Do we want a 3d point or 2D for displacement
double PerlinNoise::eval(const Point3 &p) {
  //The & operator act as a modulo operator in this case but even nicer
  //-10 % 256 = -10
  //-10 & 256 = 246
  //Coordinates of the corners surrounding the point
  int x0 = static_cast<int>(std::floor(p.x)) & tableSizeMask;
  int x1 = (x0 + 1) & tableSizeMask;
  int y0 = static_cast<int>(std::floor(p.y)) & tableSizeMask;
  int y1 = (y0 + 1) & tableSizeMask;
  int z0 = static_cast<int>(std::floor(p.z)) & tableSizeMask;
  int z1 = (z0 + 1) & tableSizeMask;

  //Translation from the bottom of the cube formed by the cell to the point
  double tx = p.x - std::floor(p.x);
  double ty = p.y - std::floor(p.y);
  double tz = p.z - std::floor(p.z);

  double u = smoothStep_5th_order(tx);
  double v = smoothStep_5th_order(ty);
  double w = smoothStep_5th_order(tz);

  //Random vector at each corner
  Vector3 g_000 = gradients[permute(x0, y0, z0)];
  Vector3 g_001 = gradients[permute(x0, y0, z1)];
  Vector3 g_010 = gradients[permute(x0, y1, z0)];
  Vector3 g_011 = gradients[permute(x0, y1, z1)];
  Vector3 g_100 = gradients[permute(x1, y0, z0)];
  Vector3 g_101 = gradients[permute(x1, y0, z1)];
  Vector3 g_110 = gradients[permute(x1, y1, z0)];
  Vector3 g_111 = gradients[permute(x1, y1, z1)];

  //offset vectors from each corner to the candidate point
  Vector3 offset_000 = Vector3(tx, ty, tz);
  Vector3 offset_001 = Vector3(tx, ty, tz - 1);
  Vector3 offset_010 = Vector3(tx, ty - 1, tz);
  Vector3 offset_011 = Vector3(tx, ty - 1, tz - 1);
  Vector3 offset_100 = Vector3(tx - 1, ty, tz);
  Vector3 offset_101 = Vector3(tx - 1, ty, tz - 1);
  Vector3 offset_110 = Vector3(tx - 1, ty - 1, tz);
  Vector3 offset_111 = Vector3(tx - 1, ty - 1, tz - 1);

  // Linear interpolation
  double a = lerp(g_000.scalar_product(offset_000), g_100.scalar_product(offset_100), u);
  double c = lerp(g_001.scalar_product(offset_001), g_101.scalar_product(offset_101), u);
  double b = lerp(g_010.scalar_product(offset_010), g_110.scalar_product(offset_110), u);
  double d = lerp(g_011.scalar_product(offset_011), g_111.scalar_product(offset_111), u);
  //Bi-linear interpolation
  double e = lerp(a, b, v);
  double f = lerp(c, d, v);
  //Tri-linear interpolation
  double g = lerp(e, f, w);

  return g;
}
//Do we want a 3d point or 2D for displacement
double PerlinNoise::eval(const Point3 &p, Vector3& derivatives) {
  //The & operator act as a modulo operator in this case but even nicer
  //-10 % 256 = -10
  //-10 & 256 = 246
  //Coordinates of the corners surrounding the point
  int x0 = static_cast<int>(std::floor(p.x)) & tableSizeMask;
  int x1 = (x0 + 1) & tableSizeMask;
  int y0 = static_cast<int>(std::floor(p.y)) & tableSizeMask;
  int y1 = (y0 + 1) & tableSizeMask;
  int z0 = static_cast<int>(std::floor(p.z)) & tableSizeMask;
  int z1 = (z0 + 1) & tableSizeMask;

  //Translation from the bottom of the cube formed by the cell to the point
  double tx = p.x - std::floor(p.x);
  double ty = p.y - std::floor(p.y);
  double tz = p.z - std::floor(p.z);

  double u = smoothStep_3rd_order(tx);
  double v = smoothStep_3rd_order(ty);
  double w = smoothStep_3rd_order(tz);

  double du = smoothStep_3rd_order_derivative(tx);
  double dv = smoothStep_3rd_order_derivative(ty);
  double dw = smoothStep_3rd_order_derivative(tz);

  //Random vector at each corner
  Vector3 g_000 = gradients[permute(x0, y0, z0)];
  Vector3 g_001 = gradients[permute(x0, y0, z1)];
  Vector3 g_010 = gradients[permute(x0, y1, z0)];
  Vector3 g_011 = gradients[permute(x0, y1, z1)];
  Vector3 g_100 = gradients[permute(x1, y0, z0)];
  Vector3 g_101 = gradients[permute(x1, y0, z1)];
  Vector3 g_110 = gradients[permute(x1, y1, z0)];
  Vector3 g_111 = gradients[permute(x1, y1, z1)];

  //offset vectors from each corner to the candidate point
  Vector3 offset_000 = Vector3(tx, ty, tz);
  Vector3 offset_001 = Vector3(tx, ty, tz - 1);
  Vector3 offset_010 = Vector3(tx, ty - 1, tz);
  Vector3 offset_011 = Vector3(tx, ty - 1, tz - 1);
  Vector3 offset_100 = Vector3(tx - 1, ty, tz);
  Vector3 offset_101 = Vector3(tx - 1, ty, tz - 1);
  Vector3 offset_110 = Vector3(tx - 1, ty - 1, tz);
  Vector3 offset_111 = Vector3(tx - 1, ty - 1, tz - 1);

  double a = offset_000.scalar_product(g_000);
  double b = offset_100.scalar_product(g_100);
  double c = offset_010.scalar_product(g_010);
  double d = offset_110.scalar_product(g_110);
  double e = offset_001.scalar_product(g_001);
  double f = offset_101.scalar_product(g_101);
  double g = offset_011.scalar_product(g_011);
  double h = offset_111.scalar_product(g_111);

  double k0 = (b - a);
  double k1 = (c - a);
  double k2 = (e - a);
  double k3 = (a + d - b - c);
  double k4 = (a + f - b - e);
  double k5 = (a + g - c - e);
  double k6 = (b + c + e + h - a - d - f - g);

  derivatives.x = du *(k0 + v * k3 + w * k4 + v * w * k6);
  derivatives.y = dv *(k1 + u * k3 + w * k5 + u * w * k6);
  derivatives.z = dw *(k2 + u * k4 + v * k5 + u * v * k6);

  return a + u * k0 + v * k1 + w * k2 + u * v * k3 + u * w * k4 + v * w * k5 + u * v * w * k6;
}

int PerlinNoise::permute(int x, int y, int z) {
  return permutations[permutations[permutations[x] + y] + z];
}

double lerp(double v0, double v1, double t) {
  return (1 - t) * v0 + t * v1;
}

double smoothStep_3rd_order(double x) {
  return x * x * (-2 * x + 3);
}

double smoothStep_3rd_order_derivative(double x) {
  return x * (6 - 6 * x);
}

double smoothStep_5th_order(double x) {
  return x * x * x * (x * (x * 6 - 15) + 10);
}

double smoothStep_5th_order_derivative(double x) {
  return 30 * x * x * (x * (x - 2) + 1);
}
