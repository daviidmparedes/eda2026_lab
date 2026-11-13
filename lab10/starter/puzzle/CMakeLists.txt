cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set the project name

project(puzzle
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -Wall: las advertencias del compilador avisan de varios errores con punteros
# antes de que el programa se caiga. -O2: algunas busquedas revisan los 181440
# tableros alcanzables, y sin optimizacion tardan bastante mas.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

add_executable(test
	tests/main.cpp
	src/estado_dado.cpp
	src/estado.cpp
	src/cola.cpp
	src/heapNode.cpp
	src/listaNodos.cpp
	src/maxHeap.cpp
	src/visitados.cpp
	src/puzzle.cpp)

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Los tableros de prueba son compartidos por starter/ y solution/, asi que
# viven en lab10/data/. Se pasa la ruta absoluta para que ./test la encuentre
# sin importar desde donde se ejecute.
get_filename_component(LAB_DIR "${PROJECT_SOURCE_DIR}/../.." ABSOLUTE)
target_compile_definitions(test PRIVATE DATA_FILE="${LAB_DIR}/data/tableros.txt")
