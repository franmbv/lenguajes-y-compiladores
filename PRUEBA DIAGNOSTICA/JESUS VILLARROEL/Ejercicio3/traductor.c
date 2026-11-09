#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *ingles;
    const char *espanol;
    size_t cantidad;
} PalabraReservada;

static PalabraReservada palabras[] = {
    {"auto", "automatico", 0},
    {"break", "romper", 0},
    {"case", "caso", 0},
    {"char", "caracter", 0},
    {"const", "constante", 0},
    {"continue", "continuar", 0},
    {"default", "predeterminado", 0},
    {"do", "hacer", 0},
    {"double", "doble", 0},
    {"else", "sino", 0},
    {"enum", "enumeracion", 0},
    {"extern", "externo", 0},
    {"float", "real", 0},
    {"for", "para", 0},
    {"goto", "ir_a", 0},
    {"if", "si", 0},
    {"inline", "en_linea", 0},
    {"int", "entero", 0},
    {"long", "largo", 0},
    {"register", "registro", 0},
    {"restrict", "restringido", 0},
    {"return", "retornar", 0},
    {"short", "corto", 0},
    {"signed", "con_signo", 0},
    {"sizeof", "tamano_de", 0},
    {"static", "estatico", 0},
    {"struct", "estructura", 0},
    {"switch", "seleccionar", 0},
    {"typedef", "definir_tipo", 0},
    {"union", "union", 0},
    {"unsigned", "sin_signo", 0},
    {"void", "vacio", 0},
    {"volatile", "volatil", 0},
    {"while", "mientras", 0},
    {"_Alignas", "alinear", 0},
    {"_Alignof", "alineacion_de", 0},
    {"_Atomic", "atomico", 0},
    {"_Bool", "booleano", 0},
    {"_Complex", "complejo", 0},
    {"_Generic", "generico", 0},
    {"_Imaginary", "imaginario", 0},
    {"_Noreturn", "sin_retorno", 0},
    {"_Static_assert", "afirmacion_estatica", 0},
    {"_Thread_local", "hilo_local", 0},
    {"free", "liberar", 0},
    {"malloc", "reservar_memoria", 0},
    {"calloc", "reservar_memoria_inicializada", 0},
    {"realloc", "reasignar_memoria", 0},
    {"exit", "salir", 0},
    {"abort", "abortar", 0},
    {"assert", "afirmar", 0},
};

static char *leer_entrada(FILE *entrada, size_t *longitud)
{
    size_t capacidad = 4096;
    size_t usados = 0;
    char *contenido = malloc(capacidad);

    if (contenido == NULL) {
        return NULL;
    }

    for (;;) {
        size_t leidos;

        if (usados == capacidad - 1) {
            size_t nueva_capacidad;
            char *nuevo_contenido;

            if (capacidad > SIZE_MAX / 2) {
                free(contenido);
                return NULL;
            }
            nueva_capacidad = capacidad * 2;
            nuevo_contenido = realloc(contenido, nueva_capacidad);
            if (nuevo_contenido == NULL) {
                free(contenido);
                return NULL;
            }
            contenido = nuevo_contenido;
            capacidad = nueva_capacidad;
        }

        leidos = fread(contenido + usados, 1, capacidad - usados - 1,
                       entrada);
        usados += leidos;
        if (leidos == 0) {
            if (ferror(entrada)) {
                free(contenido);
                return NULL;
            }
            break;
        }
    }

    contenido[usados] = '\0';
    *longitud = usados;
    return contenido;
}

static int es_caracter_identificador(unsigned char caracter)
{
    return (caracter >= 'a' && caracter <= 'z')
        || (caracter >= 'A' && caracter <= 'Z')
        || (caracter >= '0' && caracter <= '9')
        || caracter == '_';
}

static PalabraReservada *buscar_palabra(const char *texto, size_t longitud)
{
    size_t indice;

    for (indice = 0; indice < sizeof(palabras) / sizeof(palabras[0]);
         ++indice) {
        if (strlen(palabras[indice].ingles) == longitud
                && memcmp(texto, palabras[indice].ingles, longitud) == 0) {
            return &palabras[indice];
        }
    }
    return NULL;
}

