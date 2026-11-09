import ctypes
import sys
import time
MAX_COEFICIENTE = (1 << (ctypes.sizeof(ctypes.c_ulonglong) * 8)) - 1
MAX_N = min(
    (1 << (ctypes.sizeof(ctypes.c_longlong) * 8 - 1)) - 1,
    ((1 << (ctypes.sizeof(ctypes.c_size_t) * 8)) - 1)
    // ctypes.sizeof(ctypes.c_ulonglong)
    - 1,
)


def generar_coeficientes(coeficientes, n):
    coeficientes[0] = 1
    print("\nTriangulo de Pascal:\n(n = 0): 1")

    for fila in range(1, n + 1):
        coeficientes[fila] = 1
        for columna in range(fila - 1, 0, -1):
            if coeficientes[columna] > MAX_COEFICIENTE - coeficientes[columna - 1]:
                return False
            coeficientes[columna] += coeficientes[columna - 1]

        print(f"(n = {fila}): ", end="")
        print(" ".join(str(coeficientes[columna]) for columna in range(fila + 1)))

    return True


def mostrar_polinomio(coeficientes, n):
    print(f"\nPolinomio generado:\n(x + 1)^{n} = ", end="")
    terminos = []

    for exponente in range(n, -1, -1):
        coeficiente = coeficientes[exponente]
        termino = ""
        if coeficiente != 1 or exponente == 0:
            termino += str(coeficiente)
        if exponente > 0:
            termino += "x"
            if exponente > 1:
                termino += f"^{exponente}"
        terminos.append(termino)

    print(" + ".join(terminos))


def evaluar_polinomio(coeficientes, n, x):
    print(f"\nEvaluacion paso a paso para x = {x:.10g}:")
    suma = 0.0

    for exponente in range(n, -1, -1):
        valor_potencia = x ** exponente
        termino = coeficientes[exponente] * valor_potencia
        suma += termino
        paso = n - exponente + 1
        print(
            f"Paso {paso}: {coeficientes[exponente]} * "
            f"({x:.10g})^{exponente} = {termino:.10g}; "
            f"acumulado = {suma:.10g}"
        )

    print("\nResultado final:")
    print(f"f({x:.10g}) = {suma:.10g}\n")


def main():
    try:
        n = int(input("Ingrese un entero no negativo n: "))
    except (EOFError, ValueError):
        print("Error: n debe ser un entero no negativo.", file=sys.stderr)
        return 1

    if n < 0:
        print("Error: n debe ser un entero no negativo.", file=sys.stderr)
        return 1
    if n > MAX_N:
        print("Error: n es demasiado grande para reservar los coeficientes.", file=sys.stderr)
        return 1

    try:
        x = float(input("Ingrese el valor de x: "))
    except (EOFError, ValueError):
        print("Error: x debe ser un numero real.", file=sys.stderr)
        return 1

    try:
        coeficientes = [0] * (n + 1)
    except (MemoryError, OverflowError):
        print("Error: no se pudo reservar memoria para los coeficientes.", file=sys.stderr)
        return 1

    inicio = time.time_ns()
    if not generar_coeficientes(coeficientes, n):
        print("Error: los coeficientes exceden el rango numerico permitido.", file=sys.stderr)
        return 1

    mostrar_polinomio(coeficientes, n)
    evaluar_polinomio(coeficientes, n, x)
    sys.stdout.flush()
    fin = time.time_ns()
    tiempo_segundos = (fin - inicio) / 1_000_000_000

    print(f"Tiempo de ejecucion: {tiempo_segundos:.9f} segundos")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())