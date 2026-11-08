import os
import sys
import time
import platform
import re

# ==============================================================================
# 1. GENERACIÓN DEL TRIÁNGULO DE PASCAL (MEMORIA DINÁMICA)
# ==============================================================================
def generar_triangulo_pascal(n: int) -> list[list[int]]:
    """
    Construye dinámicamente el Triángulo de Pascal hasta la fila n (inclusive).
    Cada fila se asigna dinámicamente en tiempo de ejecución.
    """
    if n < 0:
        raise ValueError("n debe ser un entero no negativo.")
    
    triangulo = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j]
        triangulo.append(fila)
    return triangulo


# ==============================================================================
# 2. GENERACIÓN DEL POLINOMIO FORMATEADO (x + 1)^n
# ==============================================================================
def formatear_polinomio(coeffs: list[int]) -> str:
    """
    Formatea los coeficientes del binomio en una cadena algebraica elegante.
    Ejemplo para n=3: x^3 + 3x^2 + 3x + 1
    """
    n = len(coeffs) - 1
    if n == 0:
        return "1"
    
    terminos = []
    for k, c in enumerate(coeffs):
        exp = n - k
        if exp == 0:
            terminos.append(str(c))
        elif exp == 1:
            terminos.append("x" if c == 1 else f"{c}x")
        else:
            terminos.append(f"x^{exp}" if c == 1 else f"{c}x^{exp}")
    return " + ".join(terminos)


# ==============================================================================
# 3. EVALUACIÓN PASO A PASO DE f(x) = (x + 1)^n
# ==============================================================================
def evaluar_paso_a_paso(n: int, x: float):
    """
    Muestra de forma detallada y pedagógica el cálculo evaluado para f(x) = (x + 1)^n.
    """
    triangulo = generar_triangulo_pascal(n)
    coeffs = triangulo[n]
    poli_str = formatear_polinomio(coeffs)

    print("=" * 65)
    print(f"EVALUACIÓN PASO A PASO: f(x) = (x + 1)^{n} con x = {x}")
    print("=" * 65)
    print(f"1. Polinomio expandido mediante Pascal:")
    print(f"   f(x) = {poli_str}")
    print()

    # Paso 1: Sustitución de x
    terminos_sust = []
    for k, c in enumerate(coeffs):
        exp = n - k
        if exp == 0:
            terminos_sust.append(str(c))
        elif exp == 1:
            terminos_sust.append(f"({x})" if c == 1 else f"{c}*({x})")
        else:
            terminos_sust.append(f"({x})^{exp}" if c == 1 else f"{c}*({x})^{exp}")
    
    print(f"2. Sustitución de la variable x = {x}:")
    print(f"   f({x}) = {' + '.join(terminos_sust)}")
    print()

    # Paso 2: Cálculo de potencias y productos individuales
    valores_terminos = []
    print(f"3. Cálculo término a término:")
    for k, c in enumerate(coeffs):
        exp = n - k
        pot = x ** exp
        val = c * pot
        valores_terminos.append(val)
        
        # Formato de presentación para enteros o decimales
        val_str = f"{val:g}" if isinstance(val, float) and val.is_integer() else f"{val}"
        pot_str = f"{pot:g}" if isinstance(pot, float) and pot.is_integer() else f"{pot}"

        if exp == 0:
            print(f"   - Término k={k} (constante) : {c}")
        elif exp == 1:
            print(f"   - Término k={k} (grado 1)  : {c} * ({x}) = {val_str}")
        else:
            print(f"   - Término k={k} (grado {exp})  : {c} * ({x})^{exp} = {c} * {pot_str} = {val_str}")
    print()

    # Paso 3: Suma total de los términos
    total_evaluado = sum(valores_terminos)
    total_str = f"{total_evaluado:g}" if isinstance(total_evaluado, float) and total_evaluado.is_integer() else f"{total_evaluado}"
    suma_str = " + ".join(f"{v:g}" if isinstance(v, float) and v.is_integer() else f"{v}" for v in valores_terminos)
    print(f"4. Suma de los términos calculados:")
    print(f"   f({x}) = {suma_str} = {total_str}")
    print()

    # Paso 4: Comprobación directa mediante la fórmula binómica
    valor_directo = (x + 1) ** n
    directo_str = f"{valor_directo:g}" if isinstance(valor_directo, float) and valor_directo.is_integer() else f"{valor_directo}"
    print(f"5. Comprobación directa f({x}) = ({x} + 1)^{n}:")
    print(f"   f({x}) = ({x + 1})^{n} = {directo_str}")
    
    coincide = abs(total_evaluado - valor_directo) < 1e-9
    print(f"   -> Verificación: {'CORRECTO (Coincidencia exacta)' if coincide else 'ERROR'}")
    print("=" * 65)