static int traducir(const char *contenido, size_t longitud)
{
    size_t inicio_segmento = 0;
    size_t posicion = 0;

    while (posicion < longitud) {
        if (contenido[posicion] == '/' && posicion + 1 < longitud
                && contenido[posicion + 1] == '/') {
            posicion += 2;
            while (posicion < longitud) {
                if (contenido[posicion] == '\\' && posicion + 1 < longitud
                        && (contenido[posicion + 1] == '\n'
                            || contenido[posicion + 1] == '\r')) {
                    posicion += 2;
                    if (posicion < longitud && contenido[posicion - 1] == '\r'
                            && contenido[posicion] == '\n') {
                        ++posicion;
                    }
                } else if (contenido[posicion] == '\n'
                        || contenido[posicion] == '\r') {
                    break;
                } else {
                    ++posicion;
                }
            }
        } else if (contenido[posicion] == '/' && posicion + 1 < longitud
                && contenido[posicion + 1] == '*') {
            posicion += 2;
            while (posicion + 1 < longitud
                    && !(contenido[posicion] == '*'
                         && contenido[posicion + 1] == '/')) {
                ++posicion;
            }
            if (posicion + 1 < longitud) {
                posicion += 2;
            } else {
                posicion = longitud;
            }
        } else if (contenido[posicion] == '"' || contenido[posicion] == '\'') {
            char delimitador = contenido[posicion++];

            while (posicion < longitud) {
                if (contenido[posicion] == '\\' && posicion + 1 < longitud) {
                    posicion += 2;
                } else if (contenido[posicion++] == delimitador) {
                    break;
                }
            }
        } else if (es_caracter_identificador(
                       (unsigned char)contenido[posicion])
                && !(contenido[posicion] >= '0'
                     && contenido[posicion] <= '9')) {
            size_t inicio_palabra = posicion;
            PalabraReservada *palabra;

            do {
                ++posicion;
            } while (posicion < longitud
                    && es_caracter_identificador(
                        (unsigned char)contenido[posicion]));

            palabra = buscar_palabra(contenido + inicio_palabra,
                                      posicion - inicio_palabra);
            if (palabra != NULL) {
                if (fwrite(contenido + inicio_segmento, 1,
                           inicio_palabra - inicio_segmento, stdout)
                            != inicio_palabra - inicio_segmento
                        || fputs(palabra->espanol, stdout) == EOF) {
                    return 0;
                }
                inicio_segmento = posicion;
                ++palabra->cantidad;
            }
        } else {
            ++posicion;
        }
    }

    return fwrite(contenido + inicio_segmento, 1,
                  longitud - inicio_segmento, stdout)
        == longitud - inicio_segmento;
}

static void mostrar_resumen(void)
{
    size_t indice;
    size_t total = 0;

    for (indice = 0; indice < sizeof(palabras) / sizeof(palabras[0]);
         ++indice) {
        total += palabras[indice].cantidad;
    }

    if (total == 0) {
        fprintf(stderr, "No se encontraron palabras reservadas de C.\n");
        return;
    }

    fprintf(stderr, "\n\nPalabras reservadas encontradas y traducidas:\n");
    for (indice = 0; indice < sizeof(palabras) / sizeof(palabras[0]);
         ++indice) {
        if (palabras[indice].cantidad > 0) {
            fprintf(stderr, "  %s -> %s (%zu)\n", palabras[indice].ingles,
                    palabras[indice].espanol, palabras[indice].cantidad);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *entrada = stdin;
    char *contenido;
    size_t longitud;
    int resultado = EXIT_SUCCESS;

    if (argc > 2) {
        fprintf(stderr, "Uso: %s [archivo.c]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        entrada = fopen(argv[1], "rb");
        if (entrada == NULL) {
            perror("No se pudo abrir el archivo");
            return EXIT_FAILURE;
        }
    }

    contenido = leer_entrada(entrada, &longitud);
    if (entrada != stdin && fclose(entrada) != 0) {
        perror("No se pudo cerrar el archivo");
        free(contenido);
        return EXIT_FAILURE;
    }
    if (contenido == NULL) {
        fprintf(stderr, "Error al leer la entrada o reservar memoria.\n");
        return EXIT_FAILURE;
    }

    if (!traducir(contenido, longitud) || fflush(stdout) == EOF) {
        perror("Error al escribir el resultado");
        resultado = EXIT_FAILURE;
    } else {
        mostrar_resumen();
    }

    free(contenido);
    return resultado;
}