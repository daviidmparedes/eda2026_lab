"""Grafica los tiempos de ordenacion de codigos del laboratorio 3.

Uso:
    python3 plot_times.py [ruta_csv]

Sin argumentos lee times_codes.csv de ESTA misma carpeta (el que deja ./test) y
guarda los png tambien aca, para que los datos y los graficos queden juntos.

Genera DOS figuras, porque cada una responde una pregunta distinta:

  times_codes.png        tiempo vs n en escala log-log. Sirve para comparar
                         magnitudes: se ve de una que radixsort corre por
                         debajo de los otros dos en todo el rango.

  times_per_element.png  tiempo POR ELEMENTO (ns) vs n. Es el grafico que
                         responde la parte 2c de la prueba. Si un algoritmo es
                         O(n), su costo por elemento es constante y la curva
                         sale PLANA; si es O(n log n), el costo por elemento
                         crece como log n y la curva SUBE. En el log-log de la
                         primera figura esa diferencia casi no se nota, porque
                         un factor log n es una curvatura muy suave.
"""
import csv
import os
import sys

import matplotlib.pyplot as plt

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Etiqueta, color, marcador y estilo de linea por columna del csv.
# Cada serie se distingue por color Y por forma: asi el grafico se sigue
# leyendo impreso en blanco y negro o por alguien con daltonismo.
STYLES = {
    "quicksort": ("Quicksort  -  O(n log n)", "#2a78d6", "o", "-"),
    "mergesort": ("Mergesort  -  O(n log n)", "#eb6834", "s", "--"),
    "radixsort": ("Radixsort  -  O(n p)",     "#1baf7a", "D", "-."),
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
                if value:
                    series[c].append((n, float(value)))
    return columns, series


def style_axes(ax, xlabel, ylabel, title):
    ax.set_xlabel(xlabel, color=INK_SOFT)
    ax.set_ylabel(ylabel, color=INK_SOFT)
    ax.set_title(title, color=INK)
    ax.tick_params(colors=INK_SOFT)
    ax.grid(True, which="both", alpha=0.25, linewidth=0.6)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_total(columns, series, out_path):
    fig, ax = plt.subplots(figsize=(8, 5))
    for col in columns:
        points = series[col]
        if not points:
            continue
        label, color, marker, linestyle = STYLES.get(col, (col, None, "o", "-"))
        ax.plot([p[0] for p in points], [p[1] for p in points], label=label,
                color=color, marker=marker, linestyle=linestyle,
                linewidth=2, markersize=7)
    ax.set_xscale("log")
    ax.set_yscale("log")
    style_axes(ax, "n (cantidad de codigos)", "tiempo (ms, escala log)",
               "Ordenacion de codigos de 5 digitos")
    # Arriba a la izquierda queda vacio: con n chico nadie tarda mucho.
    ax.legend(frameon=False, labelcolor=INK_SOFT, loc="upper left", fontsize=9)
    fig.tight_layout()
    fig.savefig(out_path, dpi=150)
    print("Grafico guardado en " + out_path)


def plot_per_element(columns, series, out_path):
    fig, ax = plt.subplots(figsize=(8, 5))
    for col in columns:
        points = series[col]
        if not points:
            continue
        label, color, marker, linestyle = STYLES.get(col, (col, None, "o", "-"))
        # ms totales -> nanosegundos por elemento.
        ys = [(ms * 1e6) / n for (n, ms) in points]
        ax.plot([p[0] for p in points], ys, label=label,
                color=color, marker=marker, linestyle=linestyle,
                linewidth=2, markersize=7)
    ax.set_xscale("log")
    ax.set_ylim(bottom=0)
    style_axes(ax, "n (cantidad de codigos)", "tiempo por elemento (ns)",
               "Costo por elemento: plano = O(n), creciente = O(n log n)")
    ax.legend(frameon=False, labelcolor=INK_SOFT, loc="upper left", fontsize=9)
    fig.tight_layout()
    fig.savefig(out_path, dpi=150)
    print("Grafico guardado en " + out_path)


def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(SCRIPT_DIR, "times_codes.csv")
    # Si se pasa solo el nombre del csv, se busca en esta carpeta.
    if not os.path.isabs(csv_path) and not os.path.exists(csv_path):
        csv_path = os.path.join(SCRIPT_DIR, os.path.basename(csv_path))

    if not os.path.exists(csv_path):
        print("No encuentro " + csv_path)
        print("Compila y corre ./test primero: el ejecutable deja el csv en esta carpeta.")
        return 1

    columns, series = read_csv(csv_path)
    plot_total(columns, series, os.path.join(SCRIPT_DIR, "times_codes.png"))
    plot_per_element(columns, series, os.path.join(SCRIPT_DIR, "times_per_element.png"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
