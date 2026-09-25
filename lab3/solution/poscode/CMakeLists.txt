cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set the project name

project(poscode
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -O2: sin optimizacion los tiempos medidos no son representativos.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(test tests/main.cpp src/poscode.cpp src/sort.cpp src/utils.cpp)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)

# El dataset es compartido por starter/ y solution/, asi que vive en lab3/data/.
# Se pasa la ruta absoluta al compilador para que ./test la encuentre sin
# importar desde donde se ejecute.
get_filename_component(LAB_DIR "${PROJECT_SOURCE_DIR}/../.." ABSOLUTE)
target_compile_definitions(test PRIVATE DATA_FILE="${LAB_DIR}/data/codes_1M.bin")

# Las salidas (.csv) se escriben junto al script de Python, no dentro de build/.
target_compile_definitions(test PRIVATE OUTPUT_DIR="${PROJECT_SOURCE_DIR}/python")
