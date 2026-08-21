# Configuración del Entorno

Instrucciones generales para instalar las herramientas del curso y compilar/ejecutar cualquier laboratorio. Esta guía es independiente del laboratorio: se referencia desde cada `labX/README.md`.

## 1. Herramientas requeridas

- **g++** (estándar C++11) — compilador.
- **CMake** (≥ 3.10) — para generar los archivos de compilación de forma independiente del sistema operativo.
- **Visual Studio Code** — editor recomendado, con las extensiones `C/C++` y `CMake Tools`.

## 2. Instalación

### Linux (Ubuntu) — recomendado

```bash
sudo apt update
sudo apt install build-essential cmake
g++ --version      # verificar
cmake --version    # verificar
```

### macOS

```bash
xcode-select --install   # entrega g++/clang y herramientas de compilación
brew install cmake
```

### Windows

En Windows la instalación nativa (MSYS2 + variables de entorno) suele dar problemas y consume tiempo de laboratorio. Por eso, para este curso **estandarizamos el entorno con una máquina virtual Linux**:

1. Instalar [VirtualBox](https://www.virtualbox.org/wiki/Downloads) → sección *Windows Hosts* → aceptar todas las opciones por defecto del instalador.
2. Descargar la VM del curso (`EDAUAndes.ova`) desde el enlace entregado por el equipo docente.
3. En VirtualBox: `File → Import Appliance` y seleccionar el `.ova` descargado.
4. Si se solicita contraseña para la VM, usar `edauandes`.
5. Iniciar la VM: ya trae g++, CMake y VSCode preinstalados.

Alternativa (si no se quiere usar la VM): instalar [MSYS2](https://www.msys2.org/), y desde su terminal:

```bash
pacman -Syu
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

y agregar `C:\msys64\mingw64\bin` al `PATH` del sistema. Esta vía es más propensa a errores (p. ej. `cmake` no encuentra el generador correcto); si aparece el error *"El sistema no puede encontrar el archivo especificado"*, invocar `cmake .. -G "MinGW Makefiles"` y compilar con `mingw32-make` en lugar de `make`.

## 3. Convención de carpetas del curso

Cada ejercicio/tema sigue la misma estructura (ya usada en todo `eda_cpp`):

```
topic/
├── CMakeLists.txt
├── include/topic/topic.hpp   # declaraciones (.hpp)
├── src/topic.cpp             # implementación
└── tests/main.cpp            # programa que la ejercita (main)
```

## 4. Compilar y ejecutar

Desde la carpeta del ejercicio (la que contiene `CMakeLists.txt`):

```bash
mkdir build
cd build
cmake ..
make
./test
```

Si aparece el ejecutable corriendo sin errores de compilación, el entorno está listo.

## 5. VSCode

Extensiones recomendadas: `C/C++` (ms-vscode.cpptools) y `CMake Tools` (ms-vscode.cmake-tools). Con `CMake Tools` se puede configurar/compilar/ejecutar directamente desde la barra de estado, sin usar la terminal manualmente.
