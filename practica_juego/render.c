#include "render.h"
#include "consola.h"
#include <stdio.h>
#define nombre_juego "Laberinto del CUH"

void render_menu(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== %s ===\n", nombre_juego);
    printf("=== MENU PRINCIPAL ===\n");
    printf("1. Jugar\n");
    printf("2. Ver instrucciones\n");
    printf("3. Salir\n");
    printf("\nEstado: %s\n", j->mensaje);
    printf("Seleccione una opcion: ");
}

void render_pausa(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== JUEGO PAUSADO ===\n");
    printf("P. Continuar\n");
    printf("M. Volver al menu principal\n");
    printf("\nEstado: %s\n", j->mensaje);
    printf("Seleccione una opción: ");
}

void render_confirmar_menu(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== CONFIRMAR SALIDA AL MENU ===\n");
    printf("Se perdera el avance actual de la partida.\n");
    printf("S. Confirmar y volver al menu\n");
    printf("N. Cancelar y seguir en pausa\n");
    printf("\nEstado: %s\n", j->mensaje);
    printf("Seleccione una opcion: ");
}

void render_instrucciones(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== INSTRUCCIONES ===\n");
    printf("Objetivo: recorre el laberinto usando el teclado.\n");
    printf("Movimiento: W arriba, A izquierda, S abajo, D derecha.\n");
    printf("P: pausar partida.\n");
    printf("Q: salir del juego.\n");
    printf("Si chocas contra una pared, el HUD lo mostrara.\n");
    printf("Presiona B para volver al menu principal.\n");
    printf("\nEstado: %s\n", j->mensaje);
}

void render_juego(const Juego *j)
{
    // Limpiamos la pantalla antes de dibujar
    consola_limpiar_simple();
    printf("Pasos: %d | Posicion: (%d, %d) | Ultima tecla: %c | Choques: %d\n",
           j->pasos,
           j->jugador_x,
           j->jugador_y,
           j->ultima_tecla,
           j->choques);
    printf("Estado: %s\n\n", j->mensaje);

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
    puts("\nControles: W/A/S/D mover, P pausa, Q salir");
}
