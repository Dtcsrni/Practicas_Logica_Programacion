#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H
#include "juego.h"

#define ARCHIVO_PARTIDA "partida_guardada.txt"

int juego_guardar_partida(const Juego *j, const char *nombre_archivo);
int juego_cargar_partida(Juego *j, const char *nombre_archivo);

#endif // PERSISTENCIA_H
