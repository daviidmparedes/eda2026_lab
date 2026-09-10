# Laboratorio 3 — Ordenación de códigos en tiempo lineal

Este laboratorio es la parte práctica de la **Prueba 1 (Ordenación de Códigos)**: implementar la ordenación en tiempo `O(n)` que pedía la prueba y compararla contra dos de los algoritmos vistos en clase.

## Objetivo

Dada una lista de `n` códigos, donde cada código es una secuencia de **5 dígitos (0-9)**:

| Parte de la prueba | Qué se pide | Qué hay que programar |
|---|---|---|
| **2a** | Un algoritmo `O(n)` para ordenar `n` elementos que son dígitos (0-9) | `counting_sort` |
| **2b** | Usar el anterior para ordenar la lista de códigos en `O(n)` | `counting_sort_by_digit` y `radix_sort` |
| **3** | Comparar contra dos algoritmos vistos en clase, con una tabla de tiempos para diversos valores de `n` | nada: `quick_sort` y `merge_sort` ya vienen resueltos |

Las partes 1 y 2c son de análisis y van en el informe, no en el código. Eso sí, los tiempos que midas acá son la evidencia con la que las respaldas.

## Estructura

```
lab3/
├── data/
│   ├── codes_1M.bin        # el dataset: 1.000.000 de códigos (3,8 MB)
│   └── generate_codes.py   # cómo se generó (no hace falta correrlo)
└── starter/poscode/        # aquí trabajas
```

Y por dentro:

```
poscode/
├── CMakeLists.txt
├── include/poscode/
│   ├── poscode.hpp         # la clase Poscode          (dado)
│   ├── sort.hpp            # los algoritmos
│   └── utils.hpp           # lectura del dataset       (dado)
├── src/
│   ├── poscode.cpp         # (dado)
│   ├── sort.cpp            #  <- AQUÍ TRABAJAS
│   └── utils.cpp           # (dado)
├── tests/main.cpp          # pruebas y medición de tiempos (dado)
└── python/plot_times.py    # gráficos
```

## El dataset

**No tienes que leer archivos ni parsear nada.** Eso ya está resuelto en `utils.cpp`. Tu trabajo empieza cuando ya tienes el arreglo `Poscode*` en memoria.

El archivo `data/codes_1M.bin` trae **un millón de códigos** en binario, no en texto, por dos razones:

- **ocupa menos**: 4 bytes por código en vez de 6 (5 dígitos + el salto de línea), o sea 3,8 MB en vez de 5,7 MB;
- **se lee de una pasada, sin parsear texto**. A `n = 1.000.000`, parsear un millón de líneas tarda más que varios de los algoritmos que queremos medir, y eso contaminaría la tabla de tiempos.

Formato del archivo (todo entero es `uint32` little-endian):

| offset | campo | valor |
|---|---|---|
| 0 | magic | `"EDAP"` |
| 4 | version | `1` |
| 8 | count | `1000000` |
| 12 | digits | `5` |
| 16 | payload | `count` × `uint32`, cada valor en `[0, 100000)` |

Cada código se guarda como su valor numérico (`"05893"` → `5893`) y al leerlo se rellena con ceros a la izquierda hasta 5 caracteres, así que la conversión no pierde información.

Los barridos de tamaño leen **los primeros `n`** códigos del archivo. Como el archivo se generó al azar, cualquier prefijo sirve como muestra aleatoria.

El dataset ya viene en el repositorio. `generate_codes.py` está solo para documentar cómo se produjo y por si hay que regenerarlo (usa semilla fija, así que sale idéntico).

## La clase `Poscode`

