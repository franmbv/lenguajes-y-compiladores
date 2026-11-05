#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *ingles;
    const char *espanol;
} PalabraReservada;

static const PalabraReservada PALABRAS_RESERVADAS[] = {
    {"auto", "automatico"},
    {"break", "romper"},
    {"case", "caso"},
    {"char", "caracter"},
    {"const", "constante"},
    {"continue", "continuar"},
    {"default", "predeterminado"},
    {"do", "hacer"},
    {"double", "doble"},
    {"else", "sino"},
    {"enum", "enumeracion"},
    {"extern", "externo"},
    {"float", "flotante"},
    {"for", "para"},
    {"goto", "ir_a"},
    {"if", "si"},
    {"inline", "en_linea"},
    {"int", "entero"},
    {"long", "largo"},
    {"register", "registro"},
    {"restrict", "restringido"},
    {"return", "retornar"},
    {"short", "corto"},
    {"signed", "con_signo"},
    {"sizeof", "tamano_de"},
    {"static", "estatico"},
    {"struct", "estructura"},
    {"switch", "seleccionar"},
    {"typedef", "definir_tipo"},
    {"union", "union"},
    {"unsigned", "sin_signo"},
    {"void", "vacio"},
    {"volatile", "volatil"},
    {"while", "mientras"},
    {"_Alignas", "alinear_como"},
    {"_Alignof", "alineacion_de"},
    {"_Atomic", "atomico"},
    {"_Bool", "booleano"},
    {"_Complex", "complejo"},
    {"_Generic", "generico"},
    {"_Imaginary", "imaginario"},
    {"_Noreturn", "sin_retorno"},
    {"_Static_assert", "asercion_estatica"},
    {"_Thread_local", "local_del_hilo"}
};

static const size_t CANTIDAD_RESERVADAS =
    sizeof(PALABRAS_RESERVADAS) / sizeof(PALABRAS_RESERVADAS[0]);

static char *cargarArchivoMemoria(const char *ruta) {
    FILE *archivo = fopen(ruta, "rb");
    long tamano;
    char *texto;
    size_t leidos;

    if (archivo == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo \"%s\".\n", ruta);
        return NULL;
    }

    if (fseek(archivo, 0, SEEK_END) != 0) {
        fclose(archivo);
        fprintf(stderr, "No se pudo determinar el tamano del archivo.\n");
        return NULL;
    }

    tamano = ftell(archivo);
    if (tamano < 0 || fseek(archivo, 0, SEEK_SET) != 0) {
        fclose(archivo);
        fprintf(stderr, "No se pudo leer el archivo \"%s\".\n", ruta);
        return NULL;
    }

    texto = malloc((size_t)tamano + 1);
    if (texto == NULL) {
        fclose(archivo);
        fprintf(stderr, "No hay memoria suficiente para cargar el archivo.\n");
        return NULL;
    }

    leidos = fread(texto, 1, (size_t)tamano, archivo);
    fclose(archivo);
    if (leidos != (size_t)tamano) {
        free(texto);
        fprintf(stderr, "Ocurrio un error al leer el archivo \"%s\".\n", ruta);
        return NULL;
    }

    texto[leidos] = '\0';
    return texto;
}

static const char *buscarTraduccion(const char *palabra) {
    size_t i;

    for (i = 0; i < CANTIDAD_RESERVADAS; i++) {
        if (strcmp(palabra, PALABRAS_RESERVADAS[i].ingles) == 0) {
            return PALABRAS_RESERVADAS[i].espanol;
        }
    }
    return NULL;
}

static int agregarCaracter(char **salida, size_t *longitud, size_t *capacidad,
                           char caracter) {
    char *temporal;

    if (*longitud + 1 >= *capacidad) {
        *capacidad *= 2;
        temporal = realloc(*salida, *capacidad);
        if (temporal == NULL) {
            return 0;
        }
        *salida = temporal;
    }
    (*salida)[(*longitud)++] = caracter;
    (*salida)[*longitud] = '\0';
    return 1;
}

static int agregarTexto(char **salida, size_t *longitud, size_t *capacidad,
                        const char *texto) {
    size_t i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (!agregarCaracter(salida, longitud, capacidad, texto[i])) {
            return 0;
        }
    }
    return 1;
}

static int esInicioIdentificador(char caracter) {
    return isalpha((unsigned char)caracter) || caracter == '_';
}

static int esParteIdentificador(char caracter) {
    return isalnum((unsigned char)caracter) || caracter == '_';
}

