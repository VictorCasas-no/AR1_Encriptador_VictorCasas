#include <fstream>
#include <iostream>
#include <algorithm>
#include "rotor.h"

Rotor::Rotor() : cableado("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), posicionNotch('Z'), posicionActual(0) {}

bool Rotor::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "[ERROR] No se puede abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }

    std::getline(archivo, cableado);
    if (cableado.length() != 26 || !std::all_of(cableado.begin(), cableado.end(), [](char c) { return c >= 'A' && c <= 'Z'; })) {
        std::cout << "[ERROR] Permutacion invalida en " << nombreArchivo << std::endl;
        return false;
    }

    std::string lineaNotch;
    if (std::getline(archivo, lineaNotch)) {
        posicionNotch = lineaNotch[0];
    }
    else {
        posicionNotch = 'Z';
    }

    cableadoInverso.resize(26);
    for (int i = 0; i < 26; ++i) {
        cableadoInverso[cableado[i] - 'A'] = 'A' + i;
    }

    archivo.close();
    return true;
}

bool Rotor::guardarEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "[ERROR] No se puede guardar el archivo " << nombreArchivo << std::endl;
        return false;
    }

    archivo << cableado << std::endl;
    archivo << posicionNotch << std::endl;
    archivo.close();
    return true;
}

bool Rotor::editar(const std::string& nuevaPermutacion, char nuevoNotch) {
    if (nuevaPermutacion.size() != 26) {
        return false;
    }

    std::string copia = nuevaPermutacion;
    std::sort(copia.begin(), copia.end());
    if (copia != "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
        std::cout << "[ERROR] Permutacion invalida." << std::endl;
        return false;
    }

    cableado = nuevaPermutacion;
    posicionNotch = nuevoNotch;

    cableadoInverso.resize(26);
    for (int i = 0; i < 26; ++i) {
        cableadoInverso[cableado[i] - 'A'] = 'A' + i;
    }

    return true;
}

void Rotor::establecerPosicion(char letraInicial) {
    posicionActual = letraInicial - 'A';
}

void Rotor::avanzar() {
    posicionActual = (posicionActual + 1) % 26;
}

bool Rotor::estaEnNotch() const {
    return (cableado[posicionActual] == posicionNotch);
}

char Rotor::cifrarAdelante(char letra) const {
    int indice = (letra - 'A' + posicionActual) % 26;
    char cifrada = cableado[indice];
    return ((cifrada - 'A' - posicionActual + 26) % 26) + 'A';
}

char Rotor::cifrarAtras(char letra) const {
    int indice = (letra - 'A' + posicionActual) % 26;
    char cifrada = cableadoInverso[indice];
    return ((cifrada - 'A' - posicionActual + 26) % 26) + 'A';
}
