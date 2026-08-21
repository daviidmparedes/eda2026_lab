# EDA 2026 — Laboratorios

Laboratorios del curso **Estructuras de Datos y Algoritmos**. Cada ejercicio trae un esqueleto (`starter/`) con las partes por completar marcadas como `TODO`, y una solución de referencia (`solution/`).

Los algoritmos siguen el libro del curso y las convenciones del repositorio [`eda_cpp`](https://github.com/jmsaavedrar/eda_cpp).

---

## 1. Herramientas requeridas

- **g++** (estándar C++11) — compilador.
- **CMake** (≥ 3.10) — genera los archivos de compilación de forma independiente del sistema operativo.
- **Visual Studio Code** — editor recomendado, con las extensiones `C/C++` y `CMake Tools`.
- **Python 3 + matplotlib** — solo para los gráficos (`pip3 install matplotlib`).

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

### VSCode

Extensiones recomendadas: `C/C++` (`ms-vscode.cpptools`) y `CMake Tools` (`ms-vscode.cmake-tools`). Con `CMake Tools` se puede configurar/compilar/ejecutar desde la barra de estado, sin usar la terminal manualmente.

---

## 3. Convención de carpetas

Cada ejercicio sigue la misma estructura, la misma que usa `eda_cpp`:

```
tema/
├── CMakeLists.txt
├── include/tema/tema.hpp   # declaraciones (.hpp)
├── src/tema.cpp            # implementación   <- aquí trabajas
├── tests/main.cpp          # programa que la ejercita (main)
└── python/                 # opcional: script de graficación
```

Y cada laboratorio tiene las dos variantes del mismo ejercicio:

```
labX/
├── starter/tema/     # esqueleto con TODOs — aquí trabajas
└── solution/tema/    # solución de referencia
```

## 4. Compilar y ejecutar

Desde la carpeta del ejercicio (la que contiene el `CMakeLists.txt`):

```bash
mkdir build
cd build
cmake ..
make
./test
```

Si el ejecutable corre sin errores de compilación, el entorno está listo. La carpeta `build/` se regenera y no se versiona: si algo se rompe, `rm -rf build` y de nuevo.

## 5. Gráficos

Los ejercicios que miden algo escriben un `.csv` **dentro de su carpeta `python/`**, no dentro de `build/`. El script de graficación vive ahí mismo y, sin argumentos, lee ese `.csv` y deja el `.png` al lado:

```bash
cd build && ./test          # escribe ../python/*.csv
cd ../python
python3 plot_times.py       # lee times_random.csv y deja times_random.png aquí
```

Así los datos y los gráficos quedan juntos y ninguna otra carpeta se ensucia. Todo lo que hay en `python/*.csv` y `python/*.png` es regenerable y está en el `.gitignore`.

---

## 6. Laboratorios

| | Tema | Contenido del libro | Ejercicios |
|---|---|---|---|
| **Lab 1** | Recursividad y tiempo de ejecución | Cap. 2.3 y 2.6 (Alg. 7 y 8) | `power/`, `reorder/` |
| **Lab 2** | Algoritmos de ordenamiento | Cap. 2.7 (Alg. 9 a 14) | `sort/` |

### Lab 1 — [`lab1/`](lab1)

- **`power/`** — tres versiones de `base^exp`, contando los pasos de cada una: `pow_it` (iterativa, Alg. 7), `pow_rec_simple` (recursiva lineal) y `pow_rec_dc` (recursiva divide y vencerás, Alg. 8 / Eq. 2.2). Genera `steps.csv` y un gráfico de pasos vs exponente.
- **`reorder/`** — reordenar un arreglo in-place en `O(n)` dejando los negativos al inicio y los no-negativos al final, con la técnica de dos punteros del Cap. 2.3.1.

### Lab 2 — [`lab2/`](lab2)

- **`sort/`** — Quicksort con dos elecciones de pivote (al azar, como el Alg. 13, y al centro), Mergesort (Alg. 11 y 12) y tres algoritmos básicos de los cuales se implementa **uno**: selección (Alg. 9), inserción (Alg. 10) o burbuja. Quicksort viene ya resuelto porque se programó en clases. Mide tiempos reales, escribe dos `.csv` y genera dos gráficos log-log.

## 7. Entrega

Trabaja siempre sobre `starter/`. No modifiques `tests/main.cpp` salvo donde el enunciado lo pida explícitamente (en el lab 2, la constante `ALGORITMO_BASICO`).
