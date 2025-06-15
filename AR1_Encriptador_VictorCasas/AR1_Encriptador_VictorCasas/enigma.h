#pragma once

#ifndef ENIGMA_H
#define ENIGMA_H

#include "rotor.h"
#include <string>

class Enigma {
private:
    Rotor rotor1;
    Rotor rotor2;
    Rotor rotor3;

public:
    bool cargarRotors();

    void configurarPosicionesIniciales(char r1, char r2, char r3);

    std::string preprocesarTexto(const std::string& texto);
    std::string cifrarMensaje(const std::string& texto);
    std::string descifrarMensaje(const std::string& texto);

    bool editarRotor(int numero);
};

#endif