# ==============================================================================
# 4. BENCHMARK: GENERACIÓN HASTA n = 100 Y GUARDADO EN BENCHMARK.TXT
# ==============================================================================
def ejecutar_benchmark_y_guardar(iteraciones=1000):
    print("\n" + "=" * 65)
    print("BENCHMARK: Generación del Triángulo de Pascal hasta n = 100")
    print("=" * 65)

    # Verificación de fila 100
    tri_100 = generar_triangulo_pascal(100)
    print(f"Fila n=100 generada exitosamente. Total coeficientes: {len(tri_100[100])}")
    print(f"Coeficiente central C(100, 50): {tri_100[100][50]}")
    print("-" * 65)

    # 1. Medición de 1 corrida única
    t0 = time.perf_counter_ns()
    generar_triangulo_pascal(100)
    t1 = time.perf_counter_ns()
    t_single_ns = t1 - t0
    t_single_ms = t_single_ns / 1_000_000

    # 2. Medición promedio de iteraciones para precisión estadística
    t0 = time.perf_counter_ns()
    for _ in range(iteraciones):
        generar_triangulo_pascal(100)
    t1 = time.perf_counter_ns()
    t_avg_ns = (t1 - t0) / iteraciones
    t_avg_ms = t_avg_ns / 1_000_000

    print(f"Tiempo en Python hasta n=100 (1 corrida)       : {t_single_ms:.4f} ms ({t_single_ns:,} ns)")
    print(f"Tiempo en Python hasta n=100 (promedio {iteraciones} it): {t_avg_ms:.4f} ms ({round(t_avg_ns):,} ns)")

    # Guardar en benchmark.txt (dentro de la carpeta del Ejercicio 2)
    ruta_dir = os.path.dirname(os.path.abspath(__file__))
    ruta_benchmark = os.path.join(ruta_dir, "benchmark.txt")

    encabezado = f"""================================================================================
REPORTE DE BENCHMARK - EJERCICIO 2: POLINOMIOS CON TRIÁNGULO DE PASCAL
Lenguajes y Compiladores - Prueba Diagnóstica
Estudiante: Franner Bermúdez
================================================================================
- Grado de Prueba    : n = 100 (Triángulo de 101 filas, 5,151 coeficientes evaluados)
================================================================================

1. MEDICIÓN DE RENDIMIENTO EN PYTHON (pascal.py):
--------------------------------------------------------------------------------
- Asignación de memoria              : Listas dinámicas en tiempo de ejecución
- Tiempo de ejecución (1 corrida)    : {t_single_ms:.4f} ms ({t_single_ns:,} ns)
- Tiempo de ejecución (promedio {iteraciones} it): {t_avg_ms:.4f} ms ({round(t_avg_ns):,} ns)
================================================================================
"""

    bloque_c = ""
    t_c_avg = None
    if os.path.exists(ruta_benchmark):
        try:
            with open(ruta_benchmark, "r", encoding="utf-8") as f:
                prev = f.read()
                if "2. MEDICIÓN DE RENDIMIENTO EN C" in prev:
                    contenido_c = "2. MEDICIÓN DE RENDIMIENTO EN C" + prev.split("2. MEDICIÓN DE RENDIMIENTO EN C")[1]
                    if "3. TABLA COMPARATIVA" in contenido_c:
                        contenido_c = contenido_c.split("3. TABLA COMPARATIVA")[0].strip()
                    bloque_c = contenido_c
                    m_c = re.search(r"Tiempo de ejecución \(promedio \d+ it\):\s*([\d\.]+)\s*ms", contenido_c)
                    if m_c:
                        t_c_avg = float(m_c.group(1))
        except Exception:
            pass

    texto_final = encabezado
    if bloque_c:
        bloque_c_limpio = bloque_c.strip()
        while bloque_c_limpio.endswith("=" * 80):
            bloque_c_limpio = bloque_c_limpio[:-80].strip()
        bloque_c_limpio += "\n" + ("=" * 80)
        texto_final += "\n" + bloque_c_limpio
        if t_c_avg:
            sp = t_avg_ms / t_c_avg if t_c_avg > 0 else 0
            texto_final += f"""\n\n3. TABLA COMPARATIVA DE RENDIMIENTO (PYTHON VS C):
--------------------------------------------------------------------------------
Lenguaje        | Estructura de Memoria       | Tiempo n=100 (Promedio) | Factor
----------------+-----------------------------+-------------------------+-------
Python 3.13     | Listas dinámicas            | {t_avg_ms:>13.4f} ms      | 1.00x (Base)
C (GCC -O2)     | Punteros u128** (malloc)    | {t_c_avg:>13.4f} ms      | C es {sp:.2f}x más rápido

================================================================================
"""

    with open(ruta_benchmark, "w", encoding="utf-8") as f:
        f.write(texto_final)

    print(f"\n[OK] Resultados guardados en: {ruta_benchmark}")
    print("=" * 65)


# ==============================================================================
# 5. DEMOSTRACIÓN GENERAL
# ==============================================================================
if __name__ == "__main__":
    print("=" * 65)
    print("EJERCICIO 2: GENERADOR DE POLINOMIOS (x + 1)^n CON PASCAL")
    print("Lenguajes y Compiladores - Prueba Diagnóstica")
    print("Estudiante: Franner Bermúdez")
    print("=" * 65)

    # a) Demostración del Triángulo de Pascal (n=0 a 5)
    print("\n[A] GENERACIÓN DE POLINOMIOS (x + 1)^n DESDE EL TRIÁNGULO DE PASCAL:")
    print("-" * 65)
    tri_demo = generar_triangulo_pascal(5)
    for n_val in range(6):
        coeffs = tri_demo[n_val]
        poli = formatear_polinomio(coeffs)
        print(f"n = {n_val} | Coeficientes: {coeffs}")
        print(f"       -> (x + 1)^{n_val} = {poli}")

    # b) Demostración de evaluación paso a paso
    print("\n[B] DEMOSTRACIÓN DEL CÁLCULO EVALUADO PASO A PASO:")
    evaluar_paso_a_paso(n=4, x=2)
    print()
    evaluar_paso_a_paso(n=3, x=5)

    # c) Benchmark hasta n = 100
    ejecutar_benchmark_y_guardar(iteraciones=1000)
