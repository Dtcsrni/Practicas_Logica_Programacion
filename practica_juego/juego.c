#include "juego.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const char *MAPA_FIJO[ALTO] = {
    "###################",
    "# ### #  ### ## ###",
    "#                 #",
    "#     #   # # # # #",
    "#   # ##### #   # #",
    "#       #     ### #",
    "#   ### # ###   # #",
    "#                 #",
    "#   ##### #####   #",
    "###################"};

static void juego_reubicar_jugador_inicio(Juego *j)
{
    j->jugador_x = j->inicio_x;
    j->jugador_y = j->inicio_y;
}

static void juego_aplicar_colision_enemigo(Juego *j)
{
    if (j->vidas > 0)
    {
        j->vidas--;
    }

    if (j->trofeos_capturados > 0)
    {
        j->trofeos_capturados--;
    }

    if (j->vidas <= 0)
    {
        j->estado = ESTADO_DERROTA;
        j->partida_activa = 0;
        strcpy(j->mensaje, "Sin vidas. Derrota.");
        return;
    }

    juego_reubicar_jugador_inicio(j);
    snprintf(j->mensaje,
             sizeof(j->mensaje),
             "Colision con enemigo: -1 vida, -1 trofeo. Vidas: %d | Trofeos: %d/%d.",
             j->vidas,
             j->trofeos_capturados,
             TROFEOS_PARA_GANAR);
}

int juego_es_pared(const Juego *j, int x, int y)
{
    if (x < 0 || x >= ANCHO || y < 0 || y >= ALTO)
    {
        return 1;
    }
    return (j->mapa[y][x] == '#');
}

int juego_hay_enemigo_en(const Juego *j, int x, int y)
{
    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (j->enemigos_activos[i] && j->enemigos_x[i] == x && j->enemigos_y[i] == y)
        {
            return 1;
        }
    }
    return 0;
}

void juego_reiniciar_partida(Juego *j)
{
    for (int y = 0; y < ALTO; y++)
    {
        strncpy(j->mapa[y], MAPA_FIJO[y], ANCHO);
        j->mapa[y][ANCHO] = '\0';
    }

    j->inicio_x = 1;
    j->inicio_y = 1;

    if (juego_es_pared(j, j->inicio_x, j->inicio_y))
    {
        for (int y = 1; y < ALTO - 1; y++)
        {
            for (int x = 1; x < ANCHO - 1; x++)
            {
                if (!juego_es_pared(j, x, y))
                {
                    j->inicio_x = x;
                    j->inicio_y = y;
                    y = ALTO;
                    break;
                }
            }
        }
    }

    juego_reubicar_jugador_inicio(j);

    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        j->enemigos_x[i] = 0;
        j->enemigos_y[i] = 0;
        j->enemigos_activos[i] = 0;
    }

    j->total_enemigos = 0;
    j->vidas = 3;
    j->proxima_meta_vida = 5;
    j->pasos = 0;
    j->choques = 0;
    j->partida_activa = 1;
    j->ultima_tecla = '-';
    j->trofeos_capturados = 0;
    j->trofeo_activo = 0;

    juego_agregar_trofeo_aleatorio(j);

    for (int i = 0; i < ENEMIGOS_INICIALES; i++)
    {
        juego_agregar_enemigo_aleatorio(j);
    }

    strcpy(j->mensaje, "Partida iniciada. Captura 20 trofeos para ganar.");
}

void juego_inicializar(Juego *j)
{
    srand((unsigned int)time(NULL));
    juego_reiniciar_partida(j);
    j->estado = ESTADO_MENU;
    j->partida_activa = 0;
    strcpy(j->mensaje, "Bienvenido al laberinto.");
}

void juego_intentar_mover(Juego *j, int dx, int dy)
{
    int nx = j->jugador_x + dx;
    int ny = j->jugador_y + dy;

    if (juego_es_pared(j, nx, ny))
    {
        j->choques++;
        strcpy(j->mensaje, "Has chocado contra una pared.");
        return;
    }

    j->jugador_x = nx;
    j->jugador_y = ny;
    j->pasos++;
    strcpy(j->mensaje, "Movimiento realizado.");

    if (juego_hay_enemigo_en(j, j->jugador_x, j->jugador_y))
    {
        juego_aplicar_colision_enemigo(j);
        return;
    }

    if (j->trofeo_activo && j->jugador_x == j->meta_x && j->jugador_y == j->meta_y)
    {
        int gano_vida = 0;
        j->trofeos_capturados++;

        if (j->trofeos_capturados >= j->proxima_meta_vida)
        {
            j->vidas++;
            j->proxima_meta_vida += 5;
            gano_vida = 1;
        }

        if (j->trofeos_capturados >= TROFEOS_PARA_GANAR)
        {
            j->trofeo_activo = 0;
            j->estado = ESTADO_VICTORIA;
            j->partida_activa = 0;
            strcpy(j->mensaje, "Ganaste. Capturaste los 20 trofeos.");
        }
        else
        {
            juego_agregar_trofeo_aleatorio(j);
            if (gano_vida)
            {
                snprintf(j->mensaje,
                         sizeof(j->mensaje),
                         "Trofeo capturado. +1 vida por 5 trofeos. Vidas: %d | Trofeos: %d/%d.",
                         j->vidas,
                         j->trofeos_capturados,
                         TROFEOS_PARA_GANAR);
            }
            else
            {
                snprintf(j->mensaje,
                         sizeof(j->mensaje),
                         "Trofeo capturado. Llevas %d de %d.",
                         j->trofeos_capturados,
                         TROFEOS_PARA_GANAR);
            }
        }
    }
}

