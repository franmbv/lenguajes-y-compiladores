import time

# 1. Traducir columna de letra a número
def transformar_columna(literal_columna):
    columna_mapeada = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
    for i, columna_equivalente in enumerate(columna_mapeada):
        if literal_columna.lower() == columna_equivalente:
            return i + 1
    return None


# 2. Validar el movimiento del caballo
def validar_movimiento(pos_X, pos_Y):
    """Verifica si el salto en L es correcto."""
    pX = pos_X.replace('K', '').lower()
    pY = pos_Y.replace('K', '').lower()
    
    col_X = transformar_columna(pX[0])
    fila_X = int(pX[1])
    
    col_Y = transformar_columna(pY[0])
    fila_Y = int(pY[1])
    
    dif_f = abs(fila_Y - fila_X)
    dif_c = abs(col_Y - col_X)
    
    return (dif_f == 2 and dif_c == 1) or (dif_f == 1 and dif_c == 2)

# Recorrer la lista comparando el elemento anterior con el nuevo
def procesar_movimientos(lista):
    print("\n--- EJECUCIÓN ---")
    
    # Recorremos desde el índice 1 en adelante
    for i in range(1, len(lista)):
        anterior = lista[i - 1]
        actual = lista[i]
        
        # Validar si el movimiento es un salto de caballo válido
        if not validar_movimiento(anterior, actual):
            print(f"MOVIMIENTO INCORRECTO: Falla en el movimiento {i} ({actual}) desde {anterior}.")
            return i, actual  # Retorna el número de movimiento y la notación exacta (ej. 3, "Kb4")
            
        print(f"{anterior.upper()} -> {actual.upper()}")
        
    print("\n¡Todos los movimientos se ejecutaron con éxito!")
    return 0, ""  # Retorna 0 si todo salió bien


if __name__ == "__main__":
    print("--- SIMULADOR DE CABALLO DE AJEDREZ ---")
    print("1. Usar cadena precargada de la guía (500 movimientos)")
    print("2. Ingresar datos manualmente (Posición inicial, N y movimientos)")
    
    opcion = input("Seleccione una opción (1 o 2): ").strip()

    if opcion == "1":
        # Cadena precargada de la guía (500 movimientos) con su posición inicial predeterminada (b4)
        cadena_larga = """Kb4 Ka6 Kc5 Ka4 Kc3 Kc2 Kc1 Ke2 Kg1 Kh3 Kh5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 
        Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 
        Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 
        Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kh5 Ke6 Kc5 Ka4 
        Ka3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 
        Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 
        Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 
        Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kh5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 
        Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 
        Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 
        Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8"""
        
        lista_movimientos = cadena_larga.split()
        
    elif opcion == "2":
        pos_inicial = input("Posición inicial del caballo (ej. a1): ").strip()
        n = int(input("Cantidad de movimientos (N): ").strip())

        lista_movimientos = []
        for i in range(n):
            mov = input(f"Movimiento {i+1} (ej. Kc2): ").strip()
            lista_movimientos.append(mov)
            
        lista_movimientos.insert(0, f"K{pos_inicial}")
    else:
        print("Opción no válida.")
        lista_movimientos = []

    if lista_movimientos:
        # Medir el tiempo como pide la guía
        inicio = time.perf_counter()
        
        num_fallo = 0
        mov_fallido = ""
        if opcion == "1":
            for _ in range(10):
                res_num, res_mov = procesar_movimientos(lista_movimientos)
                if res_num != 0:
                    num_fallo = res_num
                    mov_fallido = res_mov
        else:
            num_fallo, mov_fallido = procesar_movimientos(lista_movimientos)
            
        fin = time.perf_counter()
        tiempo_total = (fin - inicio) * 1000
        
        print(f"\nTiempo de ejecución: {tiempo_total:.4f} ms") 

        # Guardar reporte detallado en el archivo 'result.txt'
        try:
            with open("result.txt", "w", encoding="utf-8") as archivo:
                archivo.write("=== REPORTE DE PRUEBA DIAGNOSTICA (PYTHON) ===\n")
                archivo.write("Ejercicio 1: Movimiento del Caballo de Ajedrez\n")
                if num_fallo == 0:
                    archivo.write("Estado: Movimiento CORRECTO\n")
                else:
                    archivo.write(f"Estado: Movimiento INCORRECTO - Falla en el movimiento {num_fallo} ({mov_fallido})\n")
                archivo.write(f"Tiempo de ejecucion: {tiempo_total:.4f} ms\n")
            print("\n[✔] Reporte detallado guardado con éxito en el archivo 'result.txt'.")
        except Exception as e:
            print(f"\n[✖] Error al intentar guardar el archivo: {e}")
