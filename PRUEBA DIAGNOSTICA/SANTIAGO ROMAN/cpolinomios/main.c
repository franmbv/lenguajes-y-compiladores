#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Función para generar los coeficientes usando Memoria Dinámica y Triángulo de Pascal
long long* generar_pascal_dinamico(int n) {
    // Memoria dinámica para un arreglo de punteros (las filas del triángulo)
    long long** triangulo = (long long**)malloc((n + 1) * sizeof(long long*));
    
    for (int i = 0; i <= n; i++) {
        // Reservamos memoria dinámica para cada fila i con i+1 elementos
        triangulo[i] = (long long*)malloc((i + 1) * sizeof(long long));
        triangulo[i][0] = 1;
        triangulo[i][i] = 1;
        
        for (int j = 1; j < i; j++) {
            triangulo[i][j] = triangulo[i - 1][j - 1] + triangulo[i - 1][j];
        }
    }
    
    // Copiamos la última fila (la que nos interesa) a un nuevo bloque dinámico para retornarla
    long long* resultado_fila = (long long*)malloc((n + 1) * sizeof(long long));
    for (int j = 0; j <= n; j++) {
        resultado_fila[j] = triangulo[n][j];
    }
    
    // Liberamos la memoria dinámica de las filas anteriores para evitar fugas de memoria
    for (int i = 0; i <= n; i++) {
        free(triangulo[i]);
    }
    free(triangulo);
    
    return resultado_fila;
}

int main() {
    // Para admitir caracteres especiales en consola en Windows
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    int n;
    double x;
    
    printf("--- EJERCICIO 2: POLINOMIOS (C) ---\n");
    printf("Ingrese un numero entero no negativo n (ej. 4): ");
    scanf("%d", &n);
    printf("Ingrese el valor de x para la evaluacion (ej. 2): ");
    scanf("%lf", &x);
    
    // a) Generar coeficientes con memoria dinámica y medir tiempo
    clock_t inicio = clock();
    long long* coeficientes = generar_pascal_dinamico(n);
    clock_t fin = clock();
    double tiempo_generacion = ((double)(fin - inicio)) / CLOCKS_PER_SEC * 1000.0;
    
    // Preparar salida de coeficientes
    char buffer_coefs[2048];
    int offset = sprintf(buffer_coefs, "Coeficientes generados para (x+1)^%d:\n[ ", n);
    for (int i = 0; i <= n; i++) {
        offset += sprintf(buffer_coefs + offset, "%lld ", coeficientes[i]);
    }
    sprintf(buffer_coefs + offset, "]\n");
    printf("%s", buffer_coefs);
    
    // b) Mostrar por pasos el cálculo de f(x) = (x+1)^n
    char buffer_pasos[4096];
    int offset_p = sprintf(buffer_pasos, "\n--- Evaluando f(x) = (x+1)^%d para x = %.0f ---\n", n, x);
    
    double resultado = 0;
    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        double termino = coeficientes[i] * pow(x, potencia);
        resultado += termino;
        offset_p += sprintf(buffer_pasos + offset_p, "(%lld * %.0f^%d)", coeficientes[i], x, potencia);
        if (i < n) {
            offset_p += sprintf(buffer_pasos + offset_p, " + ");
        }
    }
    offset_p += sprintf(buffer_pasos + offset_p, "\nResultado final f(%.0f) = %.0f\n", x, resultado);
    printf("%s", buffer_pasos);
    
    // Medir tiempo necesario para generar n = 100
    clock_t inicio_100 = clock();
    long long* coef_100 = generar_pascal_dinamico(100);
    clock_t fin_100 = clock();
    double tiempo_100 = ((double)(fin_100 - inicio_100)) / CLOCKS_PER_SEC * 1000.0;
    
    char buffer_tiempo[256];
    sprintf(buffer_tiempo, "\nTiempo necesario para generar n=100 en C: %.6f ms\n", tiempo_100);
    printf("%s", buffer_tiempo);
    
    // Guardar todo en el archivo 'result.txt'
    FILE* archivo = fopen("result.txt", "w");
    if (archivo != NULL) {
        fprintf(archivo, "=== REPORTE DE PRUEBA DIAGNOSTICA - EJERCICIO 2 (C) ===\n");
        fprintf(archivo, "Grado del polinomio (n): %d\n", n);
        fprintf(archivo, "Valor evaluado (x): %.0f\n", x);
        fprintf(archivo, "%s\n", buffer_coefs);
        fprintf(archivo, "%s\n", buffer_pasos);
        fprintf(archivo, "%s", buffer_tiempo);
        fprintf(archivo, "Tiempo de generacion individual: %.6f ms\n", tiempo_generacion);
        fclose(archivo);
        printf("\n[✔] Todo el resultado mostrado en consola fue guardado con éxito en el archivo 'result.txt'.\n");
    } else {
        printf("\n[✖] Error al intentar crear el archivo 'result.txt'.\n");
    }
    
    // Liberar memoria dinámica principal
    free(coeficientes);
    free(coef_100);
    
    return 0;
}
