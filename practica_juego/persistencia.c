#include "persistencia.h"
#include <stdio.h>
#include <string.h>

static int posicion_valida(int x, int y)
{
    return x >= 0 && x < ANCHO && y >= 0 && y < ALTO;
}

int juego_guardar_partida(const Juego *j, const char *ruta)
{
    FILE *archivo;
    int i;

    archivo = fopen(ruta, "w");
    if (archivo == NULL)
    {
        return 0;
    }

    fprintf(archivo, "LABERINTO_CUH_V2\n");
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

    for (i = 0; i < MAX_ENEMIGOS; i++)
    {
        fprintf(archivo,
                "%d %d %d %d\n",
                j->enemigos_x[i],
                j->enemigos_y[i],
                j->enemigos_activos[i],
                j->enemigos_trofeos[i]);
    }

    fclose(archivo);
    return 1;
}

int juego_cargar_partida(Juego *j, const char *ruta)
{
    FILE *archivo;
    Juego temp;
    char cabecera[32];
    int ultima_tecla = '-';
    int estado = ESTADO_JUGANDO;
    int enemigos_activos = 0;
    int i;

    archivo = fopen(ruta, "r");
    if (archivo == NULL)
    {
        return 0;
    }

    if (fscanf(archivo, "%31s", cabecera) != 1 || strcmp(cabecera, "LABERINTO_CUH_V2") != 0)
    {
        fclose(archivo);
        return 0;
    }

    juego_reiniciar_partida(&temp);

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

    for (i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (fscanf(archivo,
                   "%d %d %d %d",
                   &temp.enemigos_x[i],
                   &temp.enemigos_y[i],
                   &temp.enemigos_activos[i],
                   &temp.enemigos_trofeos[i]) != 4)
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

    if (estado < ESTADO_MENU || estado > ESTADO_DERROTA)
    {
        return 0;
    }

    for (i = 0; i < MAX_ENEMIGOS; i++)
    {
        if (temp.enemigos_activos[i])
        {
            if (!posicion_valida(temp.enemigos_x[i], temp.enemigos_y[i]) || temp.enemigos_trofeos[i] < 0)
            {
                return 0;
            }
            enemigos_activos++;
        }
    }

    if (enemigos_activos != temp.total_enemigos || temp.total_enemigos < 1 || temp.total_enemigos > MAX_ENEMIGOS)
    {
        return 0;
    }

    temp.ultima_tecla = (char)ultima_tecla;
    temp.estado = ESTADO_PAUSA;
    temp.partida_activa = 1;
    strcpy(temp.mensaje, "Partida cargada. Juego en pausa.");

    *j = temp;
    return 1;
}
