#ifndef RENDER_H
#define RENDER_H

#include "juego.h"

//render_juego()
//Dibujar frames, limpiar consola, imprimir HUD e imprimir mapa
//Ahora ya no solo existe una pantalla de juego, sino un menu y una pantalla de pausa
void render_menu(const Juego *j);
void render_pausa(const Juego *j);
void render_confirmar_menu(const Juego *j);
void render_instrucciones(const Juego *j);
void render_juego(const Juego *j);
void render_victoria(const Juego *j);
void juego_agregar_trofeo_aleatorio(Juego *j);
void render_derrota(const Juego *j);

#endif // RENDER_H
