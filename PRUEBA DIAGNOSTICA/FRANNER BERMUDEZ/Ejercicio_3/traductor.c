#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==========================================================================
 * 1. TABLA DE PALABRAS RESERVADAS DE C Y SU EQUIVALENTE EN ESPANOL
 * ========================================================================== */
typedef struct {
    const char *c;
    const char *espanol;
    size_t conteo;
} PalabraReservada;

static PalabraReservada tabla[] = {
    /* C89 / C90 */
    { "auto",       "automatico",      0 },
    { "break",      "romper",          0 },
    { "case",       "caso",            0 },
    { "char",       "caracter",        0 },
    { "const",      "constante",       0 },
    { "continue",   "continuar",       0 },
    { "default",    "predeterminado",  0 },
    { "do",         "hacer",           0 },
    { "double",     "doble",           0 },
    { "else",       "sino",            0 },
    { "enum",       "enumeracion",     0 },
    { "extern",     "externo",         0 },
    { "float",      "flotante",        0 },
    { "for",        "para",            0 },
    { "goto",       "ir_a",            0 },
    { "if",         "si",              0 },
    { "int",        "entero",          0 },
    { "long",       "largo",           0 },
    { "register",   "registro",        0 },
    { "return",     "retornar",        0 },
    { "short",      "corto",           0 },
    { "signed",     "con_signo",       0 },
    { "sizeof",     "tamano_de",       0 },
    { "static",     "estatico",        0 },
    { "struct",     "estructura",      0 },
    { "switch",     "cambiar",         0 },
    { "typedef",    "tipo_definido",   0 },
    { "union",      "union",           0 },
    { "unsigned",   "sin_signo",       0 },
    { "void",       "vacio",           0 },
    { "volatile",   "volatil",         0 },
    { "while",      "mientras",        0 },
    /* C99 / C11 */
    { "inline",     "en_linea",        0 },
    { "restrict",   "restringido",     0 },
    { "_Bool",      "booleano",        0 },
    { "_Complex",   "complejo",        0 },
    { "_Imaginary", "imaginario",      0 },
    { "_Alignas",   "alinear_como",    0 },
    { "_Alignof",   "alineacion_de",   0 },
    { "_Atomic",    "atomico",         0 },
    { "_Generic",   "generico",        0 },
    { "_Noreturn",  "sin_retorno",     0 },
    { "_Static_assert", "verificacion_estatica", 0 },
    { "_Thread_local",  "hilo_local",           0 }
};

static const size_t NUM_PALABRAS = sizeof(tabla) / sizeof(tabla[0]);

/* Devuelve el indice de la palabra reservada o -1 si no lo es. */
static int buscar_palabra(const char *id) {
    for (size_t k = 0; k < NUM_PALABRAS; k++) {
        if (strcmp(id, tabla[k].c) == 0) {
            return (int)k;
        }
    }
    return -1;
}

/* ==========================================================================
 * 2. CARGA DEL ARCHIVO EN MEMORIA DINAMICA
 * ========================================================================== */
