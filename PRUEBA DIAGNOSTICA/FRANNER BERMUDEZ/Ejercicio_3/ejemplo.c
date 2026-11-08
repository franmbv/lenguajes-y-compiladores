/*
 * ejemplo.c - Programa de prueba para el Ejercicio 3
 * Contiene palabras reservadas de C, ademas de comentarios y cadenas
 * que mencionan palabras reservadas y que NO deben traducirse.
 */

#include <stdio.h>
#include <stdlib.h>

/* Comentario de bloque: if, while, return y for aqui no se traducen. */
typedef struct Nodo {
    int valor;
    struct Nodo *siguiente;
} Nodo;

enum Color {
    ROJO,
    VERDE
};

union Dato {
    int valor_entero;
    float valor_flotante;
};

static int sumar(const Nodo *nodo) {
    int total = 0;
    while (nodo != NULL) {
        total += nodo->valor;
        nodo = nodo->siguiente;
    }
    return total;
}

int main(void) {
    // Comentario de linea: int if else for while return no se traducen.
    const char *mensaje = "if while for return int tampoco se traducen";
    unsigned long cantidad = 3;
    Nodo *cabeza = NULL;
    volatile int bandera = 0;
    register int auxiliar = 0;

    for (unsigned long i = 0; i < cantidad; i++) {
        Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
        if (nuevo == NULL) {
            return 1;
        } else {
            nuevo->valor = (int)i;
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        }
    }

    printf("%s\n", mensaje);
    printf("Suma: %d\n", sumar(cabeza));

    switch (cantidad) {
        case 3:
            bandera = 1;
            break;
        default:
            goto fin;
    }

    while (cabeza != NULL) {
        Nodo *temporal = cabeza;
        cabeza = cabeza->siguiente;
        free(temporal);
    }

fin:
    auxiliar = bandera;
    (void)auxiliar;
    return 0;
}
