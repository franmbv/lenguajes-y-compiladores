#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <time.h>

double obtener_tiempo_ms(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return ((double)counter.QuadPart / (double)freq.QuadPart) * 1000.0;
}
#else
double obtener_tiempo_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}
#endif

// Entero sin signo de 128 bits para soportar con exactitud C(100, 50) (~1.0089e29)
typedef unsigned __int128 u128;

// ==============================================================================
// 1. GESTIÓN DE MEMORIA DINÁMICA: TRIÁNGULO DE PASCAL
// ==============================================================================

/**
 * Asigna dinámicamente un array bidimensional irregular de (n + 1) filas.
 * Cada fila i contiene exactamente (i + 1) elementos de tipo u128.
 */
u128** generar_triangulo_pascal(int n) {
    if (n < 0) return NULL;

    // Asignación dinámica del vector de punteros a filas
    u128** tri = (u128**)malloc((size_t)(n + 1) * sizeof(u128*));
    if (!tri) {
        fprintf(stderr, "Error: Fallo al asignar memoria para las filas del triángulo.\n");
        exit(EXIT_FAILURE);
    }

    // Asignación dinámica de cada fila individual
    for (int i = 0; i <= n; i++) {
        tri[i] = (u128*)malloc((size_t)(i + 1) * sizeof(u128));
        if (!tri[i]) {
            fprintf(stderr, "Error: Fallo al asignar memoria para la fila %d.\n", i);
            exit(EXIT_FAILURE);
        }

        tri[i][0] = 1;
        tri[i][i] = 1;
        for (int j = 1; j < i; j++) {
            tri[i][j] = tri[i - 1][j - 1] + tri[i - 1][j];
        }
    }
    return tri;
}

/**
 * Libera de forma completa y recursiva toda la memoria dinámica asignada.
 */
void liberar_triangulo_pascal(u128** tri, int n) {
    if (!tri) return;
    for (int i = 0; i <= n; i++) {
        free(tri[i]);
    }
    free(tri);
}

// ==============================================================================
// 2. UTILIDADES DE IMPRESIÓN PARA ENTEROS DE 128 BITS
// ==============================================================================
void sprint_u128(char* dest, u128 val) {
    if (val == 0) {
        strcpy(dest, "0");
        return;
    }
    char buf[64];
    int idx = 0;
    while (val > 0) {
        buf[idx++] = (char)('0' + (val % 10));
        val /= 10;
    }
    for (int i = 0; i < idx; i++) {
        dest[i] = buf[idx - 1 - i];
    }
    dest[idx] = '\0';
}

void print_u128(u128 val) {
    char buf[64];
    sprint_u128(buf, val);
    printf("%s", buf);
}

// ==============================================================================
// 3. GENERACIÓN DEL POLINOMIO FORMATEADO (x + 1)^n
// ==============================================================================
void imprimir_polinomio(int n, u128* coeffs) {
    if (n == 0) {
        printf("1");
        return;
    }
    char buf[64];
    for (int k = 0; k <= n; k++) {
        int exp = n - k;
        u128 c = coeffs[k];
        sprint_u128(buf, c);

        if (k > 0) printf(" + ");

        if (exp == 0) {
            printf("%s", buf);
        } else if (exp == 1) {
            if (c == 1) printf("x");
            else printf("%sx", buf);
        } else {
            if (c == 1) printf("x^%d", exp);
            else printf("%sx^%d", buf, exp);
        }
    }
}

// ==============================================================================
// 4. EVALUACIÓN PASO A PASO DE f(x) = (x + 1)^n
// ==============================================================================
void evaluar_paso_a_paso(int n, double x) {
    u128** tri = generar_triangulo_pascal(n);
    u128* coeffs = tri[n];

    printf("=================================================================\n");
    printf("EVALUACION PASO A PASO: f(x) = (x + 1)^%d con x = %g\n", n, x);
    printf("=================================================================\n");
    printf("1. Polinomio expandido mediante Pascal:\n   f(x) = ");
    imprimir_polinomio(n, coeffs);
    printf("\n\n");

    printf("2. Sustitucion de la variable x = %g:\n   f(%g) = ", x, x);
    char buf[64];
    for (int k = 0; k <= n; k++) {
        int exp = n - k;
        u128 c = coeffs[k];
        sprint_u128(buf, c);
        if (k > 0) printf(" + ");
        if (exp == 0) {
            printf("%s", buf);
        } else if (exp == 1) {
            if (c == 1) printf("(%g)", x);
            else printf("%s*(%g)", buf, x);
        } else {
            if (c == 1) printf("(%g)^%d", x, exp);
            else printf("%s*(%g)^%d", buf, x, exp);
        }
    }
    printf("\n\n");

    printf("3. Calculo termino a termino:\n");
    double total = 0.0;
    for (int k = 0; k <= n; k++) {
        int exp = n - k;
        u128 c = coeffs[k];
        sprint_u128(buf, c);
        double pot = pow(x, exp);
        double val = (double)c * pot;
        total += val;

        if (exp == 0) {
            printf("   - Termino k=%d (constante) : %s\n", k, buf);
        } else if (exp == 1) {
            printf("   - Termino k=%d (grado 1)  : %s * (%g) = %g\n", k, buf, x, val);
        } else {
            printf("   - Termino k=%d (grado %d)  : %s * (%g)^%d = %s * %g = %g\n", k, exp, buf, x, exp, buf, pot, val);
        }
    }
    printf("\n");

    printf("4. Suma de los terminos calculados:\n   f(%g) = %g\n\n", x, total);

    double directo = pow(x + 1.0, n);
    printf("5. Comprobacion directa f(%g) = (%g + 1)^%d:\n", x, x, n);
    printf("   f(%g) = (%g)^%d = %g\n", x, x + 1.0, n, directo);
    bool ok = fabs(total - directo) < 1e-7;
    printf("   -> Verificacion: %s\n", ok ? "CORRECTO (Coincidencia exacta)" : "ERROR");
    printf("=================================================================\n");

    liberar_triangulo_pascal(tri, n);
}

