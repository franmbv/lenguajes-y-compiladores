#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para almacenar la palabra reservada y su traducción
typedef struct {
    const char* ingles;
    const char* espanol;
} Traduccion;

// Lista de palabras reservadas comunes en C y su equivalente al español
Traduccion palabras_reservadas[] = {
    {"auto", "automatico"},
    {"break", "romper"},
    {"case", "caso"},
    {"char", "caracter"},
    {"const", "constante"},
    {"continue", "continuar"},
    {"default", "por_defecto"},
    {"do", "hacer"},
    {"double", "doble"},
    {"else", "sino"},
    {"enum", "enumeracion"},
    {"extern", "externo"},
    {"float", "flotante"},
    {"for", "para"},
    {"goto", "ir_a"},
    {"if", "si"},
    {"int", "entero"},
    {"long", "largo"},
    {"register", "registro"},
    {"return", "retornar"},
    {"short", "corto"},
    {"signed", "con_signo"},
    {"sizeof", "tamano_de"},
    {"static", "estatico"},
    {"struct", "estructura"},
    {"switch", "segun"},
    {"typedef", "definir_tipo"},
    {"union", "union"},
    {"unsigned", "sin_signo"},
    {"void", "vacio"},
    {"volatile", "volatil"},
    {"while", "mientras"},
    {"include", "incluir"},
    {"define", "definir"}
};

int total_reservadas = sizeof(palabras_reservadas) / sizeof(Traduccion);

// Función para cargar un archivo en memoria de forma dinámica
char* cargar_archivo_dinamico(const char* nombre_archivo, long* tamano_out) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        return NULL;
    }

    // Ir al final del archivo para medir su tamaño exacto
    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    // Asignación de memoria dinámica para almacenar todo el código fuente
    char* buffer = (char*)malloc(tamano + 1);
    if (!buffer) {
        fclose(archivo);
        return NULL;
    }

    long leidos = fread(buffer, 1, tamano, archivo);
    buffer[leidos] = '\0'; // Carácter nulo al final de la cadena
    
    fclose(archivo);
    *tamano_out = leidos;
    return buffer;
}

int main() {
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    printf("TRADUCTOR DE PALABRAS RESERVADAS EN C ---\n");

    char nombre_archivo[100];
    printf("Ingrese el nombre del archivo fuente a cargar (por defecto 'test.c'): ");
    
    // Leer el nombre del archivo ingresado por teclado
    if (fgets(nombre_archivo, sizeof(nombre_archivo), stdin) != NULL) {
        // Remover el salto de línea al final si el usuario presionó Enter
        nombre_archivo[strcspn(nombre_archivo, "\n")] = 0;
        
        // Si no escribió nada, asignamos 'test.c' por defecto
        if (strlen(nombre_archivo) == 0) {
            strcpy(nombre_archivo, "test.c");
        }
    } else {
        strcpy(nombre_archivo, "test.c");
    }

    // Intentamos cargar el archivo seleccionado en memoria de forma dinámica
    long tamano_codigo = 0;
    char* codigo_memoria = cargar_archivo_dinamico(nombre_archivo, &tamano_codigo);

    if (!codigo_memoria) {
        printf("[✖] Error: No se pudo abrir el archivo '%s'. Asegúrate de que esté en la misma carpeta.\n", nombre_archivo);
        return 1;
    }

    printf("[✔] Archivo '%s' cargado exitosamente en memoria dinámica (%ld bytes).\n\n", nombre_archivo, tamano_codigo);

    // Abrir el archivo result.txt para guardar el reporte
    FILE* result = fopen("result.txt", "w");
    if (!result) {
        printf("[✖] Error al crear el archivo result.txt\n");
        free(codigo_memoria);
        return 1;
    }

    fprintf(result, "=== REPORTE DE PRUEBA DIAGNOSTICA - EJERCICIO 3 ===\n");
    fprintf(result, "Archivo analizado: %s\n", nombre_archivo);
    fprintf(result, "Verificacion y traduccion de palabras reservadas de C cargadas en memoria:\n\n");

    printf("--- PALABRAS RESERVADAS ENCONTRADAS Y TRADUCIDAS ---\n");
    
    // Analizar el texto en memoria buscando las palabras reservadas
    int encontradas = 0;
    for (int i = 0; i < total_reservadas; i++) {
        if (strstr(codigo_memoria, palabras_reservadas[i].ingles) != NULL) {
            printf(" [C] '%-10s'  --->  [ESPANOL] '%s'\n", 
                   palabras_reservadas[i].ingles, 
                   palabras_reservadas[i].espanol);

            fprintf(result, "Palabra C: %-10s ==> Traduccion: %s\n", 
                    palabras_reservadas[i].ingles, 
                    palabras_reservadas[i].espanol);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf(" No se encontraron palabras reservadas conocidas en este archivo.\n");
        fprintf(result, "No se encontraron palabras reservadas conocidas.\n");
    }

    fclose(result);
    printf("\n[✔] Reporte guardado con éxito en el archivo 'result.txt'.\n");

    // Liberar la memoria dinámica reservada
    free(codigo_memoria);

    return 0;
}
