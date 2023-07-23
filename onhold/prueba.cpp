#include <iostream>
#include <fstream>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;

int main() {
    // Variables para almacenar el número y la resta
    std::string numeroStr;
    mp::cpp_int numero, resta;

    // Nombre del archivo de texto
    std::string nombreArchivo = "estado.txt";

    // Objeto de entrada de archivo
    std::ifstream archivo(nombreArchivo);

    // Verificar si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        return 1;
    }

    // Leer el número del archivo y almacenarlo en una variable como cadena
    std::getline(archivo, numeroStr);

    // Cerramos el archivo
    archivo.close();

    // Convertir la cadena a un número de múltiple precisión
    try {
        numero = mp::cpp_int(numeroStr);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Realizar la resta con 400000
    resta = numero - 400000;
    
    // Imprimir los resultados
    std::cout << "Número: " << numero << std::endl;
    std::cout << "Resta con 400000: " << resta << std::endl;

    return 0;
}
