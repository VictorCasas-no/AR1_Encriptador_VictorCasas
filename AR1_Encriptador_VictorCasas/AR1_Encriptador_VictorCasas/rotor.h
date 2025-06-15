#pragma once

#ifndef ROTOR_H
#define ROTOR_H

#include <string>
#include <vector>

class Rotor {
private:
    std::string cableado;
    std::string cableadoInverso;
    char posicionNotch;
    int posicionActual;

public:
    Rotor();

    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    bool guardarEnArchivo(const std::string& nombreArchivo) const;
    bool editar(const std::string& nuevaPermutacion, char nuevoNotch);

    void establecerPosicion(char letraInicial);
    void avanzar();

    bool estaEnNotch() const;

    char cifrarAdelante(char letra) const;
    char cifrarAtras(char letra) const;
};

#endif

