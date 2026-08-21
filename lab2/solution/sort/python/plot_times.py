"""Grafica n vs tiempo de ejecucion para los cuatro algoritmos de ordenamiento.

Uso:
    python3 plot_times.py [ruta_csv] [ruta_png_salida]

Sin argumentos lee times_random.csv de ESTA misma carpeta (el que deja ./test)
y guarda el png tambien aca. Asi los datos y los graficos quedan juntos y no se
ensucian otras carpetas. Para el otro barrido:
    python3 plot_times.py times_sorted.csv

Los ejes van en escala logaritmica: con n duplicandose en cada fila, la pendiente
de cada curva en log-log es directamente el exponente del costo (pendiente ~2 para
O(n^2), pendiente ~1 para O(n log n)).
"""
import csv
import os
import sys

import matplotlib.pyplot as plt

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Etiqueta, color, marcador y estilo de linea por columna del csv.
# Cada serie se distingue por color Y por forma: asi el grafico sigue siendo
# legible impreso en blanco y negro o para daltonicos.
# El estilo va amarrado al nombre de la columna, no a su posicion, asi que
# cada algoritmo conserva su color aunque elijas medir solo uno de los basicos.
STYLES = {
    "seleccion":    ("Seleccion  -  O(n^2)",                      "#2a78d6", "o", "-"),
    "insercion":    ("Insercion  -  O(n^2)",                      "#eb6834", "s", "--"),
    "burbuja":      ("Burbuja  -  O(n^2)",                        "#1baf7a", "^", "-."),
    "mergesort":    ("Mergesort  -  O(n log n)",                  "#eda100", "D", ":"),
    "quick_azar":   ("Quicksort pivote al azar  -  O(n log n)",   "#e87ba4", "v", "-"),
    "quick_centro": ("Quicksort pivote al centro  -  O(n log n)", "#008300", "P", "--"),
}

TITLES = {
    "times_random": "Tiempo vs n - entrada aleatoria",
    "times_sorted": "Tiempo vs n - entrada ya ordenada",
}

INK = "#0b0b0b"
INK_SOFT = "#52514e"


def read_csv(path):
    """Retorna (columnas, {columna: [(n, ms), ...]}). Ignora celdas vacias."""
    series = {}
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        columns = [c for c in reader.fieldnames if c != "n"]
        for c in columns:
            series[c] = []
        for row in reader:
            n = int(row["n"])
            for c in columns:
                value = row[c].strip()
                if value:  # el cuadratico no se mide para n grande
                    series[c].append((n, float(value)))
    return columns, series


def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(SCRIPT_DIR, "times_random.csv")
    # Si se pasa solo el nombre del csv, se busca en esta carpeta.
    if not os.path.isabs(csv_path) and not os.path.exists(csv_path):
        csv_path = os.path.join(SCRIPT_DIR, os.path.basename(csv_path))
    stem = os.path.splitext(os.path.basename(csv_path))[0]
    out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join(SCRIPT_DIR, stem + ".png")

    if not os.path.exists(csv_path):
        print("No encuentro " + csv_path)
        print("Compila y corre ./test primero: el ejecutable deja los csv en esta carpeta.")
        return 1

    columns, series = read_csv(csv_path)

    fig, ax = plt.subplots(figsize=(8, 5))
    for col in columns:
        points = series[col]
        if not points:
            continue
        label, color, marker, linestyle = STYLES.get(col, (col, None, "o", "-"))
        xs = [p[0] for p in points]
        ys = [p[1] for p in points]
        ax.plot(xs, ys, label=label, color=color, marker=marker,
                linestyle=linestyle, linewidth=2, markersize=6)

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("n (cantidad de elementos)", color=INK_SOFT)
    ax.set_ylabel("tiempo (ms, escala log)", color=INK_SOFT)
    ax.set_title(TITLES.get(stem, stem), color=INK)
    ax.tick_params(colors=INK_SOFT)
    ax.grid(True, which="both", alpha=0.25, linewidth=0.6)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    # Arriba a la izquierda: es la zona que queda vacia en ambos barridos
    # (con n chico ningun algoritmo tarda mucho), asi la leyenda no tapa curvas.
    ax.legend(frameon=False, labelcolor=INK_SOFT, loc="upper left", fontsize=9)

    fig.tight_layout()
    fig.savefig(out_path, dpi=150)
    print("Grafico guardado en " + out_path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
