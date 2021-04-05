#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include "Object.hh"
#include "Scene.hh"
#include "Perlin.h"

void triangleMesh(Scene& scene, std::shared_ptr<Texture_Material> texture_material, const std::vector<int> &faceIndex,
               const std::vector<int> &vertexIndices, std::vector<Point3> points, const std::vector<Vector3>& normals,
               const std::vector<Point3>& textureCoordinates);

void triangleMesh(Scene& scene, std::shared_ptr<Texture_Material> texture_material, const std::vector<int> &faceIndex
        , const std::vector<int> &vertexIndices, std::vector<Point3> points);

void rectangle_displaced_by_noise(Scene& scene, const Point3& A, const Point3& B, const Point3& C, const Point3& D,
                             unsigned widthDivisions, unsigned heightDivisions, std::shared_ptr<Texture_Material> textureMaterial);