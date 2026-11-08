#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <fstream> // Necesario para crear y escribir en result.txt

using namespace std;

// 1. Traducir columna de letra a número
int transformar_columna(char literal_columna) {
    char columna_mapeada[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    char letra = tolower(literal_columna);
    for (int i = 0; i < 8; i++) {
        if (letra == columna_mapeada[i]) {
            return i + 1;
        }
    }
    return 0;
}

// 2. Validar el movimiento del caballo
bool validar_movimiento(string pos_X, string pos_Y) {
    // Quitar la 'K' y convertir a minúsculas manualmente
    string pX = "", pY = "";
    
    for (char c : pos_X) {
        if (c != 'K' && c != 'k') {
            pX += tolower(c);
        }
    }

    for (char c : pos_Y) { 
        if (c != 'K' && c != 'k') {
            pY += tolower(c); 
        }
    }

    int col_X = transformar_columna(pX[0]);
    int fila_X = pX[1] - '0'; // Convierte char '1'-'8' a int 1-8

    int col_Y = transformar_columna(pY[0]);
    int fila_Y = pY[1] - '0';

    int dif_f = abs(fila_Y - fila_X);
    int dif_c = abs(col_Y - col_X);

    return (dif_f == 2 && dif_c == 1) || (dif_f == 1 && dif_c == 2);
}

// 3. Recorrer la lista comparando el elemento anterior con el nuevo
bool procesar_movimientos(const vector<string>& lista) {
    cout << "\n--- EJECUCIÓN ---\n";

    for (size_t i = 1; i < lista.size(); i++) {
        string anterior = lista[i - 1];
        string actual = lista[i];

        if (!validar_movimiento(anterior, actual)) {
            cout << "MOVIMIENTO INCORRECTO: El movimiento " << i << " (" << actual << ") no es válido desde " << anterior << ".\n";
            return false; // Muestra el error pero deja que el programa continúe si está en el ciclo de 10 veces
        }

        // Convertir a mayúsculas para mostrar en consola
        string ant_mayus = anterior, act_mayus = actual;
        for (auto &c : ant_mayus) c = toupper(c);
        for (auto &c : act_mayus) c = toupper(c);

        cout << ant_mayus << " -> " << act_mayus << "\n";
    }

    cout << "\n¡Todos los movimientos se ejecutaron con éxito!\n";
    return true;
}

int main() {
    // Para admitir caracteres limpios en consola
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    cout << "--- SIMULADOR DE CABALLO DE AJEDREZ (C++) ---\n";
    cout << "1. Usar cadena precargada de la guía (500 movimientos)\n";
    cout << "2. Ingresar datos manualmente\n";
    cout << "Seleccione una opción (1 o 2): ";

    int opcion;
    cin >> opcion;

    vector<string> lista_movimientos;

    if (opcion == 1) {
        // Cadena precargada dividida en un vector en C++
        lista_movimientos = {
            "Kb4", "Ka6", "Kc5", "Ka4", "Kc3", "Kc2", "Kc1", "Ke2", "Kg1", "Kh3", "Kh5", "Ke6", "Kc7", "Ka8", "Ka8", "Kb5", "Kd6", "Kb7", "Ka5", 
            "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Ka6", "Kb8", "Kd7", "Kb6", "Ka8", "Kc7", "Ka6", "Kc5", "Ka4", "Kc3", "Ka2", "Kc1", "Ke2", 
            "Kg3", "Kh5", "Kg7", "Ke8", "Kc7", "Ka8", "Ka8", "Kb5", "Kd4", "Kb3", "Ka1", "Kc2", "Ka3", "Kc4", "Kc5", "Kc6", "Ka7", "Kc8", "Ka7", 
            "Kc6", "Ka5", "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Kd5", "Kb6", "Ka8", "Kc7", "Ka6", "Kc5", "Ka4", "Kc3", "Ka2", "Kc1", "Ke2", 
            "Kg1", "Kh3", "Kh5", "Ke6", "Kc5", "Ka4", "Ka3", "Ka2", "Kc1", "Ke2", "Kg3", "Kh5", "Kg7", "Ke8", "Kc7", "Ka8", "Ka8", "Kb5", "Kd6", 
            "Kb7", "Ka5", "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Ka6", "Kb8", "Kd7", "Kb6", "Ka8", "Kc7", "Ka6", "Kc5", "Ka4", "Kc3", "Ka2", 
            "Kc1", "Ke2", "Kg3", "Kh5", "Kg7", "Ke8", "Kc7", "Ka8", "Ka8", "Kb5", "Kd4", "Kb3", "Ka1", "Kc2", "Ka3", "Kc4", "Kc5", "Kc6", "Ka7", 
            "Kc8", "Ka7", "Kc6", "Ka5", "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Kd5", "Kb6", "Ka8", "Kc7", "Ka6", "Kc5", "Ka4", "Kc3", "Ka2", 
            "Kc1", "Ke2", "Kg1", "Kh3", "Kh5", "Ke6", "Kc5", "Ka4", "Kc3", "Ka2", "Kc1", "Ke2", "Kg3", "Kh5", "Kg7", "Ke8", "Kc7", "Ka8", "Ka8", 
            "Kb5", "Kd6", "Kb7", "Ka5", "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Ka6", "Kb8", "Kd7", "Kb6", "Ka8", "Kc7", "Ka6", "Kc5", "Ka4", 
            "Kc3", "Ka2", "Kc1", "Ke2", "Kg3", "Kh5", "Kg7", "Ke8", "Kc7", "Ka8", "Ka8", "Kb5", "Kd4", "Kb3", "Ka1", "Kc2", "Ka3", "Kc4", "Kc5", 
            "Kc6", "Ka7", "Ka7", "Kc6", "Ka5", "Kc4", "Ka3", "Kc2", "Ka1", "Kc2", "Kb4", "Kd5", "Kb6", "Ka8"
        };
    } 
    else if (opcion == 2) {
        string pos_inicial;
        int n;
        cout << "Posición inicial del caballo (ej. a1): ";
        cin >> pos_inicial;
        cout << "Cantidad de movimientos (N): ";
        cin >> n;

        for (int i = 0; i < n; i++) {
            string mov;
            cout << "Movimiento " << (i + 1) << " (ej. Kc2): ";
            cin >> mov;
            lista_movimientos.push_back(mov);
        }
        // Insertar posición inicial al principio
        lista_movimientos.insert(lista_movimientos.begin(), "K" + pos_inicial);
    } 
    else {
        cout << "Opción inválida.\n";
        return 0;
    }

    if (!lista_movimientos.empty()) {
        // Medir tiempo de ejecución (10 veces exactas para la opción 1)
        auto inicio = chrono::high_resolution_clock::now();

        bool exito = true;
        if (opcion == 1) {
            for (int t = 0; t < 10; t++) {
                if (!procesar_movimientos(lista_movimientos)) {
                    exito = false; // Registra que hubo error, pero ejecuta las 10 veces
                }
            }
        } else {
            exito = procesar_movimientos(lista_movimientos);
        }

        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duracion = fin - inicio;

        cout << "\nTiempo de ejecución: " << duracion.count() << " ms\n";

        // Guardar reporte final en el archivo 'result.txt'
        ofstream archivo("result.txt");
        if (archivo.is_open()) {
            archivo << "=== REPORTE DE PRUEBA DIAGNOSTICA (C++) ===\n";
            archivo << " Ejercicio 1: Movimiento del Caballo de Ajedrez\n";
            if (exito) {
                archivo << " Estado: Movimiento CORRECTO\n";
            } else {
                archivo << " Estado: Movimiento INCORRECTO (Fallo detectado durante las pruebas)\n";
            }
            archivo << " Tiempo de ejecucion (10 iteraciones): " << duracion.count() << " ms\n";
            archivo.close();
            cout << "\n[✔] Todo el resultado anterior fue guardado con éxito en el archivo 'result.txt'.\n";
        } else {
            cout << "\n[✖] Error al intentar crear o abrir el archivo 'result.txt'.\n";
        }
    }

    return 0;
}
