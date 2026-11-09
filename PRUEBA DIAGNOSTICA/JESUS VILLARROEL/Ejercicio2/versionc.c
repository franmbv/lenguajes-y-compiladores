#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int generarCoeficientes(unsigned long long coeficientes[], size_t n)
{
    coeficientes[0] = 1;
    printf("\nTriangulo de Pascal:\n(n = 0): 1\n");

    for (size_t fila = 1; fila <= n; ++fila) {
        coeficientes[fila] = 1;
        for (size_t columna = fila - 1; columna > 0; --columna) {
            if (coeficientes[columna] > ULLONG_MAX - coeficientes[columna - 1]) {
                return 0;
            }
            coeficientes[columna] += coeficientes[columna - 1];
        }

        printf("(n = %zu): ", fila);
        for (size_t columna = 0; columna <= fila; ++columna) {
            printf("%llu%s", coeficientes[columna],
                   columna == fila ? "\n" : " ");
        }
    }

    return 1;
}

static void mostrarPolinomio(const unsigned long long coeficientes[], size_t n)
{
    printf("\nPolinomio generado:\n(x + 1)^%zu = ", n);
    for (size_t exponente = n;; --exponente) {
        unsigned long long coeficiente = coeficientes[exponente];

        if (exponente != n) {
            printf(" + ");
        }
        if (coeficiente != 1 || exponente == 0) {
            printf("%llu", coeficiente);
        }
        if (exponente > 0) {
            printf("x");
            if (exponente > 1) {
                printf("^%zu", exponente);
            }
        }

        if (exponente == 0) {
            break;
        }
    }
    printf("\n");
}

static void evaluarPolinomio(const unsigned long long coeficientes[],
                             size_t n, double x)
{
    printf("\nEvaluacion paso a paso para x = %.10g:\n", x);
    double suma = 0.0;
    for (size_t exponente = n;; --exponente) {
        double valorPotencia = pow(x, (double)exponente);
        double termino = (double)coeficientes[exponente] * valorPotencia;
        suma += termino;

        printf("Paso %zu: %llu * (%.10g)^%zu = %.10g; acumulado = %.10g\n",
               n - exponente + 1, coeficientes[exponente], x,
               exponente,
               termino, suma);

        if (exponente == 0) {
            break;
        }
    }
    printf("\nResultado final:\n");
    printf("f(%.10g) = %.10g\n\n", x, suma);
}

int main(void)
{
    long long entradaN;
    double x;
    size_t n;
    unsigned long long *coeficientes;

    printf("Ingrese un entero no negativo n: ");
    if (scanf("%lld", &entradaN) != 1 || entradaN < 0) {
        fprintf(stderr, "Error: n debe ser un entero no negativo.\n");
        return 1;
    }

    printf("Ingrese el valor de x: ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: x debe ser un numero real.\n");
        return 1;
    }

    if ((uintmax_t)entradaN > (uintmax_t)SIZE_MAX) {
        fprintf(stderr, "Error: n es demasiado grande para este sistema.\n");
        return 1;
    }
    n = (size_t)entradaN;

    if (n >= SIZE_MAX / sizeof(*coeficientes)) {
        fprintf(stderr, "Error: no hay espacio direccionable para los coeficientes.\n");
        return 1;
    }

    coeficientes = malloc((n + 1) * sizeof(*coeficientes));
    if (coeficientes == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para los coeficientes.\n");
        return 1;
    }

    clock_t inicio = clock();
    if (inicio == (clock_t)-1) {
        fprintf(stderr, "Error: no se pudo iniciar la medicion del tiempo.\n");
        free(coeficientes);
        return 1;
    }

    if (!generarCoeficientes(coeficientes, n)) {
        fprintf(stderr, "Error: los coeficientes exceden el rango numerico permitido.\n");
        free(coeficientes);
        return 1;
    }

    mostrarPolinomio(coeficientes, n);
    evaluarPolinomio(coeficientes, n, x);

    if (fflush(stdout) != 0) {
        perror("Error al mostrar el resultado");
        free(coeficientes);
        return 1;
    }

    clock_t fin = clock();
    if (fin == (clock_t)-1) {
        fprintf(stderr, "Error: no se pudo finalizar la medicion del tiempo.\n");
        free(coeficientes);
        return 1;
    }
    double tiempoSegundos = ((double)fin - (double)inicio) / CLOCKS_PER_SEC;

    free(coeficientes);

    printf("Tiempo de ejecucion: %.9f segundos\n", tiempoSegundos);
    return 0;
}