cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set the project name

project(busqueda
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -Wall: en un laboratorio de punteros las advertencias del compilador avisan
# de varios errores antes de que el programa se caiga. No las ignores.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(test
	tests/main.cpp
	src/node.cpp
	src/llist.cpp
	src/datanode.cpp
	src/stacknode.cpp
	src/stack.cpp
	src/queue.cpp
	src/camino.cpp
	src/jarras.cpp
	src/laberinto.cpp)

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)