Es la misma del repositorio del curso ([`eda_cpp/poscodes`](https://github.com/jmsaavedrar/eda_cpp)). Ya viene implementada, y solo necesitas dos cosas de ella:

```cpp
char getValue(size_t i);          // el carácter en la posición i (i = 0 es el más significativo)
const std::string &getData();     // el código completo, para comparar
```

`getValue` devuelve un **carácter**, no un número. Para pasar de `'7'` al entero `7`:

```cpp
int digito = codigo.getValue(i) - '0';
```

Y como todos los códigos tienen el mismo largo, el orden alfabético de los strings coincide con el numérico: `"05893" < "13802" < "83471"`.

## Compilar y ejecutar

Desde `lab3/starter/poscode`:

```bash
mkdir build
cd build
cmake ..
make
./test
```

`./test` encuentra el dataset solo: `CMakeLists.txt` le pasa la ruta absoluta al compilador.

Para los gráficos:

```bash
cd ../python
python3 plot_times.py
```

El `.csv` y los `.png` quedan los dos en `python/`, junto al script.

> El `CMakeLists.txt` compila con `-O2`. **No lo saques**: sin optimizar, los tiempos medidos no representan el costo real de los algoritmos.

## Qué hay que implementar

Todo está en **`src/sort.cpp`**, y son tres funciones en orden, donde cada una usa la anterior. `quick_sort` y `merge_sort` **ya vienen resueltos** (son los del laboratorio 2 adaptados a `Poscode`): son la base de comparación, no hay que tocarlos.

Mientras una función esté sin implementar, `./test` la marca como `FALLA` en la tabla pero el resto sigue corriendo igual, así que puedes avanzar de a una.

### Paso 1 — `counting_sort` (Parte 2a)

```cpp
void counting_sort(int *A, size_t n);
```

Ordena un arreglo de `n` enteros que solo toma valores en `[0, 9]`, en `O(n)`.

La clave está en el enunciado: hay solo **10 valores posibles**. Piensa qué información te basta reunir sobre el arreglo para poder escribirlo ordenado sin comparar sus elementos entre sí.

Tu implementación no puede tener ciclos anidados sobre `n`.

### Paso 2 — `counting_sort_by_digit`

```cpp
void counting_sort_by_digit(Poscode *A, size_t n, size_t d);
```

Lo mismo, pero sobre códigos, y mirando **un solo dígito**: el de la posición `d` (`d = 0` es el más significativo).

**Tiene que ser estable.** Dos códigos con el mismo dígito `d` deben quedar en el mismo orden relativo que traían. Sin eso, el Paso 3 no funciona.

Es el error más común del laboratorio y es **silencioso**: el arreglo igual queda ordenado por ese dígito, así que a simple vista parece correcto. Por eso `./test` trae una prueba específica, y cuando esté bien vas a ver:

```
Parte 2b - counting_sort_by_digit debe ser estable
  entrada  : 10001 20002 10003 30004 20005 10006 30007 20008
  por d=0  : 10001 10003 10006 20002 20005 20008 30004 30007
  resultado: [ok] es estable
```

### Paso 3 — `radix_sort` (Parte 2b)

```cpp
void radix_sort(Poscode *A, size_t n);
```

Ordena los `n` códigos en `O(n)`, usando `counting_sort_by_digit`.

Un código tiene varios dígitos y `counting_sort_by_digit` solo sabe mirar uno. Tienes que decidir **en qué orden recorrer los dígitos** para que al final el arreglo quede ordenado por el código completo: el orden correcto es el punto del ejercicio, y el incorrecto también deja el arreglo "casi" ordenado.

El largo de un código se obtiene así:

```cpp
size_t p = A[0].getData().size();   // 5 en este dataset
```

## Cómo se miden los tiempos

Vale la pena entender el arnés antes de confiar en los números que produce:

- Cada algoritmo se mide **sobre los mismos datos**: se ordena siempre una copia, nunca el arreglo original.
- Se reporta el **menor** tiempo de varias repeticiones, no el promedio: el ruido externo (otros procesos, el planificador, el asignador de memoria) solo puede sumar tiempo, nunca restarlo.
- La cantidad de repeticiones no es fija: se repite hasta acumular unos 400 ms. Así los casos rápidos, que a `n` chico son pura fluctuación, se repiten muchas veces, y los lentos se repiten el mínimo.
- El tiempo de **lectura del archivo queda fuera** de la medición: el dataset se carga una vez por cada `n`, antes de cronometrar.
- Antes del barrido, `./test` verifica que los tres algoritmos produzcan **exactamente el mismo arreglo**. `isSorted` por sí solo no basta: un `radix_sort` con un contador mal calculado puede dejar algo ordenado pero con códigos repetidos o perdidos.

## Qué hay que entregar

Los valores de `n` del barrido son los que pide la prueba: 1.000, 10.000, 50.000, 100.000, 500.000 y 1.000.000.

1. **La tabla de tiempos** que imprime `./test` (también queda en `python/times_codes.csv`), con los tres algoritmos.
2. **Los dos gráficos** que genera `plot_times.py`.

## Entrega

No modifiques `tests/main.cpp` ni `src/utils.cpp`.

Antes de entregar, `./test` tiene que mostrar:

- `Parte 2a ... [ok]`
- `Parte 2b ... [ok] es estable`
- los tres algoritmos en `[ok]` en la verificación de resultados iguales,
- y **ningún `FALLA`** en la tabla de tiempos.
