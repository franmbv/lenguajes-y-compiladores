#include <iostream>
#include <string>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <limits>
#include <sstream>

bool obtenerCasilla(const std::string& texto, std::size_t inicio, int& columna, int& fila) {
    if (inicio + 1 >= texto.size()) {
        return false;
    }

    const char letra = static_cast<char>(std::tolower(static_cast<unsigned char>(texto[inicio])));
    const char numero = texto[inicio + 1];
    if (letra < 'a' || letra > 'h' || numero < '1' || numero > '8') {
        return false;
    }

    columna = letra - 'a';
    fila = numero - '1';
    return true;
}



int main(int, char* argv[]) {
    std::string inicio;
    int cantidadMovimientos;
    int columna, fila;

    std::cout << "Ingrese la posicion inicial: ";
    if (!(std::cin >> inicio)) {
        std::cerr << "Error: no se pudo leer la posicion inicial.\n";
        return 1;
    }

    if (inicio.size() != 2 || !obtenerCasilla(inicio, 0, columna, fila)) {
        std::cout << "La posicion inicial no es valida.\n";
        return 1;
    }

    std::cout << "Ingrese la cantidad de movimientos: ";
    if (!(std::cin >> cantidadMovimientos) || cantidadMovimientos < 0) {
        std::cerr << "Error: ingrese una cantidad de movimientos valida.\n";
        return 1;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string cadenaMovimientos;
    std::cout << "Ingrese la cadena C con los movimientos separados por espacios: ";
    if (!std::getline(std::cin, cadenaMovimientos)) {
        std::cerr << "Error: no se pudo leer la cadena de movimientos.\n";
        return 1;
    }

    const auto inicioValidacion = std::chrono::steady_clock::now();
    std::istringstream movimientos(cadenaMovimientos);
    bool correcto = true;
    int codigoSalida = 0;
    std::string resultado;

    for (int numeroMovimiento = 1; numeroMovimiento <= cantidadMovimientos; ++numeroMovimiento) {
        std::string movimiento;
        if (!(movimientos >> movimiento)) {
            resultado = "movimiento INCORRECTO: falta el movimiento "
                      + std::to_string(numeroMovimiento) + " en la cadena C.";
            correcto = false;
            break;
        }

        if (movimiento.empty() || movimiento[0] != 'K') {
            const char letra = movimiento.empty() ? '\0' : movimiento[0];
            resultado = "movimiento INCORRECTO falla letra ";
            resultado += letra;
            resultado += " no pertenece a un caballo.";
            correcto = false;
            break;
        }

        int nuevaColumna, nuevaFila;
        if (movimiento.size() != 3 || !obtenerCasilla(movimiento, 1, nuevaColumna, nuevaFila)) {
            resultado = "movimiento INCORRECTO falla en la casilla destino del movimiento "
                      + std::to_string(numeroMovimiento) + ".";
            correcto = false;
            break;
        }

        const int diferenciaColumna = std::abs(nuevaColumna - columna);
        const int diferenciaFila = std::abs(nuevaFila - fila);
        if (!((diferenciaColumna == 1 && diferenciaFila == 2) ||
              (diferenciaColumna == 2 && diferenciaFila == 1))) {
            resultado = "movimiento INCORRECTO falla en el movimiento "
                      + std::to_string(numeroMovimiento) + ".";
            correcto = false;
            break;
        }

        columna = nuevaColumna;
        fila = nuevaFila;
    }

    if (correcto) {
        std::string movimientoExtra;
        if (movimientos >> movimientoExtra) {
            resultado = "Error: la cadena C contiene mas movimientos de los indicados.";
            correcto = false;
            codigoSalida = 1;
        } else {
            resultado = "movimiento CORRECTO.";
        }
    }

    const auto finValidacion = std::chrono::steady_clock::now();
    const double tiempoMilisegundos =
        std::chrono::duration<double, std::milli>(finValidacion - inicioValidacion).count();

    std::cout << resultado << '\n';
    std::cout << std::fixed << std::setprecision(6)
              << "Tiempo de validacion: " << tiempoMilisegundos << " ms\n";

    const std::filesystem::path ruta =
        std::filesystem::absolute(argv[0]).parent_path()
        / "tiempos_ejecucion_cpp.txt";
    std::ofstream archivoTiempos(ruta, std::ios::app);
    
    if (!archivoTiempos.is_open()) {
        std::cerr << "Error: no se pudo abrir tiempos_ejecucion_cpp.txt.\n";
        return 1;
    }

    archivoTiempos << "n=" << cantidadMovimientos
                   << "; tiempo_ms=" << std::fixed << std::setprecision(6)
                   << tiempoMilisegundos << "; resultado=" << resultado << '\n';
    if (!archivoTiempos) {
        std::cerr << "Error: no se pudo guardar el tiempo de ejecucion.\n";
        return 1;
    }
    archivoTiempos.close();
    
    return codigoSalida;
}