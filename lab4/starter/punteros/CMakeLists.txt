cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set the project name

project(punteros
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -Wall: en un laboratorio de punteros las advertencias del compilador avisan
# de varios errores antes de que el programa se caiga. No las ignores.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(test
	tests/main.cpp
	src/indireccion.cpp
	src/memoria.cpp
	src/arreglos.cpp
	src/funciones.cpp
	src/proteccion.cpp
	src/alcance.cpp)

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)
