#ifndef MISIONEROS_MISIONEROS_HPP
#define MISIONEROS_MISIONEROS_HPP

#include "misioneros/llist.hpp"

/**
 * Laboratorio 6 - Misioneros y canibales (parte practica del Control 2)
 *
 * Tres misioneros y tres canibales estan en la orilla izquierda de un rio,
 * junto a un bote en el que caben a lo mas dos personas. El bote no cruza
 * vacio. Si en una orilla hay misioneros, en esa orilla nunca puede haber mas
 * canibales que misioneros; la regla se revisa despues de cada cruce, contando
 * a los que acaban de bajar del bote.
 *
 * Un estado es (m, c, b):
 *   m   misioneros en la orilla IZQUIERDA
 *   c   canibales en la orilla IZQUIERDA
 *   b   1 si el bote esta en la orilla izquierda, 0 si esta en la derecha
 */

const int MISIONEROS = 3;
const int CANIBALES = 3;
const int CAPACIDAD_BOTE = 2;
const int MAX_SUCESORES = 5;

// El id del estado (m, c, b).
// Esta definido para todo m en [0, MISIONEROS], c en [0, CANIBALES] y b en
// {0, 1}, aunque el estado no respete la regla. Dos estados distintos tienen
// ids distintos, y todo id esta en el rango [0, cantidadIds()).
int idEstado(int m, int c, int b);

// La inversa de idEstado: escribe en m, c y b el estado que representa id.
void estadoDe(int id, int &m, int &c, int &b);

// Cuantos ids distintos puede entregar idEstado.
int cantidadIds();

// true si dejar m misioneros y c canibales en la orilla izquierda (y el resto
// en la derecha) respeta la regla en las dos orillas. false si m esta fuera de
// [0, MISIONEROS] o c esta fuera de [0, CANIBALES].
bool valido(int m, int c);

// true si id es el estado meta (0, 0, 0): los seis y el bote en la orilla
// derecha.
bool esMeta(int id);

// Escribe en suc los ids de todos los estados que se alcanzan desde id con un
// solo cruce valido, y retorna cuantos escribio. suc tiene espacio para
// MAX_SUCESORES enteros.
int sucesores(int id, int* suc);

// Busca una secuencia de cruces validos desde el estado idInicio hasta un
// estado en que esMeta es true. Retorna una lista nueva con los ids de los
// estados en orden, desde idInicio hasta la meta (ambos incluidos), o nullptr
// si no existe. Quien llama queda a cargo de liberar la lista.
LList* resolver(int idInicio);

#endif