// ==============================================================================
// 5. BENCHMARK HASTA n = 100 Y ACTUALIZACIÓN DE BENCHMARK.TXT
// ==============================================================================
void ejecutar_benchmark_y_guardar(int iters) {
    printf("\n=================================================================\n");
    printf("BENCHMARK C NATIVO: Generacion de Pascal hasta n = 100\n");
    printf("=================================================================\n");

    // Fila 100
    u128** tri = generar_triangulo_pascal(100);
    printf("Fila n=100 generada exitosamente en memoria dinamica.\n");
    printf("Coeficiente central C(100, 50): ");
    print_u128(tri[100][50]);
    printf("\n-----------------------------------------------------------------\n");
    liberar_triangulo_pascal(tri, 100);

    // Medición 1 corrida
    double t0 = obtener_tiempo_ms();
    u128** t1_ptr = generar_triangulo_pascal(100);
    liberar_triangulo_pascal(t1_ptr, 100);
    double t_single_ms = obtener_tiempo_ms() - t0;

    // Medición promedio
    t0 = obtener_tiempo_ms();
    for (int i = 0; i < iters; i++) {
        u128** tmp = generar_triangulo_pascal(100);
        liberar_triangulo_pascal(tmp, 100);
    }
    double t_total_ms = obtener_tiempo_ms() - t0;
    double t_avg_ms = t_total_ms / (double)iters;

    printf("Tiempo en C hasta n=100 (1 corrida)       : %.4f ms\n", t_single_ms);
    printf("Tiempo en C hasta n=100 (promedio %d it) : %.6f ms\n", iters, t_avg_ms);

    // Actualizar benchmark.txt
    FILE* f_read = fopen("benchmark.txt", "r");
    char buffer_py[4096] = "";
    if (f_read) {
        char line[256];
        while (fgets(line, sizeof(line), f_read)) {
            if (strstr(line, "2. MEDICIÓN DE RENDIMIENTO EN C")) break;
            strcat(buffer_py, line);
        }
        fclose(f_read);
    }

    FILE* f_out = fopen("benchmark.txt", "w");
    if (f_out) {
        if (strlen(buffer_py) > 0) {
            fputs(buffer_py, f_out);
        } else {
            fprintf(f_out, "================================================================================\n");
            fprintf(f_out, "REPORTE DE BENCHMARK - EJERCICIO 2: POLINOMIOS CON TRIÁNGULO DE PASCAL\n");
            fprintf(f_out, "Lenguajes y Compiladores - Prueba Diagnóstica\n");
            fprintf(f_out, "Estudiante: Franner Bermúdez\n");
            fprintf(f_out, "================================================================================\n");
            fprintf(f_out, "Grado de Prueba    : n = 100 (Triángulo de 101 filas, 5,151 coeficientes evaluados)\n");
            fprintf(f_out, "================================================================================\n\n");
        }
        fprintf(f_out, "2. MEDICIÓN DE RENDIMIENTO EN C (pascal.c):\n");
        fprintf(f_out, "--------------------------------------------------------------------------------\n");
        fprintf(f_out, "- Asignación de memoria              : Punteros dobles (u128**) con malloc / free\n");
        fprintf(f_out, "- Tipo de dato numérico              : unsigned __int128 (16 bytes, exactitud hasta 3.4e38)\n");
        fprintf(f_out, "- Tiempo de ejecución (1 corrida)    : %.4f ms\n", t_single_ms);
        fprintf(f_out, "- Tiempo de ejecución (promedio %d it): %.6f ms\n", iters, t_avg_ms);
        fprintf(f_out, "================================================================================\n");
        fclose(f_out);
        printf("\n[OK] Resultados registrados en: benchmark.txt\n");
    }
    printf("=================================================================\n");
}

int main(void) {
    printf("=================================================================\n");
    printf("EJERCICIO 2: GENERADOR DE POLINOMIOS (x + 1)^n CON PASCAL (C - GCC)\n");
    printf("Lenguajes y Compiladores - Prueba Diagnostica\n");
    printf("Estudiante: Franner Bermudez\n");
    printf("=================================================================\n");

    // [A] Demostración de n=0 a 5
    printf("\n[A] GENERACION DE POLINOMIOS (x + 1)^n DESDE EL TRIANGULO DE PASCAL:\n");
    printf("-----------------------------------------------------------------\n");
    u128** tri_demo = generar_triangulo_pascal(5);
    for (int i = 0; i <= 5; i++) {
        printf("n = %d | Coeficientes: [", i);
        for (int j = 0; j <= i; j++) {
            print_u128(tri_demo[i][j]);
            if (j < i) printf(", ");
        }
        printf("]\n       -> (x + 1)^%d = ", i);
        imprimir_polinomio(i, tri_demo[i]);
        printf("\n");
    }
    liberar_triangulo_pascal(tri_demo, 5);

    // [B] Demostración paso a paso
    printf("\n[B] DEMOSTRACION DEL CALCULO EVALUADO PASO A PASO:\n");
    evaluar_paso_a_paso(4, 2.0);
    printf("\n");
    evaluar_paso_a_paso(3, 5.0);

    // [C] Benchmark hasta n = 100
    ejecutar_benchmark_y_guardar(10000);

    return 0;
}
