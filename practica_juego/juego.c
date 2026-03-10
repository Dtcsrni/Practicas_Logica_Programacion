#include "juego.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Mapa base fijo

// ...existing code...
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
// ...existing code...

int juego_es_pared(const Juego *j, int x, int y)
{
    // Fuera de mapa => pared
    if (x < 0 || x >= ANCHO || y < 0 || y >= ALTO)
        return 1;
    return (j->mapa[y][x] == '#');
}
// Función para reiniciar la partida

void juego_reiniciar_partida(Juego *j)
{
    for (int y = 0; y < ALTO; y++)
    {
        strncpy(j->mapa[y], MAPA_FIJO[y], ANCHO);
        j->mapa[y][ANCHO] = '\0'; // Asegurar terminacion nula
    }

    // Definir posicion inicial del jugador
    j->jugador_x = 1;
    j->jugador_y = 1;

    // Validación de seguridad
    // Si el mapa cambiara y la posición inicial fuera pared, entonces buscamos la primera celda libre
    // en los alrededores y colocamos al jugador ahí
    if (juego_es_pared(j, j->jugador_x, j->jugador_y))
    {
        for (int y = 1; y < ALTO - 1; y++)
        {
            for (int x = 1; x < ANCHO - 1; x++)
            {
                if (!juego_es_pared(j, x, y))
                {
                    j->jugador_x = x;
                    j->jugador_y = y;
                    y = ALTO; // Salir del ciclo externo
                    break;    // Salir del ciclo interno
                }
            }
        }
    }
    j->pasos = 0;
    j->choques = 0;
    j->partida_activa = 1;
    j->ultima_tecla = '-';
    j->trofeos_capturados = 0;
    strcpy(j->mensaje, "Partida iniciada. Captura 10 trofeos para ganar.");

    juego_agregar_trofeo_aleatorio(j);
    juego_agregar_enemigo_aleatorio(j);
}

void juego_inicializar(Juego *j)
{
    srand((unsigned int)time(NULL)); // Inicializar semilla para números aleatorios
    // Al inicializar partida, ejecutar el método para reiniciar y de esa manera podamos adaptar el jugador
    // en caso de que cambie el mapa
    juego_reiniciar_partida(j);

    // El programa ya no inicia directamente jugando si no, en un menú principal
    j->estado = ESTADO_MENU;
    j->partida_activa = 0;
    strcpy(j->mensaje, "Bienvenido al laberinto.");
}
void juego_intentar_mover(Juego *j, int dx, int dy)
{
    // Calculamos posicion destino
    int nx = j->jugador_x + dx;
    int ny = j->jugador_y + dy;
    // Verificamos colision: si es pared, no hacemos nada
    if (juego_es_pared(j, nx, ny))
    {
        j->choques++;
        strcpy(j->mensaje, "Has chocado contra una pared.");
        return;
    }

    // Si no es pared, actualizamos posicion del jugador
    j->jugador_x = nx;
    j->jugador_y = ny;
    j->pasos++;

    strcpy(j->mensaje, "Movimiento realizado.");

    if (j->enemigo_activo && j->jugador_x == j->enemigo_x && j->jugador_y == j->enemigo_y)
    {
        j->estado = ESTADO_DERROTA;
        j->partida_activa = 0;
        strcpy(j->mensaje, "¡Has sido atrapado por el enemigo! Fin del juego.");
        return;
    }

    if (j->trofeo_activo && j->jugador_x == j->meta_x && j->jugador_y == j->meta_y)
    {
        j->trofeos_capturados++;

        if (j->trofeos_capturados >= TROFEOS_PARA_GANAR)
        {
            j->trofeo_activo = 0;
            j->estado = ESTADO_VICTORIA;
            j->partida_activa = 0;
            strcpy(j->mensaje, "¡Ganaste! Capturaste los 10 trofeos.");
        }
        else
        {
            juego_agregar_trofeo_aleatorio(j);
            snprintf(
                j->mensaje,
                sizeof(j->mensaje),
                "Capturaste un trofeo. Llevas %d de %d.",
                j->trofeos_capturados,
                TROFEOS_PARA_GANAR);
        }
    }
}

