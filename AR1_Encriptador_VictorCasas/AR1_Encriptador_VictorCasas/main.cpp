#include <iostream>
#include <string>
#include <fstream>
#include "enigma.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Función para mostrar el menú
void mostrarMenu() {                                            //Creo función de mostrar el menú

    std::cout << "ENIGMA" << std::endl;                         //Pongo nombre del aplicativo
    std::cout << "-------------------------" << std::endl;      //Línea separadora
    std::cout << "1. Cifrar mensaje" << std::endl;              //Opción 1 para cifrar
    std::cout << "2. Descifrar mensaje" << std::endl;           //Opción 2 para descifrar
    std::cout << "3. Editar rotors" << std::endl;               //Opción 3 para editar rotors
    std::cout << "4. Salir" << std::endl;                       //Opción 4 para salir del programa
    std::cout << "Opcion: ";                                    //Pido al usuario su opción

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Función para guardar texto en archivo, con opción de poner espacios cada 5 letras
void guardarTexto(const std::string& nombreArchivo, const std::string& texto, bool conEspacios = false) {

    std::ofstream archivo(nombreArchivo);                       //Abro el archivo en modo escritura

    if (!archivo.is_open()) {                                   //Si no se pudo abrir
        std::cerr << "[ERROR] No se pudo abrir archivo para guardar" << std::endl; //Muestro error
        return;                                                 //Salgo de la función
    }

    int contador = 0;                                           //Contador de letras

    for (char c : texto) {                                      //Recorro cada letra del texto
        archivo << c;                                           //Escribo letra en archivo
        if (conEspacios) {                                      //Si se pide con espacios
            contador++;                                         //Aumento el contador
            if (contador % 5 == 0)                              //Cada 5 letras
                archivo << ' ';                                 //Agrego un espacio
        }
    }

    archivo.close();                                            //Cierro el archivo
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Función para verificar si una cadena es una permutación válida de 26 letras únicas
bool esPermutacionValida(const std::string& permutacion) {

    if (permutacion.size() != 26) {
        return false;                 //Debe tener 26 letras
    }
    bool letras[26] = { false };                                //Vector para verificar letras repetidas

    for (char c : permutacion) {                                //Recorro cada letra
        if (c < 'A' || c > 'Z') return false;                   //Debe ser mayúscula A-Z
        int indice = c - 'A';                                   //Obtengo índice
        if (letras[indice]) return false;                       //Si ya estaba, es repetida
        letras[indice] = true;                                  //Marco letra como usada
    }
    return true;                                                //Es una permutación válida
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Función para editar un rotor con nueva permutación y letra notch
bool editarRotor(int numeroRotor) {

    if (numeroRotor < 1 || numeroRotor > 3) {                   //Si el numero es inválido
        std::cout << "[ERROR] Numero de rotor invalido" << std::endl; //Mensaje de error
        return false;                                           //Salgo con error
    }

    std::string nuevaPermutacion;                               //Variable para nueva permutación
    char nuevoNotch = 'Z';                                      //Valor por defecto del notch

    std::cout << "Introduce la nueva permutacion (26 letras A-Z sin repetir): ";
    std::cin >> nuevaPermutacion;                               //Leo la permutación

    for (char& c : nuevaPermutacion) {                          //Recorro cada letra
        if (c >= 'a' && c <= 'z') {                             //Si es minúscula
            c = c - 'a' + 'A';                                  //La convierto a mayúscula
        }
    }

    if (!esPermutacionValida(nuevaPermutacion)) {              //Si no es válida
        std::cout << "[ERROR] Permutacion incorrecta — se requieren 26 letras unicas A-Z" << std::endl;
        return false;                                           //Salgo con error
    }

    std::cout << "Introduce la letra notch (A-Z) o ENTER para 'Z': ";
    std::string lineaNotch;
    std::cin.ignore();                                          //Limpio buffer
    std::getline(std::cin, lineaNotch);                         //Leo la línea completa

    if (lineaNotch.size() == 1) {                               //Si escribieron una letra...
        char c = lineaNotch[0];
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';            //Paso a mayúscula si hace falta
        if (c >= 'A' && c <= 'Z') {                             //Si es válida
            nuevoNotch = c;                                     //Asigno notch
        }
        else {
            std::cout << "[ERROR] Letra notch invalida, se usara 'Z'" << std::endl;
        }
    }
    else if (lineaNotch.size() > 1) {                           //Si escribo más de una letra...
        std::cout << "[ERROR] Entrada notch invalida, se usara 'Z'" << std::endl;
    }

    std::string nombreArchivo = "Rotor" + std::to_string(numeroRotor) + ".txt"; //Nombre de archivo
    std::ofstream archivo(nombreArchivo);                       //Abro archivo
    if (!archivo.is_open()) {                                   //Si falla
        std::cout << "[ERROR] No se pudo abrir el archivo para guardar" << std::endl;
        return false;                                           //Salta error
    }

    archivo << nuevaPermutacion << "\n" << nuevoNotch << "\n"; //Guardo datos
    archivo.close();                                            //Cierro archivo

    return true;                                                //Todo okey

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    Enigma enigma;                                             //Creo objeto Enigma

    if (!enigma.cargarRotors()) {                              //Intento cargar los rotors desde archivo
        std::cout << "[ERROR] No se pudieron cargar los rotors" << std::endl;  //Si falla... mensaje
        return 1;                                              //Cierro programa
    }

    int opcion = 0;                                            //Variable para la opción del menú
    
    do {

        mostrarMenu();                                        //Muestro el menú
        std::cin >> opcion;                                   //Leo opción del usuario
        if (std::cin.fail()) {                                //Si falla la lectura...
            std::cin.clear();                                 //Limpio error
            std::cin.ignore(10000, '\n');                     //Ignoro entrada mala
            opcion = 0;                                       //Pongo opcion inválida para repetir
        }

        if (opcion == 1 || opcion == 2) {                     //Si la opción es cifrar o descifrar

            char r1, r2, r3;                                  //Variables para las letras de ventana
            std::cout << "Introduzca las tres letras de ventana (A-Z) separadas por espacio: ";
            std::cin >> r1 >> r2 >> r3;                      //Leo las tres letras

            if (r1 < 'A' || r1 > 'Z' || r2 < 'A' || r2 > 'Z' || r3 < 'A' || r3 > 'Z') {  //Si alguna es inválida...
                std::cout << "[ERROR] Letras invalidas" << std::endl;                 //Mensaje de error
                continue;                                         //Vuelvo a mostrar menú
            }

            enigma.configurarPosicionesIniciales(r1, r2, r3);   //Configuro las posiciones iniciales

            std::cout << "Introduzca el mensaje (solo letras mayusculas A-Z): ";
            std::cin.ignore();                                 //Limpio buffer antes del getline
            std::string mensaje;                               //User pone el input
            std::getline(std::cin, mensaje);                   //Leo el mensaje completo

            std::string textoProcesado = enigma.preprocesarTexto(mensaje);  //Limpio y preparo texto
            std::string resultado;                              //Variable para resultado final

            if (opcion == 1) {                                      //Si es cifrar...
                resultado = enigma.cifrarMensaje(textoProcesado);   //Cifro el texto
                guardarTexto("Xifrat.txt", resultado, true);        //Guardo con espacios cada 5 letras
                int grupos = (int)(resultado.size() / 5);           //Calculo cantidad de grupos de 5
                if (resultado.size() % 5 != 0) grupos++;            //Ajusto si sobran letras
                std::cout << "[OK] Mensaje cifrado guardado en Xifrat.txt (" << resultado.size() << " letras, " << grupos << " grupos de 5)" << std::endl;
            }
            else {                                                    //Si es descifrar...
                resultado = enigma.descifrarMensaje(textoProcesado);  //Descifro el texto
                guardarTexto("Desxifrat.txt", resultado, false);      //Guardo sin espacios
                std::cout << "[OK] Mensaje descifrado guardado en Desxifrat.txt" << std::endl;
            }
        }
        else if (opcion == 3) {                                //Si quiere editar un rotor...
            int numeroRotor;
            std::cout << "Que rotor desea editar? (1, 2 o 3): ";
            std::cin >> numeroRotor;                           //Leo numero de rotor
            if (!editarRotor(numeroRotor)) {                   //Intento editar
                std::cout << "[ERROR] No se pudo editar el rotor" << std::endl;  //Si falla... mensaje
            }
            else {
                std::cout << "[OK] Rotor editado correctamente" << std::endl;   //Si bien... mensaje OK
            }
        }
        else if (opcion == 4) {                                //Si quiere salir
            std::cout << "Adios!" << std::endl;                //Mensaje de despedida
        }
        else {                                                 //Si escribe opcion invalida
            std::cout << "[ERROR] Opcion no valida" << std::endl; //Mensaje de error
        }
    } while (opcion != 4);                                     //Repito hasta que elija salir

    return 0;                                                  //Fin del programa
}