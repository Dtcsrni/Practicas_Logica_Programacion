#include "render.h"
#include "consola.h"
#include <stdio.h>
#define nombre_juego "Laberinto del CUH"

void render_menu(void)
{
    consola_limpiar_simple();
    printf("=== %s ===\n", nombre_juego);
    printf("=== MENU PRINCIPAL ===\n");
    printf("1. Jugar\n");
    printf("2. Salir\n");
    printf("Seleccione una opcion: ");
}

void render_pausa(void)
{
    consola_limpiar_simple();
    printf("=== JUEGO PAUSADO ===\n");
    printf("P. Continuar\n");
    printf("M. Volver al menú\n");
    printf("Seleccione una opción: ");
}

void render_juego(const Juego *j)
{
    // Limpiamos la pantalla antes de dibujar
    consola_limpiar_simple();
    // Mostrar el HUD mínimo
    printf("Pasos: %d \n\n", j->pasos);

    // Dibujar el mapa y al jugador
    for (int y = 0; y < ALTO; y++)
    {
        for (int x = 0; x < ANCHO; x++)
        {
            if (x == j->jugador_x && y == j->jugador_y)
            {
                putchar('@'); // Dibujar al jugador
            }
            else
            {
                putchar(j->mapa[y][x]); // Dibujar el mapa
            }
        }
        putchar('\n'); // Nueva línea al final de cada fila
    }
    puts("\n Controles: w/a/s/d + enter para mover, q para salir");
}