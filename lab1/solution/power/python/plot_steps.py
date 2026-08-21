"""Grafica n (exponente) vs numero de pasos para las 3 versiones de pow.

Uso:
    python3 plot_steps.py [ruta_csv] [ruta_png_salida]

Sin argumentos lee steps.csv de ESTA misma carpeta (el que deja ./test) y
guarda steps.png tambien aca. Asi los datos y los graficos quedan juntos y no
se ensucian otras carpetas.
"""
import csv
import os
import sys

import matplotlib.pyplot as plt

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

INK = "#0b0b0b"
INK_SOFT = "#52514e"

# Etiqueta, color, marcador y estilo de linea por columna del csv.
# Cada serie se distingue por color Y por forma, para que el grafico siga
# siendo legible impreso en blanco y negro o para daltonicos.
STYLES = {
    "it":         ("pow_it (iterativo)  -  O(n)",              "#2a78d6", "o", "-"),
    "rec_simple": ("pow_rec_simple (recursivo)  -  O(n)",      "#eb6834", "s", "--"),
    "rec_dc":     ("pow_rec_dc (divide y venceras)  -  O(log n)", "#1baf7a", "^", "-."),
}


def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(SCRIPT_DIR, "steps.csv")
    out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join(SCRIPT_DIR, "steps.png")

    if not os.path.exists(csv_path):
        print("No encuentro " + csv_path)
        print("Compila y corre ./test primero: el ejecutable deja el csv en esta carpeta.")
        return 1

    exps = []
    series = {"it": [], "rec_simple": [], "rec_dc": []}
    with open(csv_path, newline="") as f:
        for row in csv.DictReader(f):
            exps.append(int(row["exp"]))
            for c in series:
                series[c].append(int(row[c]))

    fig, ax = plt.subplots(figsize=(8, 5))
    for col in ("it", "rec_simple", "rec_dc"):
        label, color, marker, linestyle = STYLES[col]
        ax.plot(exps, series[col], label=label, color=color, marker=marker,
                linestyle=linestyle, linewidth=2, markersize=6)

    ax.set_xlabel("n (exponente)", color=INK_SOFT)
    ax.set_ylabel("pasos (iteraciones / llamadas recursivas)", color=INK_SOFT)
    ax.set_title("Pasos vs n para las 3 implementaciones de potencia", color=INK)
    ax.tick_params(colors=INK_SOFT)
    ax.grid(True, alpha=0.25, linewidth=0.6)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.legend(frameon=False, labelcolor=INK_SOFT, loc="upper left")

    fig.tight_layout()
    fig.savefig(out_path, dpi=150)
    print("Grafico guardado en " + out_path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