static int copiarLiteralOComentario(const char *texto, size_t *indice,
                                    char **salida, size_t *longitud,
                                    size_t *capacidad) {
    size_t i = *indice;
    char inicio = texto[i];
    char cierre;

    if (inicio == '/' && (texto[i + 1] == '/' || texto[i + 1] == '*')) {
        if (!agregarCaracter(salida, longitud, capacidad, texto[i++]) ||
            !agregarCaracter(salida, longitud, capacidad, texto[i++])) {
            return 0;
        }
        if (inicio == '/' && texto[i - 1] == '/') {
            while (texto[i] != '\0' && texto[i] != '\n') {
                if (!agregarCaracter(salida, longitud, capacidad, texto[i++])) {
                    return 0;
                }
            }
        } else {
            while (texto[i] != '\0') {
                if (!agregarCaracter(salida, longitud, capacidad, texto[i])) {
                    return 0;
                }
                if (texto[i] == '*' && texto[i + 1] == '/') {
                    i++;
                    if (!agregarCaracter(salida, longitud, capacidad, texto[i])) {
                        return 0;
                    }
                    i++;
                    break;
                }
                i++;
            }
        }
        *indice = i;
        return 1;
    }

    cierre = inicio == '"' ? '"' : '\'';
    do {
        if (!agregarCaracter(salida, longitud, capacidad, texto[i])) {
            return 0;
        }
        if (texto[i] == '\\' && texto[i + 1] != '\0') {
            i++;
            if (!agregarCaracter(salida, longitud, capacidad, texto[i])) {
                return 0;
            }
        }
        i++;
    } while (texto[i] != '\0' && texto[i] != cierre);

    if (texto[i] == cierre && !agregarCaracter(salida, longitud, capacidad, texto[i])) {
        return 0;
    }
    *indice = texto[i] == '\0' ? i : i + 1;
    return 1;
}

static char *traducirPalabrasReservadas(const char *texto) {
    size_t capacidad = strlen(texto) + 1;
    size_t longitud = 0;
    size_t indice = 0;
    char *salida = malloc(capacidad);

    if (salida == NULL) {
        return NULL;
    }
    salida[0] = '\0';

    while (texto[indice] != '\0') {
        if (texto[indice] == '"' || texto[indice] == '\'' ||
            (texto[indice] == '/' &&
             (texto[indice + 1] == '/' || texto[indice + 1] == '*'))) {
            if (!copiarLiteralOComentario(texto, &indice, &salida, &longitud,
                                          &capacidad)) {
                free(salida);
                return NULL;
            }
        } else if (esInicioIdentificador(texto[indice])) {
            char palabra[128];
            size_t inicio = indice;
            size_t longitudPalabra;
            const char *traduccion;

            while (esParteIdentificador(texto[indice])) {
                indice++;
            }
            longitudPalabra = indice - inicio;
            if (longitudPalabra >= sizeof(palabra)) {
                size_t posicion;

                for (posicion = inicio; posicion < indice; posicion++) {
                    if (!agregarCaracter(&salida, &longitud, &capacidad,
                                         texto[posicion])) {
                        free(salida);
                        return NULL;
                    }
                }
            } else {
                memcpy(palabra, texto + inicio, longitudPalabra);
                palabra[longitudPalabra] = '\0';
                traduccion = buscarTraduccion(palabra);
                if (!agregarTexto(&salida, &longitud, &capacidad,
                                  traduccion == NULL ? palabra : traduccion)) {
                    free(salida);
                    return NULL;
                }
            }
        } else if (!agregarCaracter(&salida, &longitud, &capacidad, texto[indice++])) {
            free(salida);
            return NULL;
        }
    }
    return salida;
}

int main(int argc, char *argv[]) {
    const char *ruta = argc > 1 ? argv[1] : "codigo_fuente.c";
    char *texto = cargarArchivoMemoria(ruta);
    char *traducido;

    if (texto == NULL) {
        return EXIT_FAILURE;
    }

    traducido = traducirPalabrasReservadas(texto);
    if (traducido == NULL) {
        free(texto);
        fprintf(stderr, "No hay memoria suficiente para traducir el archivo.\n");
        return EXIT_FAILURE;
    }

    printf("=== Codigo original ===\n%s\n", texto);
    printf("=== Codigo traducido ===\n%s\n", traducido);

    free(traducido);
    free(texto);
    return EXIT_SUCCESS;
}
