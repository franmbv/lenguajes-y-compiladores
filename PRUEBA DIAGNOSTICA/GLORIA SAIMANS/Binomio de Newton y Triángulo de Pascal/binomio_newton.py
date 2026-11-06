from __future__ import annotations

import time


def generar_pascal(n: int) -> list[int]:
    """Construye dinámicamente la fila n del triángulo de Pascal."""
    if not isinstance(n, int) or isinstance(n, bool) or n < 0:
        raise ValueError("n debe ser un entero no negativo")

    coeficientes = [1]
    for posicion in range(1, n + 1):
        coeficientes.append(coeficientes[-1] * (n - posicion + 1) // posicion)
    return coeficientes


def imprimir_polinomio(coeficientes: list[int], n: int) -> None:
    terminos: list[str] = []
    for exponente in range(n, -1, -1):
        coeficiente = coeficientes[n - exponente]
        if exponente == 0:
            termino = str(coeficiente)
        elif exponente == 1:
            termino = "x" if coeficiente == 1 else f"{coeficiente}x"
        else:
            termino = f"x^{exponente}" if coeficiente == 1 else f"{coeficiente}x^{exponente}"
        terminos.append(termino)
    print(" + ".join(terminos))


def evaluar_polinomio(coeficientes: list[int], n: int, x: float) -> float:
    resultado = 0.0
    print("\nEvaluación paso a paso:")
    for exponente in range(n, -1, -1):
        coeficiente = coeficientes[n - exponente]
        termino = coeficiente * (x**exponente)
        if exponente == 0:
            print(f"Término constante: {coeficiente} -> acumulado: {resultado + termino:.3f}")
        else:
           
            print(
                f"{coeficiente}({x:.3f})^{exponente} = "
                f"{termino:.3f} -> acumulado: {resultado + termino:.3f}"
            )
        resultado += termino
    print(f"Resultado final: {resultado:.3f}")
    return resultado


def medir_generacion(n: int = 100, repeticiones: int = 100_000) -> float:
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        generar_pascal(n)
    return (time.perf_counter() - inicio) * 1000 / repeticiones


def main() -> None:
    print("Cálculo del polinomio (x + 1)^n")
    try:
        n = int(input("Ingrese n (entero no negativo): "))
        if n < 0:
            raise ValueError
        x = float(input("Ingrese el valor de x: "))
    except ValueError:
        raise SystemExit("Error: introduzca un entero no negativo y un número real válidos.")

    coeficientes = generar_pascal(n)
    print("\nPolinomio generado:")
    imprimir_polinomio(coeficientes, n)
    evaluar_polinomio(coeficientes, n, x)
    
  
    print(
        "\nRendimiento Python (generación de n=100, promedio de 100000 ejecuciones): "
        f"{medir_generacion():.6f} ms \n"
    )


if __name__ == "__main__":
    main()