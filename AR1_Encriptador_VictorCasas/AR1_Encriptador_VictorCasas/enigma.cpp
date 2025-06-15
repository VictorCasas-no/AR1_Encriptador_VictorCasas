#include "enigma.h"
#include <fstream>
#include <iostream>

bool Enigma::cargarRotors() {
    if (!rotor1.cargarDesdeArchivo("Rotor1.txt")) return false;
    if (!rotor2.cargarDesdeArchivo("Rotor2.txt")) return false;
    if (!rotor3.cargarDesdeArchivo("Rotor3.txt")) return false;
    return true;
}

void Enigma::configurarPosicionesIniciales(char r1, char r2, char r3) {
    rotor1.establecerPosicion(r1);
    rotor2.establecerPosicion(r2);
    rotor3.establecerPosicion(r3);
}

std::string Enigma::preprocesarTexto(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        if (c >= 'A' && c <= 'Z') {
            resultado += c;
        }
        // Ignorar todo lo demás
    }
    return resultado;
}

std::string Enigma::cifrarMensaje(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        rotor1.avanzar();
        if (rotor1.estaEnNotch())
            rotor2.avanzar();
        if (rotor2.estaEnNotch())
            rotor3.avanzar();

        char paso = rotor1.cifrarAdelante(c);
        paso = rotor2.cifrarAdelante(paso);
        paso = rotor3.cifrarAdelante(paso);

        paso = 'Z' - (paso - 'A');

        paso = rotor3.cifrarAtras(paso);
        paso = rotor2.cifrarAtras(paso);
        paso = rotor1.cifrarAtras(paso);

        resultado += paso;
    }
    return resultado;
}

std::string Enigma::descifrarMensaje(const std::string& texto) {
    // Igual al cifrado
    return cifrarMensaje(texto);
}

bool Enigma::editarRotor(int numero) {
    if (numero < 1 || numero > 3) return false;

    Rotor* rotor = nullptr;
    if (numero == 1) rotor = &rotor1;
    else if (numero == 2) rotor = &rotor2;
    else rotor = &rotor3;

    std::string permutacion;
    char notch;

    std::cout << "Introduzca la nueva permutacion (26 letras A-Z sin repetir): ";
    std::cin >> permutacion;

    std::cout << "Introduzca el nuevo notch (una letra A-Z): ";
    std::cin >> notch;

    if (!rotor->editar(permutacion, notch)) {
        std::cout << "[ERROR] No se pudo editar el rotor\n";
        return false;
    }

    std::string nombreArchivo = "Rotor" + std::to_string(numero) + ".txt";
    if (!rotor->guardarEnArchivo(nombreArchivo)) {
        std::cout << "[ERROR] No se pudo guardar el rotor\n";
        return false;
    }

    return true;
}