void juego_agregar_trofeo(Juego *j, int x, int y)
{
    // Validar que no quede fuera del mapa ni dentro de una pared
    if (juego_es_pared(j, x, y))
    {
        j->trofeo_activo = 0;
        strcpy(j->mensaje, "No se pudo colocar el trofeo, posición inválida.");
    }
    else
    {
        // Evitar colocarlo encima del jugador
        if (x == j->jugador_x && y == j->jugador_y)
        {
            j->trofeo_activo = 0;
            strcpy(j->mensaje, "No se pudo colocar el trofeo, posición inválida.");
        }
        else
        {
            j->meta_x = x;
            j->meta_y = y;
            j->trofeo_activo = 1;
            strcpy(j->mensaje, "Trofeo colocado en el mapa.");
        }
    }
}

void juego_agregar_trofeo_aleatorio(Juego *j)
{
    int x;
    int y;
    int intentos = 0;
    int max_intentos = 100;

    do
    {
        x = rand() % ANCHO;
        y = rand() % ALTO;
        intentos++;
    } while (intentos < max_intentos && (juego_es_pared(j, x, y) || (x == j->jugador_x && y == j->jugador_y)));
    if (intentos == max_intentos)
    {
        j->trofeo_activo = 0;
        strcpy(j->mensaje, "No se pudo colocar el trofeo, no se encontró una posición válida.");
        return;
    }

    j->meta_x = x;
    j->meta_y = y;
    j->trofeo_activo = 1;
    strcpy(j->mensaje, "Trofeo colocado aleatoriamente en el mapa.");
}

int juego_posicion_ocupada(const Juego *j, int x, int y)
{
    // Determinar si la posición está ocupada por el jugador, el enemigo o un trofeo
    if (x == j->jugador_x && y == j->jugador_y)
        return 1; // Ocupada por el jugador
    if (j->trofeo_activo && x == j->meta_x && y == j->meta_y)
        return 1; // Ocupada por el trofeo
    if (j->enemigo_activo && x == j->enemigo_x && y == j->enemigo_y)
        return 1; // Ocupada por el enemigo
    return 0;
}

int juego_agregar_enemigo_aleatorio(Juego *j)
{
    int x;
    int y;
    int intentos = 0;
    int max_intentos = 100;
    do
    {
        x = rand() % ANCHO;
        y = rand() % ALTO;
        intentos++;
    } while (intentos < max_intentos && (juego_es_pared(j, x, y) || juego_posicion_ocupada(j, x, y)));
    if (intentos >= max_intentos)
    {
        j->enemigo_activo = 0;
        strcpy(j->mensaje, "No se pudo colocar el enemigo, no se encontró una posición válida.");
        return 0;
    }

    j->enemigo_x = x;
    j->enemigo_y = y;
    j->enemigo_activo = 1;
    strcpy(j->mensaje, "Enemigo colocado aleatoriamente en el mapa.");
    return 1;
}

void juego_mover_enemigo(Juego *j)
{
    if (!j->enemigo_activo || j->estado != ESTADO_JUGANDO)
        return;
    // Movemos el enemigo en una dirección aleatoria (arriba, abajo, izquierda, derecha)
    int direcciones[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int intentos = 0;
    int max_intentos = 10;
    while (intentos < max_intentos)
    {
        int i = rand() % 4;
        int nx = j->enemigo_x + direcciones[i][0];
        int ny = j->enemigo_y + direcciones[i][1];
        if (!juego_es_pared(j, nx, ny) && !(j->trofeo_activo && nx == j->meta_x && ny == j->meta_y) && !(nx == j->jugador_x && ny == j->jugador_y))
        {
            j->enemigo_x = nx;
            j->enemigo_y = ny;
            break;
        }
        intentos++;
    }
    if (j->enemigo_x == j->jugador_x && j->enemigo_y == j->jugador_y)
    {
        j->estado = ESTADO_DERROTA;
        j->partida_activa = 0;
        strcpy(j->mensaje, "¡Has sido atrapado por el enemigo! Fin del juego.");
    }
}