import os
import sys
import time

if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")

# ==============================================================================
# 1. FUNCIÓN DE VALIDACIÓN DEL CABALLO
# ==============================================================================
def validar_caballo(pos_inicial, cadena_movimientos):
    col_map = {c: i for i, c in enumerate('abcdefgh')}

    # Acepta notación con o sin 'K' (ej. 'a1', 'Ka1')
    pos = pos_inicial.lstrip('Kk').lower()
    if len(pos) != 2 or pos[0] not in col_map or pos[1] not in '12345678':
        return False, f"Error: Posición inicial inválida '{pos_inicial}'."
    c_col, c_row = col_map[pos[0]], int(pos[1])

    movimientos = cadena_movimientos.strip().split()
    if not movimientos:
        return False, "Error: La cadena de movimientos está vacía."

    for idx, mov in enumerate(movimientos, 1):
        # El movimiento debe comenzar con 'K'.
        if not mov.startswith('K'):
            return False, f"movimiento INCORRECTO falla letra {mov[0]} no pertenece a un caballo."

        # Debe tener columna a-h y fila 1-8.
        if len(mov) != 3 or mov[1].lower() not in col_map or mov[2] not in '12345678':
            return False, f"movimiento INCORRECTO falla en el movimiento {idx}"

        n_col, n_row = col_map[mov[1].lower()], int(mov[2])
        dx, dy = abs(n_col - c_col), abs(n_row - c_row)

        # El caballo se mueve en L: (1,2) o (2,1).
        if (dx, dy) not in [(1, 2), (2, 1)]:
            return False, f"movimiento INCORRECTO falla en el movimiento {idx}"

        c_col, c_row = n_col, n_row

    return True, "movimiento CORRECTO."


# ==============================================================================
# 2. SECUENCIA DE 500 MOVIMIENTOS
# ==============================================================================
SECUENCIA_500 = (
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
    "Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 "
    "Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 "
    "Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 "
    "Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 "
    "Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
    "Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
    "Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 "
    "Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 "
    "Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 "
    "Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
    "Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 "
    "Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 "
    "Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
    "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4"
)


# ==============================================================================
# 3. DEMOSTRACIÓN DE LOS CASOS Y MEDICIÓN DE TIEMPO
# ==============================================================================
if __name__ == "__main__":
    print("=" * 65)
    print("EJERCICIO 1: VERIFICADOR DEL CABALLO (caballo.py)")
    print("Lenguajes y Compiladores - Prueba Diagnóstica")
    print("=" * 65)

    casos_oficiales = [
        ("a1", "Kc2 Kd4 Kb3 Kc5"),
        ("a1", "Kc2 Kd4 Kb4 Kc5"),
        ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"),
        ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", "Kb6 Kd7"),
        ("a8", "Kb6 Kb7"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"),
    ]

    print("\n[1] VERIFICACIÓN DE LOS 9 CASOS DEL ENUNCIADO:")
    print("-" * 65)
    for i, (ak, cad) in enumerate(casos_oficiales, 1):
        _, res = validar_caballo(ak, cad)
        print(f"Ejemplo {i}: Ak={ak} | C=\"{cad}\"")
        print(f"   -> Salida: {res}")

    # Medición: verificar 10 veces la secuencia de 500 movimientos.
    t0 = time.perf_counter()
    for _ in range(10):
        _, res_500 = validar_caballo("a2", SECUENCIA_500)
    t_total_ms = (time.perf_counter() - t0) * 1000
    t_promedio_ms = t_total_ms / 10

    print("\n[2] MEDICIÓN DE TIEMPO (PYTHON):")
    print("-" * 65)
    print(f"Estado de la secuencia de 500 : {res_500}")
    print(f"Tiempo total (10 x 500 movs)  : {t_total_ms:.4f} ms")
    print(f"Promedio por secuencia de 500 : {t_promedio_ms:.4f} ms")

    # Reporte de tiempo.
    ruta_dir = os.path.dirname(os.path.abspath(__file__))
    ruta_tiempos = os.path.join(ruta_dir, "tiempos.txt")

    reporte = f"""================================================================================
REPORTE DE TIEMPO DE EJECUCIÓN - EJERCICIO 1: VERIFICADOR DEL CABALLO
Lenguajes y Compiladores - Prueba Diagnóstica
Estudiante: Franner Bermúdez
================================================================================

- Estado de la secuencia de 500 : {res_500}
- Tiempo total de verificar 10 veces la secuencia de 500 (5.000 movimientos):
    * Tiempo total    : {t_total_ms:.4f} ms
    * Promedio por secuencia de 500 : {t_promedio_ms:.4f} ms
================================================================================
"""

    with open(ruta_tiempos, "w", encoding="utf-8") as f:
        f.write(reporte)

    print(f"\n[OK] Resultados guardados en: {ruta_tiempos}")
    print("=" * 65)
