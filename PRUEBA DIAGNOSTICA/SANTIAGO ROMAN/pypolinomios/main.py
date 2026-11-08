import time

def generar_pascal_dinamico(n):
    """Genera los coeficientes de (x+1)^n usando el triángulo de Pascal con listas (memoria dinámica)."""
    triangulo = []
    
    for i in range(n + 1):
        fila = [0] * (i + 1)
        fila[0] = 1  
        fila[i] = 1  
        
        for j in range(1, i):
            fila[j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j]
            
        triangulo.append(fila)
        
    return triangulo[n]

def evaluar_polinomio_pasos(coeficientes, x):
    """Muestra por pasos el cálculo de f(x) = (x+1)^n con el x dado y retorna el texto generado."""
    n = len(coeficientes) - 1
    lineas = []
    
    cabecera = f"\n--- Evaluando f(x) = (x+1)^{n} para x = {x} ---"
    print(cabecera)
    lineas.append(cabecera)
    
    resultado = 0
    pasos = []
    
    for i, c in enumerate(coeficientes):
        potencia = n - i
        termino = c * (x ** potencia)
        resultado += termino
        pasos.append(f"({c} * {x}^{potencia})")
        
    texto_pasos = " + ".join(pasos)
    texto_resultado = f"Resultado final f({x}) = {resultado}"
    
    print(texto_pasos)
    print(texto_resultado)
    
    lineas.append(texto_pasos)
    lineas.append(texto_resultado)
    
    return "\n".join(lineas)

if __name__ == "__main__":
    print("--- EJERCICIO 2: POLINOMIOS (TRIÁNGULO DE PASCAL) ---")
    
    # Definimos valores por defecto o interactivos
    try:
        n = int(input("Ingrese un número entero no negativo n (ej. 4): ").strip())
        x = float(input("Ingrese el valor de x para la evaluación (ej. 2): ").strip())
    except ValueError:
        n = 4
        x = 2.0
        print("Valores por defecto aplicados: n = 4, x = 2")

    # a) Generar coeficientes
    inicio = time.perf_counter()
    coeficientes = generar_pascal_dinamico(n)
    fin = time.perf_counter()
    tiempo_generacion = (fin - inicio) * 1000

    texto_coefs = f"\nCoeficientes generados para (x+1)^{n}:\n{coeficientes}"
    print(texto_coefs)

    # b) Mostrar por pasos el cálculo para x dado
    texto_evaluacion = evaluar_polinomio_pasos(coeficientes, x)

    # Medir tiempo para n = 100 como pide la guía
    inicio_100 = time.perf_counter()
    generar_pascal_dinamico(100)
    fin_100 = time.perf_counter()
    tiempo_100 = (fin_100 - inicio_100) * 1000

    texto_tiempo100 = f"\nTiempo necesario para generar n=100: {tiempo_100:.6f} ms"
    print(texto_tiempo100)

    # --- GUARDAR TODO EN EL ARCHIVO result.txt ---
    try:
        with open("result.txt", "w", encoding="utf-8") as archivo:
            archivo.write("=== REPORTE DE PRUEBA DIAGNOSTICA - EJERCICIO 2 (POLINOMIOS) ===\n")
            archivo.write(f"Grado del polinomio (n): {n}\n")
            archivo.write(f"Valor evaluado (x): {x}\n")
            archivo.write(texto_coefs + "\n")
            archivo.write(texto_evaluacion + "\n")
            archivo.write(texto_tiempo100 + "\n")
            archivo.write(f"Tiempo de generacion individual ({n} elementos): {tiempo_generacion:.6f} ms\n")
            
        print("\n[✔] Todo el resultado mostrado en consola fue guardado con éxito en el archivo 'result.txt'.")
    except Exception as e:
        print(f"\n[✖] Error al intentar guardar el archivo: {e}")