static char *cargar_archivo(const char *ruta, size_t *tamano) {
    FILE *f = fopen(ruta, "rb");
    if (!f) {
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long n = ftell(f);
    if (n < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);

    /* Asignacion dinamica del buffer que contendra todo el programa. */
    char *buffer = (char *)malloc((size_t)n + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    size_t leidos = fread(buffer, 1, (size_t)n, f);
    fclose(f);

    buffer[leidos] = '\0';
    *tamano = leidos;
    return buffer;
}

/* ==========================================================================
 * 3. ESCANER Y TRADUCCION
 * ========================================================================== */
static int traducir(const char *codigo, size_t n, FILE *salida) {
    size_t i = 0;
    int sustituciones = 0;

    while (i < n) {
        unsigned char c = (unsigned char)codigo[i];

        /* Comentario de linea: se copia sin traducir. */
        if (c == '/' && i + 1 < n && codigo[i + 1] == '/') {
            size_t j = i;
            while (j < n && codigo[j] != '\n') {
                j++;
            }
            fwrite(codigo + i, 1, j - i, salida);
            i = j;
            continue;
        }

        /* Comentario de bloque: se copia sin traducir. */
        if (c == '/' && i + 1 < n && codigo[i + 1] == '*') {
            size_t j = i + 2;
            while (j + 1 < n && !(codigo[j] == '*' && codigo[j + 1] == '/')) {
                j++;
            }
            if (j + 1 < n) {
                j += 2;
            } else {
                j = n;
            }
            fwrite(codigo + i, 1, j - i, salida);
            i = j;
            continue;
        }

        /* Cadena de texto o caracter: se copia sin traducir. */
        if (c == '"' || c == '\'') {
            char comilla = (char)c;
            size_t j = i + 1;
            while (j < n) {
                if (codigo[j] == '\\' && j + 1 < n) {
                    j += 2;
                    continue;
                }
                if (codigo[j] == comilla) {
                    j++;
                    break;
                }
                if (codigo[j] == '\n') {
                    break;
                }
                j++;
            }
            fwrite(codigo + i, 1, j - i, salida);
            i = j;
            continue;
        }

        /* Identificador o palabra reservada. */
        if (isalpha(c) || c == '_') {
            size_t j = i + 1;
            while (j < n) {
                unsigned char d = (unsigned char)codigo[j];
                if (isalnum(d) || d == '_') {
                    j++;
                } else {
                    break;
                }
            }

            size_t len = j - i;
            char id[128];
            int idx = -1;

            if (len < sizeof(id)) {
                memcpy(id, codigo + i, len);
                id[len] = '\0';
                idx = buscar_palabra(id);
            }

            if (idx >= 0) {
                fputs(tabla[idx].espanol, salida);
                tabla[idx].conteo++;
                sustituciones++;
            } else {
                fwrite(codigo + i, 1, len, salida);
            }

            i = j;
            continue;
        }

        /* Cualquier otro caracter se copia tal cual. */
        fputc(codigo[i], salida);
        i++;
    }

    return sustituciones;
}

/* ==========================================================================
 * 4. PROGRAMA PRINCIPAL
 * ========================================================================== */
int main(int argc, char **argv) {
    const char *ruta_entrada = (argc > 1) ? argv[1] : "ejemplo.c";
    const char *ruta_salida  = (argc > 2) ? argv[2] : "salida_traducida.txt";

    printf("=================================================================\n");
    printf("EJERCICIO 3: TRADUCTOR DE PALABRAS RESERVADAS DE C A ESPANOL\n");
    printf("Lenguajes y Compiladores - Prueba Diagnostica\n");
    printf("Estudiante: Franner Bermudez\n");
    printf("=================================================================\n");
    printf("Archivo de entrada : %s\n", ruta_entrada);
    printf("Archivo de salida  : %s\n\n", ruta_salida);

    size_t tamano = 0;
    char *codigo = cargar_archivo(ruta_entrada, &tamano);
    if (!codigo) {
        fprintf(stderr, "[ERROR] No se pudo abrir o cargar el archivo '%s'.\n", ruta_entrada);
        return 1;
    }
    printf("[OK] Programa cargado dinamicamente en memoria: %zu bytes.\n\n", tamano);

    FILE *salida = fopen(ruta_salida, "wb");
    if (!salida) {
        fprintf(stderr, "[ERROR] No se pudo crear el archivo de salida '%s'.\n", ruta_salida);
        free(codigo);
        return 1;
    }

    int sustituciones = traducir(codigo, tamano, salida);
    fclose(salida);
    free(codigo);

    /* Resumen de palabras reservadas encontradas. */
    printf("Palabras reservadas encontradas:\n");
    printf("-----------------------------------------------------------------\n");

    size_t distintas = 0;
    for (size_t k = 0; k < NUM_PALABRAS; k++) {
        if (tabla[k].conteo > 0) {
            printf("  %-16s -> %-24s (%zu)\n",
                   tabla[k].c, tabla[k].espanol, tabla[k].conteo);
            distintas++;
        }
    }

    if (distintas == 0) {
        printf("  No se encontraron palabras reservadas de C en el programa.\n");
    } else {
        printf("-----------------------------------------------------------------\n");
        printf("  Tipos distintos : %zu\n", distintas);
        printf("  Total sustituido: %d\n", sustituciones);
    }

    printf("\n[OK] Programa traducido guardado en: %s\n", ruta_salida);
    printf("=================================================================\n");
    return 0;
}