void juego_agregar_trofeo(Juego *j, int x, int y)
{
    if (juego_es_pared(j, x, y) || (x == j->jugador_x && y == j->jugador_y) || juego_hay_enemigo_en(j, x, y))
    {
        j->trofeo_activo = 0;
        strcpy(j->mensaje, "No se pudo colocar el trofeo, posicion invalida.");
        return;
    }

    j->meta_x = x;
    j->meta_y = y;
    j->trofeo_activo = 1;
    strcpy(j->mensaje, "Trofeo colocado en el mapa.");
}

void juego_agregar_trofeo_aleatorio(Juego *j)
{
    int x;
    int y;
    int intentos = 0;
    int max_intentos = 200;

    do
    {
        x = rand() % ANCHO;
        y = rand() % ALTO;
        intentos++;
    } while (intentos < max_intentos && (juego_es_pared(j, x, y) || (x == j->jugador_x && y == j->jugador_y) || juego_hay_enemigo_en(j, x, y)));

    if (intentos >= max_intentos)
    {
        j->trofeo_activo = 0;
        strcpy(j->mensaje, "No se pudo colocar el trofeo aleatorio.");
        return;
    }

    j->meta_x = x;
    j->meta_y = y;
    j->trofeo_activo = 1;
}

int juego_posicion_ocupada(const Juego *j, int x, int y)
{
    if (x == j->jugador_x && y == j->jugador_y)
    {
        return 1;
    }

    if (j->trofeo_activo && x == j->meta_x && y == j->meta_y)
    {
        return 1;
    }

    if (juego_hay_enemigo_en(j, x, y))
    {
        return 1;
    }

    return 0;
}

int juego_agregar_enemigo_aleatorio(Juego *j)
{
    int indice = -1;
    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (!j->enemigos_activos[i])
        {
            indice = i;
            break;
        }
    }

    if (indice < 0)
    {
        return 0;
    }

    int x;
    int y;
    int intentos = 0;
    int max_intentos = 200;

    do
    {
        x = rand() % ANCHO;
        y = rand() % ALTO;
        intentos++;
    } while (intentos < max_intentos && (juego_es_pared(j, x, y) || juego_posicion_ocupada(j, x, y)));

    if (intentos >= max_intentos)
    {
        strcpy(j->mensaje, "No se pudo colocar un enemigo.");
        return 0;
    }

    j->enemigos_x[indice] = x;
    j->enemigos_y[indice] = y;
    j->enemigos_activos[indice] = 1;
    j->total_enemigos++;
    return 1;
}

void juego_mover_enemigo(Juego *j)
{
    if (j->estado != ESTADO_JUGANDO)
    {
        return;
    }

    int direcciones[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (int e = 0; e < MAX_ENEMIGOS; e++)
    {
        if (!j->enemigos_activos[e])
        {
            continue;
        }

        int moved = 0;
        for (int intentos = 0; intentos < 10; intentos++)
        {
            int i = rand() % 4;
            int nx = j->enemigos_x[e] + direcciones[i][0];
            int ny = j->enemigos_y[e] + direcciones[i][1];

            if (juego_es_pared(j, nx, ny))
            {
                continue;
            }

            if (j->trofeo_activo && nx == j->meta_x && ny == j->meta_y)
            {
                continue;
            }

            int ocupado_por_otro = 0;
            for (int o = 0; o < MAX_ENEMIGOS; o++)
            {
                if (o != e && j->enemigos_activos[o] && j->enemigos_x[o] == nx && j->enemigos_y[o] == ny)
                {
                    ocupado_por_otro = 1;
                    break;
                }
            }
            if (ocupado_por_otro)
            {
                continue;
            }

            j->enemigos_x[e] = nx;
            j->enemigos_y[e] = ny;
            moved = 1;
            break;
        }

        (void)moved;

        if (j->enemigos_x[e] == j->jugador_x && j->enemigos_y[e] == j->jugador_y)
        {
            juego_aplicar_colision_enemigo(j);
            return;
        }
    }
}
