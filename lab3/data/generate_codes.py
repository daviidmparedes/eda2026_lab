"""Genera el dataset de codigos del laboratorio 3 en formato binario.

Cada codigo es una secuencia de 5 digitos (0-9), tal como en la Prueba 1
("Ordenacion de Codigos"). El archivo se guarda en binario y no en texto por
dos razones:

  * ocupa menos: 4 bytes por codigo en vez de 6 (5 digitos + salto de linea),
    o sea 4 MB en vez de 6 MB para el millon de codigos;
  * se lee de una sola pasada, sin parsear texto. A n = 1000000 parsear un
    millon de lineas tarda mas que varios de los algoritmos que queremos medir,
    y eso contaminaria la tabla de tiempos.

FORMATO DEL ARCHIVO (todo entero es uint32 little-endian)

    offset  0   magic    4 bytes  "EDAP"
    offset  4   version  uint32   = 1
    offset  8   count    uint32   cantidad de codigos guardados
    offset 12   digits   uint32   digitos por codigo (= 5)
    offset 16   payload  count * uint32, cada valor en [0, 10^digits)

Un codigo se guarda como su valor numerico: "05893" -> 5893. Al leerlo se
rellena con ceros a la izquierda hasta 'digits' caracteres, asi que la
conversion no pierde informacion.

El archivo generado ya viene en el repositorio. Este script esta aca para
documentar como se produjo y para poder regenerarlo:

    python3 generate_codes.py
"""
import os
import random
import struct

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

MAGIC = b"EDAP"
VERSION = 1
COUNT = 1000000
DIGITS = 5
# Semilla fija: todos los estudiantes trabajan sobre exactamente los mismos
# datos, y el archivo del repositorio se puede reproducir cuando haga falta.
SEED = 2026

OUT_NAME = "codes_1M.bin"


def main():
    random.seed(SEED)
    max_value = 10 ** DIGITS

    header = MAGIC + struct.pack("<III", VERSION, COUNT, DIGITS)
    # struct.pack por elemento seria lentisimo para un millon de valores:
    # se arma la lista completa y se empaqueta de una sola vez.
    values = [random.randrange(max_value) for _ in range(COUNT)]
    payload = struct.pack("<%dI" % COUNT, *values)

    out_path = os.path.join(SCRIPT_DIR, OUT_NAME)
    with open(out_path, "wb") as f:
        f.write(header)
        f.write(payload)

    size_mb = (len(header) + len(payload)) / (1024.0 * 1024.0)
    print("Escrito %s" % out_path)
    print("  codigos : %d de %d digitos" % (COUNT, DIGITS))
    print("  tamano  : %.2f MB" % size_mb)
    print("  primeros: %s" % ", ".join(str(v).zfill(DIGITS) for v in values[:6]))


if __name__ == "__main__":
    main()
