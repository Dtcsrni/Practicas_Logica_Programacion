#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void juego_generar_mapa_dinamico(Juego *j)
{
    int x, y;
    int cantidad_obstaculos;
    int intentos = 0;

    // 1. Crear mapa base borde con muros, interior vacio
    for (y = 0; y < ALTO; y++)
    {
        for (x = 0; x < ANCHO; x++)
        {
            if (x == 0 || x == ANCHO - 1 || y == 0 || y == ALTO - 1)
            {
                j->mapa[y][x] = '#';
            }
            else
            {
                j->mapa[y][x] = ' ';
            }
        }
        j->mapa[y][ANCHO] = '\0';
    }
    // 2. Corredor seguro cruz central
    for (y = 1; y < ALTO - 1; y++)
    {
        j->mapa[y][ANCHO / 2] = ' ';
    }
    for (x = 1; x < ANCHO - 1; x++)
    {
        j->mapa[ALTO / 2][x] = ' ';
    }
    // 3. Zona segura del jugador
    j->mapa[1][1] = ' ';
    j->mapa[1][2] = ' ';
    j->mapa[2][1] = ' ';
    // 4. Generar lógica de obstáculos aleatorios
    cantidad_obstaculos = OBSTACULOS_MIN + rand() % (OBSTACULOS_MAX - OBSTACULOS_MIN + 1);
    while (cantidad_obstaculos > 0 && intentos < 200)
    {
        int ox = 1 + rand() % (ANCHO - 2);
        int oy = 1 + rand() % (ALTO - 2);
        int forma = rand() % 3; // 0: bloque, 1: horizontal, 2: vertical
                                // No tocar zona segura ni corredor central
        if ((ox <= 2 && oy <= 2) || (ox == ANCHO / 2) || (oy == ALTO / 2))
        {
            intentos++;
            continue;
        }
        if (forma == 0)
        { // bloque simple 1x1
            j->mapa[oy][ox] = '#';
        }
        else if (forma == 1 && ox + 1 < ANCHO - 1)
        { // bloque horizontal 1x2
            // horizontal 1x2
            if (oy != ALTO / 2)
            { // no bloquear corredor horizontal
                j->mapa[oy][ox] = '#';
                j->mapa[oy][ox + 1] = '#';
            }
        }
        else if (forma == 2 && oy + 1 < ALTO - 1)
        { // bloque vertical 2x1
            // vertical 2x1
            if (ox != ANCHO / 2)
            { // no bloquear corredor vertical
                j->mapa[oy][ox] = '#';
                j->mapa[oy + 1][ox] = '#';
            }
        }
        cantidad_obstaculos--;
        intentos++;
    }
    int juego_indice_enemigo_en(const Juego *j, int x, int y)
    {
        int i;
        for (i = 0; i < MAX_ENEMIGOS; i++)
        {
            if (j->enemigos_activos[i] && j->enemigos_x[i] == x && j->enemigos_y[i] == y)
            {
                return i;
            }
        }
        return -1; // No se encontró enemigo en la posición
    }
    void juego_eliminar_enemigo(Juego * j, int indice)
    {
        if (indice < 0 || indice >= MAX_ENEMIGOS)
            return; // Índice inválido o enemigo ya inactivo
        if (!j->enemigos_activos[indice])
            return; // Enemigo ya inactivo
        j->enemigos_activos[indice] = 0;
        j->total_enemigos--;
        j->enemigos_eliminados++;

        snprintf(
            j->mensaje,
            sizeof(j->mensaje),
            "Enemigo eliminado. Total enemigos: %d/%d. Enemigos eliminados: %d.",
            j->total_enemigos, MAX_ENEMIGOS, j->enemigos_eliminados);
    }
    void juego_atacar_cuchillo(Juego * j)
    {
        // Ataca en la dirección actual del jugador
        int atx = j->jugador_x + j->dir_x;
        int aty = j->jugador_y + j->dir_y;
        int indice_enemigo = juego_indice_enemigo_en(j, atx, aty);
        if (indice_enemigo >= 0)
        {
            juego_eliminar_enemigo(j, indice_enemigo);
            strcpy(j->mensaje, "Ataque con cuchillo exitoso. Enemigo eliminado.");
        }
        else
        {
            strcpy(j->mensaje, "Ataque con cuchillo fallido. No hay enemigo en esa dirección.");
        }
    }
    void juego_disparar_pistola(Juego * j)
    {
        int x, y, indice;
        if (j->municion <= 0)
        {
            strcpy(j->mensaje, "No tienes munición para disparar.");
            return;
        }
        // Dispara en la dirección actual del jugador hasta chocar con pared o enemigo
        x = j->jugador_x + j->dir_x;
        y = j->jugador_y + j->dir_y;
        while (!juego_es_pared(j, x, y))
        {
            indice = juego_indice_enemigo_en(j, x, y);
            if (indice >= 0)
            {
                juego_eliminar_enemigo(j, indice);
                strcpy(j->mensaje, "Disparo de pistola exitoso. Enemigo eliminado.");
                j->municion--;
                return;
            }
            x += j->dir_x;
            y += j->dir_y;
        }
        strcopy(j->mensaje, "Disparo de pistola fallido. No hay enemigo en esa dirección.");
        j->municion--;
    }

    static const char *MAPA_FIJO[ALTO] = {
        "####################",
        "# ### #  ### ## ####",
        "#                  #",
        "#     #   # # # #  #",
        "#   # ##### #   #  #",
        "#       #     ###  #",
        "#   ### # ###   #  #",
        "#                  #",
        "#   ##### #####    #",
        "####################"};

    static int juego_abs(int valor)
    {
        return (valor < 0) ? -valor : valor;
    }

    static void juego_reubicar_jugador_inicio(Juego * j)
    {
        j->jugador_x = j->inicio_x;
        j->jugador_y = j->inicio_y;
    }

    static void juego_copiar_mapa(Juego * j)
    {
        int y;

        for (y = 0; y < ALTO; y++)
        {
            strncpy(j->mapa[y], MAPA_FIJO[y], ANCHO);
            j->mapa[y][ANCHO] = '\0';
        }
    }

    static int juego_buscar_generacion_enemigo(const Juego *j, int *x, int *y)
    {
        int fila;
        int columna;

        if (!juego_es_pared(j, ANCHO - 2, ALTO - 2))
        {
            *x = ANCHO - 2;
            *y = ALTO - 2;
            return 1;
        }

        for (fila = ALTO - 2; fila >= 1; fila--)
        {
            for (columna = ANCHO - 2; columna >= 1; columna--)
            {
                if (!juego_es_pared(j, columna, fila))
                {
                    *x = columna;
                    *y = fila;
                    return 1;
                }
            }
        }

        return 0;
    }

    static void juego_aplicar_colision_enemigo(Juego * j)
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

    static int juego_prioridad_movimiento(int diferencia_x, int diferencia_y, int mov_x, int mov_y)
    {
        int dir_x = (diferencia_x > 0) - (diferencia_x < 0);
        int dir_y = (diferencia_y > 0) - (diferencia_y < 0);

        if (juego_abs(diferencia_x) >= juego_abs(diferencia_y))
        {
            if (mov_x == dir_x && mov_y == 0)
            {
                return 0;
            }
            if (mov_x == 0 && mov_y == dir_y)
            {
                return 1;
            }
            if (mov_x == 0 && mov_y == -dir_y)
            {
                return 2;
            }
            if (mov_x == -dir_x && mov_y == 0)
            {
                return 3;
            }
        }
        else
        {
            if (mov_x == 0 && mov_y == dir_y)
            {
                return 0;
            }
            if (mov_x == dir_x && mov_y == 0)
            {
                return 1;
            }
            if (mov_x == -dir_x && mov_y == 0)
            {
                return 2;
            }
            if (mov_x == 0 && mov_y == -dir_y)
            {
                return 3;
            }
        }

        return 4;
    }

    static void juego_calcular_movimiento_enemigo(const Juego *j, int indice, int *nx, int *ny)
    {
        int direcciones[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        int mejor_distancia = 100000;
        int mejor_prioridad = 100000;
        int ex = j->enemigos_x[indice];
        int ey = j->enemigos_y[indice];
        int diferencia_x = j->jugador_x - ex;
        int diferencia_y = j->jugador_y - ey;
        int i;

        *nx = ex;
        *ny = ey;

        for (i = 0; i < 4; i++)
        {
            int candidato_x = ex + direcciones[i][0];
            int candidato_y = ey + direcciones[i][1];
            int distancia;
            int prioridad;

            if (juego_es_pared(j, candidato_x, candidato_y))
            {
                continue;
            }

            distancia = juego_abs(j->jugador_x - candidato_x) + juego_abs(j->jugador_y - candidato_y);
            prioridad = juego_prioridad_movimiento(diferencia_x, diferencia_y, direcciones[i][0], direcciones[i][1]);

            if (distancia < mejor_distancia || (distancia == mejor_distancia && prioridad < mejor_prioridad))
            {
                mejor_distancia = distancia;
                mejor_prioridad = prioridad;
                *nx = candidato_x;
                *ny = candidato_y;
            }
        }
    }

    static void juego_procesar_captura_enemigo(Juego * j, int indice)
    {
        int se_reprodujo = 0;

        j->enemigos_trofeos[indice]++;

        if ((j->enemigos_trofeos[indice] % 2) == 0 && j->total_enemigos < MAX_ENEMIGOS)
        {
            se_reprodujo = juego_agregar_enemigo_aleatorio(j);
        }

        juego_agregar_trofeo_aleatorio(j);

        if (se_reprodujo)
        {
            snprintf(j->mensaje,
                     sizeof(j->mensaje),
                     "Un enemigo capturo 2 trofeos y genero otro. Enemigos: %d/%d.",
                     j->total_enemigos,
                     MAX_ENEMIGOS);
        }
        else
        {
            snprintf(j->mensaje,
                     sizeof(j->mensaje),
                     "Un enemigo capturo un trofeo. Enemigos: %d/%d.",
                     j->total_enemigos,
                     MAX_ENEMIGOS);
        }
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

    void juego_reiniciar_partida(Juego * j)
    {
        int x;
        int y;
        int i;

        juego_generar_mapa_dinamico(j);
        j->inicio_x = 1;
        j->inicio_y = 1;
        j->jugador_x = j->inicio_x;
        j->jugador_y = j->inicio_y;
        j->dir_x = 1;
        j->dir_y = 0;
        j->arma_actual = ARMA_CUCHILLO;
        j->municion = MUNICION_INICIAL;
        j->enemigos_eliminados = 0;

        if (juego_es_pared(j, j->inicio_x, j->inicio_y))
        {
            for (y = 1; y < ALTO - 1; y++)
            {
                for (x = 1; x < ANCHO - 1; x++)
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

        for (i = 0; i < MAX_ENEMIGOS; i++)
        {
            j->enemigos_x[i] = 0;
            j->enemigos_y[i] = 0;
            j->enemigos_activos[i] = 0;
            j->enemigos_trofeos[i] = 0;
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

        for (i = 0; i < ENEMIGOS_INICIALES; i++)
        {
            juego_agregar_enemigo_aleatorio(j);
        }

        juego_agregar_trofeo_aleatorio(j);
        strcpy(j->mensaje, "Partida iniciada. Captura 20 trofeos para ganar.");
    }

    void juego_inicializar(Juego * j)
    {
        srand((unsigned int)time(NULL));
        juego_reiniciar_partida(j);
        j->estado = ESTADO_MENU;
        j->partida_activa = 0;
        strcpy(j->mensaje, "Bienvenido al laberinto.");
    }

    void juego_intentar_mover(Juego * j, int dx, int dy)
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

    void juego_agregar_trofeo(Juego * j, int x, int y)
    {
        if (juego_es_pared(j, x, y) || juego_posicion_ocupada(j, x, y))
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

    void juego_agregar_trofeo_aleatorio(Juego * j)
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
        } while (intentos < max_intentos &&
                 (juego_es_pared(j, x, y) || juego_hay_enemigo_en(j, x, y) ||
                  (x == j->jugador_x && y == j->jugador_y)));

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

    int juego_agregar_enemigo_aleatorio(Juego * j)
    {
        int indice = -1;
        int x;
        int y;
        int i;

        for (i = 0; i < MAX_ENEMIGOS; i++)
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

        if (!juego_buscar_generacion_enemigo(j, &x, &y))
        {
            strcpy(j->mensaje, "No se pudo colocar un enemigo.");
            return 0;
        }

        j->enemigos_x[indice] = x;
        j->enemigos_y[indice] = y;
        j->enemigos_activos[indice] = 1;
        j->enemigos_trofeos[indice] = 0;
        j->total_enemigos++;
        return 1;
    }

    void juego_mover_enemigo(Juego * j)
    {
        int e;

        if (j->estado != ESTADO_JUGANDO)
        {
            return;
        }

        for (e = 0; e < MAX_ENEMIGOS; e++)
        {
            int nx;
            int ny;

            if (!j->enemigos_activos[e])
            {
                continue;
            }

            juego_calcular_movimiento_enemigo(j, e, &nx, &ny);
            j->enemigos_x[e] = nx;
            j->enemigos_y[e] = ny;

            if (j->trofeo_activo && nx == j->meta_x && ny == j->meta_y)
            {
                juego_procesar_captura_enemigo(j, e);
            }

            if (j->enemigos_x[e] == j->jugador_x && j->enemigos_y[e] == j->jugador_y)
            {
                juego_aplicar_colision_enemigo(j);
                return;
            }
        }
    }
