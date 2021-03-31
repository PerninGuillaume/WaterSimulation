#pragma once
#include <vector>
#include "Object.hh"
#include "Scene.hh"

void triangleMesh(Scene& scene, std::shared_ptr<Texture_Material> texture_material, const std::vector<int> &faceIndex,
               const std::vector<int> &vertexIndices, std::vector<Point3> points, const std::vector<Vector3>& normals,
               const std::vector<Point3>& textureCoordinates);