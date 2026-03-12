#include "render.h"
#include "consola.h"
#include <stdio.h>

#define nombre_juego "Laberinto del CUH"

static int render_hay_enemigo_en(const Juego *j, int x, int y)
{
    int i;

    for (i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (j->enemigos_activos[i] && j->enemigos_x[i] == x && j->enemigos_y[i] == y)
        {
            return 1;
        }
    }

    return 0;
}

static void render_base_juego(const Juego *j)
{
    int x;
    int y;

    consola_ir_inicio();
    printf("Pasos: %d | Posicion: (%d, %d) | Ultima tecla: %c | Choques: %d | Vidas: %d | Trofeos: %d/%d | Enemigos: %d/%d\n",
           j->pasos,
           j->jugador_x,
           j->jugador_y,
           j->ultima_tecla,
           j->choques,
           j->vidas,
           j->trofeos_capturados,
           TROFEOS_PARA_GANAR,
           j->total_enemigos,
           MAX_ENEMIGOS);
    printf("Estado: %s\n\n", j->mensaje);

    for (y = 0; y < ALTO; y++)
    {
        for (x = 0; x < ANCHO; x++)
        {
            if (x == j->jugador_x && y == j->jugador_y)
            {
                putchar('@');
            }
            else if (render_hay_enemigo_en(j, x, y))
            {
                putchar('E');
            }
            else if (j->trofeo_activo && x == j->meta_x && y == j->meta_y)
            {
                putchar('$');
            }
            else
            {
                putchar(j->mapa[y][x]);
            }
        }
        putchar('\n');
    }
}

void render_menu(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== %s ===\n", nombre_juego);
    printf("=== MENU PRINCIPAL ===\n");
    printf("1. Jugar\n");
    printf("2. Cargar partida\n");
    printf("3. Ver instrucciones\n");
    printf("4. Salir\n");
    printf("\nEstado: %s\n", j->mensaje);
    printf("Seleccione una opcion: ");
}

void render_pausa(const Juego *j)
{
    render_base_juego(j);
    puts("\nPAUSA | P continuar | G guardar | C cargar | M menu");
}

void render_confirmar_menu(const Juego *j)
{
    render_base_juego(j);
    puts("\nCONFIRMAR | S volver al menu | N seguir en pausa");
}

void render_instrucciones(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== INSTRUCCIONES ===\n");
    printf("Objetivo: captura 20 trofeos en el laberinto para ganar.\n");
    printf("Movimiento: W arriba, A izquierda, S abajo, D derecha.\n");
    printf("Inicialmente hay 1 enemigo y todos aparecen en la esquina inferior derecha.\n");
    printf("Los enemigos persiguen al jugador y pueden capturar trofeos.\n");
    printf("Cuando un enemigo captura 2 trofeos, genera otro hasta %d enemigos.\n", MAX_ENEMIGOS);
    printf("Colision con enemigo: -1 vida, -1 trofeo y vuelves al inicio.\n");
    printf("Cada 5 trofeos obtenidos ganas 1 vida extra.\n");
    printf("P: pausar. G: guardar. C: cargar. Q: salir.\n");
    printf("Presiona B para volver al menu principal.\n");
    printf("\nEstado: %s\n", j->mensaje);
}

void render_juego(const Juego *j)
{
    render_base_juego(j);
    puts("\nControles: W/A/S/D mover | P pausa | G guardar | C cargar | Q salir");
}

void render_victoria(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== VICTORIA ===\n");
    printf("Capturaste %d trofeos en %d pasos y %d choques.\n", j->trofeos_capturados, j->pasos, j->choques);
    printf("Presiona M para volver al menu principal, R para reiniciar o Q para salir.\n");
    printf("\nEstado: %s\n", j->mensaje);
}

void render_derrota(const Juego *j)
{
    consola_limpiar_simple();
    printf("=== DERROTA ===\n");
    printf("Te quedaste sin vidas despues de %d pasos y %d choques.\n", j->pasos, j->choques);
    printf("Presiona M para volver al menu principal, R para reiniciar o Q para salir.\n");
    printf("\nEstado: %s\n", j->mensaje);
}
