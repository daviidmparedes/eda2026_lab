"""Grafica n (exponente) vs numero de pasos para las 3 versiones de pow.

Uso:
    python3 plot_steps.py [ruta_csv] [ruta_png_salida]

Por defecto lee build/steps.csv (generado por ./test) y guarda steps.png junto al csv.
"""
import csv
import sys
import os

import matplotlib.pyplot as plt


def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else "steps.csv"
    out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(csv_path) or ".", "steps.png")

    exps, it, rec_simple, rec_dc = [], [], [], []
    with open(csv_path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            exps.append(int(row["exp"]))
            it.append(int(row["it"]))
            rec_simple.append(int(row["rec_simple"]))
            rec_dc.append(int(row["rec_dc"]))

    plt.figure(figsize=(8, 5))
    plt.plot(exps, it, marker="o", label="pow_it (iterativo, O(n))")
    plt.plot(exps, rec_simple, marker="s", label="pow_rec_simple (recursivo, O(n))")
    plt.plot(exps, rec_dc, marker="^", label="pow_rec_dc (divide y venceras, O(log n))")
    plt.xlabel("n (exponente)")
    plt.ylabel("pasos (iteraciones / llamadas recursivas)")
    plt.title("Pasos vs n para las 3 implementaciones de potencia")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(out_path)
    print(f"Grafico guardado en {out_path}")


if __name__ == "__main__":
    main()
