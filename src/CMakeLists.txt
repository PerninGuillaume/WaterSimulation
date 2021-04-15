cmake_minimum_required(VERSION 3.16)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -Wall -Werror -pedantic -fopenmp -Ofast -march=native -flto -funroll-loops")

add_executable(raytracing Moteur.cpp Image.cpp Object.cpp Light.cpp Rayon.cpp Vector.cpp Camera.cpp Scene.cpp Blob.cpp Texture_Material.cpp TriangleMesh.hh TriangleMesh.cpp Perlin.cpp Perlin.hh)
