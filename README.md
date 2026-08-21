# EDA 2026 — Laboratorios

Laboratorios del curso **Estructuras de Datos y Algoritmos**. Cada laboratorio trae un esqueleto de código con las partes por completar marcadas como `TODO`.

## Antes de empezar

Lee [`SETUP.md`](SETUP.md): instalación de `g++`, CMake y VSCode en Linux, macOS y Windows (incluida la máquina virtual del curso).

## Laboratorios

| | Tema | Contenido del libro | Carpeta |
|---|---|---|---|
| **Lab 1** | Potencia y reordenamiento lineal | Cap. 2.6 (divide y vencerás) | [`lab1/starter/`](lab1/starter) |
| **Lab 2** | Quicksort y comparación de algoritmos de ordenamiento | Cap. 2.7 (Alg. 9 a 14) | [`lab2/`](lab2) — ver [`lab2/README.md`](lab2/README.md) |

- **Lab 1** tiene dos ejercicios independientes: `power/` (tres versiones de la potencia: iterativa, recursiva simple y divide y vencerás) y `reorder/` (reordenar un arreglo en `O(n)` dejando los negativos al inicio).
- **Lab 2** tiene un solo módulo, `sort/`, con su propio enunciado, tablas de referencia y gráficos en [`lab2/README.md`](lab2/README.md).

## Compilar y ejecutar

Igual en todos los ejercicios. Desde la carpeta que contiene el `CMakeLists.txt`:

```bash
mkdir build
cd build
cmake ..
make
./test
```

La carpeta `build/` se regenera y no se versiona: si algo se rompe, `rm -rf build` y de nuevo.

## Convención de carpetas

Todos los ejercicios siguen la misma estructura, la misma del repositorio [`eda_cpp`](https://github.com/jmsaavedrar/eda_cpp) del curso:

```
tema/
├── CMakeLists.txt
├── include/tema/tema.hpp   # declaraciones
├── src/tema.cpp            # implementación  <- aquí trabajas
└── tests/main.cpp          # programa que la ejercita (main)
```

Algunos ejercicios agregan una carpeta `python/` con scripts de graficación (requieren `matplotlib`: `pip3 install matplotlib`).

## Entrega

Trabaja siempre sobre `starter/`. No modifiques `tests/main.cpp` salvo donde el enunciado lo pida explícitamente.
