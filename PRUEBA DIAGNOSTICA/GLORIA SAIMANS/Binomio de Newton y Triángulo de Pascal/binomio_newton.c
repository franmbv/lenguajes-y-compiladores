#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int leerEnteroNoNegativo(const char *texto, unsigned long *valor) {
    char *fin;
    unsigned long resultado;

    errno = 0;
    resultado = strtoul(texto, &fin, 10);
    if (errno != 0 || fin == texto || *fin != '\0') {
        return 0;
    }

    *valor = resultado;
    return 1;
}

static int leerNumeroReal(const char *texto, double *valor) {
    char *fin;
    double resultado;

    errno = 0;
    resultado = strtod(texto, &fin);
    if (errno != 0 || fin == texto || *fin != '\0') {
        return 0;
    }

    *valor = resultado;
    return 1;
}

double *generarPascal(unsigned long n) {
    size_t cantidad;
    double *coeficientes;
    size_t i;

    if (n == (unsigned long)-1) {
        return NULL;
    }

    cantidad = (size_t)n + 1;
    if (cantidad <= (size_t)n || cantidad > (size_t)-1 / sizeof(*coeficientes)) {
        return NULL;
    }

    coeficientes = malloc(cantidad * sizeof(*coeficientes));
    if (coeficientes == NULL) {
        return NULL;
    }

    coeficientes[0] = 1.0;
    for (i = 1; i < cantidad; i++) {
        coeficientes[i] = coeficientes[i - 1] * (double)(n - i + 1) / (double)i;
    }

    return coeficientes;
}

void imprimirPolinomio(const double *coeficientes, unsigned long n) {
    long long exponente;

    if (n == 0) {
        printf("1\n");
        return;
    }

    for (exponente = (long long)n; exponente >= 0; exponente--) {
        double coeficiente = coeficientes[n - (unsigned long)exponente];
        int esConstante = exponente == 0;
        int esPrimero = exponente == (long long)n;

        if (!esPrimero) {
            printf(" + ");
        }

        if (esConstante || coeficiente != 1.0L) {
            printf("%.0f", coeficiente);
        }
        if (!esConstante) {
            printf("x");
            if (exponente > 1) {
                printf("^%lld", exponente);
            }
        }
    }
    printf("\n");
}

double evaluarPolinomio(const double *coeficientes, unsigned long n, double x) {
    double resultado = 0.0;
    long long exponente;

    printf("\nEvaluacion paso a paso:\n");
    for (exponente = (long long)n; exponente >= 0; exponente--) {
        double termino = coeficientes[n - (unsigned long)exponente];

        for (long long potencia = 0; potencia < exponente; potencia++) {
            termino *= x;
        }

        if (exponente == 0) {
            printf("Termino constante: %.0f", termino);
        } else {
            printf("%.0f * (%.6f)^%lld = %.6f",
                   coeficientes[n - (unsigned long)exponente],
                   x,
                   exponente,
                   termino);
        }
        resultado += termino;
        printf("  -> acumulado: %.6f\n", resultado);
    }

    printf("Resultado final: %.6f\n", resultado);
    return resultado;
}

static double medirGeneracion(unsigned long n, unsigned long repeticiones) {
    clock_t inicio = clock();
    unsigned long repeticion;

    for (repeticion = 0; repeticion < repeticiones; repeticion++) {
        double *coeficientes = generarPascal(n);
        if (coeficientes == NULL) {
            fprintf(stderr, "No se pudo reservar memoria para n=%lu.\n", n);
            exit(EXIT_FAILURE);
        }
        free(coeficientes);
    }

    return (double)(clock() - inicio) * 1000.0 / CLOCKS_PER_SEC / (double)repeticiones;
}

int main(void) {
    char entradaN[64];
    char entradaX[64];
    unsigned long n;
    double x;
    double *coeficientes;
    double tiempoPromedio;

    printf("Calculo del polinomio (x + 1)^n\n");
    printf("Ingrese n (entero no negativo): ");
    if (scanf("%63s", entradaN) != 1 || !leerEnteroNoNegativo(entradaN, &n)) {
        fprintf(stderr, "Error: n debe ser un entero no negativo.\n");
        return EXIT_FAILURE;
    }

    printf("Ingrese el valor de x: ");
    if (scanf("%63s", entradaX) != 1 || !leerNumeroReal(entradaX, &x)) {
        fprintf(stderr, "Error: x debe ser un numero real.\n");
        return EXIT_FAILURE;
    }

    coeficientes = generarPascal(n);
    if (coeficientes == NULL) {
        fprintf(stderr, "Error: no fue posible reservar memoria para n=%lu.\n", n);
        return EXIT_FAILURE;
    }

    printf("\nPolinomio generado:\n");
    imprimirPolinomio(coeficientes, n);
    evaluarPolinomio(coeficientes, n, x);
    free(coeficientes);

    tiempoPromedio = medirGeneracion(100, 100000);
    printf("\nRendimiento C (generacion de n=100, promedio de 100000 ejecuciones): %.6f ms\n",
           tiempoPromedio);
    return EXIT_SUCCESS;
}
