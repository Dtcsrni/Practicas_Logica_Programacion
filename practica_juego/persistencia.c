#include "persistencia.h"
#include <stdio.h>
#include <string.h>

static int posicion_valida(int x, int y)
{
    return x >= 0 && x < ANCHO && y >= 0 && y < ALTO;
}

int juego_guardar_partida(const Juego *j, const char *ruta)
{
    FILE *archivo = fopen(ruta, "w");
    if (archivo == NULL)
    {
        return 0;
    }

    fprintf(archivo, "LABERINTO_CUH_V1\n");
    fprintf(archivo, "%d %d %d %d\n", j->jugador_x, j->jugador_y, j->inicio_x, j->inicio_y);
    fprintf(archivo, "%d %d %d %d %d\n", j->vidas, j->pasos, j->choques, j->partida_activa, (int)j->ultima_tecla);
    fprintf(archivo, "%d %d %d %d %d %d %d\n",
            j->meta_x,
            j->meta_y,
            j->trofeo_activo,
            j->trofeos_capturados,
            j->proxima_meta_vida,
            (int)j->estado,
            j->total_enemigos);

    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        fprintf(archivo, "%d %d %d\n", j->enemigos_x[i], j->enemigos_y[i], j->enemigos_activos[i]);
    }

    fclose(archivo);
    return 1;
}

int juego_cargar_partida(Juego *j, const char *ruta)
{
    FILE *archivo = fopen(ruta, "r");
    if (archivo == NULL)
    {
        return 0;
    }

    char cabecera[32];
    if (fscanf(archivo, "%31s", cabecera) != 1 || strcmp(cabecera, "LABERINTO_CUH_V1") != 0)
    {
        fclose(archivo);
        return 0;
    }

    Juego temp;
    juego_reiniciar_partida(&temp);

    int ultima_tecla = '-';
    int estado = ESTADO_JUGANDO;

    if (fscanf(archivo, "%d %d %d %d", &temp.jugador_x, &temp.jugador_y, &temp.inicio_x, &temp.inicio_y) != 4 ||
        fscanf(archivo, "%d %d %d %d %d", &temp.vidas, &temp.pasos, &temp.choques, &temp.partida_activa, &ultima_tecla) != 5 ||
        fscanf(archivo, "%d %d %d %d %d %d %d",
               &temp.meta_x,
               &temp.meta_y,
               &temp.trofeo_activo,
               &temp.trofeos_capturados,
               &temp.proxima_meta_vida,
               &estado,
               &temp.total_enemigos) != 7)
    {
        fclose(archivo);
        return 0;
    }

    for (int i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (fscanf(archivo, "%d %d %d", &temp.enemigos_x[i], &temp.enemigos_y[i], &temp.enemigos_activos[i]) != 3)
        {
            fclose(archivo);
            return 0;
        }
    }

    fclose(archivo);

    if (!posicion_valida(temp.jugador_x, temp.jugador_y) ||
        !posicion_valida(temp.inicio_x, temp.inicio_y) ||
        (temp.trofeo_activo && !posicion_valida(temp.meta_x, temp.meta_y)))
    {
        return 0;
    }

    if (temp.vidas < 0 || temp.trofeos_capturados < 0 || temp.proxima_meta_vida < 5)
    {
        return 0;
    }

    temp.ultima_tecla = (char)ultima_tecla;
    if (estado < ESTADO_MENU || estado > ESTADO_DERROTA)
    {
        return 0;
    }
    temp.estado = (EstadoJuego)estado;

    *j = temp;
    return 1;
}
