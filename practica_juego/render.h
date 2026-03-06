#ifndef RENDER_H
#define RENDER_H

#include "juego.h"

//render_juego()
//Dibujar frames, limpiar consola, imprimir HUD e imprimir mapa
//Ahora ya no solo existe una pantalla de juego, sino un menu y una pantalla de pausa
void render_menu(void);
void render_pausa(void);
void render_juego(const Juego *j);

#endif // RENDER_H