#include "juego.h"
#include "render.h"
#include "entrada.h"
#include "persistencia.h"
#include <string.h>

static void registrar_tecla(Juego *j, char tecla)
{
    if (tecla >= 32 && tecla <= 126)
    {
        j->ultima_tecla = tecla;
    }
    else
    {
        j->ultima_tecla = '?';
    }
}

static int tecla_a_vector(char tecla, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;

    if (tecla == 'w' || tecla == 'W')
    {
        *dy = -1;
        return 1;
    }
    else if (tecla == 's' || tecla == 'S')
    {
        *dy = 1;
        return 1;
    }
    else if (tecla == 'a' || tecla == 'A')
    {
        *dx = -1;
        return 1;
    }
    else if (tecla == 'd' || tecla == 'D')
    {
        *dx = 1;
        return 1;
    }

    return 0;
}

static void guardar_partida_actual(Juego *j)
{
    if (juego_guardar_partida(j, ARCHIVO_PARTIDA))
    {
        strcpy(j->mensaje, "Partida guardada exitosamente.");
    }
    else
    {
        strcpy(j->mensaje, "Error al guardar la partida.");
    }
}

static int cargar_partida_actual(Juego *j)
{
    if (juego_cargar_partida(j, ARCHIVO_PARTIDA))
    {
        return 1;
    }

    strcpy(j->mensaje, "Error al cargar la partida.");
    return 0;
}

static void manejar_menu(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == '1')
    {
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == '2')
    {
        if (!cargar_partida_actual(j))
        {
            j->estado = ESTADO_MENU;
        }
    }
    else if (tecla == '3')
    {
        j->estado = ESTADO_INSTRUCCIONES;
        strcpy(j->mensaje, "Consulta los controles antes de jugar.");
    }
    else if (tecla == '4')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido.");
    }
}

static void manejar_pausa(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 'p' || tecla == 'P')
    {
        strcpy(j->mensaje, "Partida reanudada.");
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == 'g' || tecla == 'G')
    {
        guardar_partida_actual(j);
    }
    else if (tecla == 'c' || tecla == 'C')
    {
        (void)cargar_partida_actual(j);
    }
    else if (tecla == 'm' || tecla == 'M')
    {
        strcpy(j->mensaje, "Confirma si deseas volver al menu principal.");
        j->estado = ESTADO_CONFIRMAR_MENU;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido.");
    }
}

static void manejar_confirmar_menu(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 's' || tecla == 'S')
    {
        j->partida_activa = 0;
        strcpy(j->mensaje, "Regresaste al menu principal.");
        j->estado = ESTADO_MENU;
    }
    else if (tecla == 'n' || tecla == 'N')
    {
        strcpy(j->mensaje, "Se conservo el avance actual.");
        j->estado = ESTADO_PAUSA;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido.");
    }
}

static void manejar_instrucciones(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 'b' || tecla == 'B')
    {
        strcpy(j->mensaje, "Volviste al menu principal.");
        j->estado = ESTADO_MENU;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido. Presiona B para volver.");
    }
}

static void manejar_jugando(Juego *j, char tecla)
{
    int dx;
    int dy;

    if (tecla != '\0')
    {
        registrar_tecla(j, tecla);
    }

    if (tecla == 'p' || tecla == 'P')
    {
        strcpy(j->mensaje, "Partida pausada.");
        j->estado = ESTADO_PAUSA;
        return;
    }
    if (tecla == 'g' || tecla == 'G')
    {
        guardar_partida_actual(j);
        return;
    }
    if (tecla == 'c' || tecla == 'C')
    {
        (void)cargar_partida_actual(j);
        return;
    }
    if (tecla == 'q' || tecla == 'Q')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
        return;
    }

    if (tecla_a_vector(tecla, &dx, &dy))
    {
        juego_intentar_mover(j, dx, dy);
    }
    else if (tecla != '\0')
    {
        strcpy(j->mensaje, "Comando no valido. Usa WASD, P, G, C o Q.");
    }

    if (j->estado == ESTADO_JUGANDO)
    {
        juego_mover_enemigo(j);
    }
}

static void manejar_victoria(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 'r' || tecla == 'R')
    {
        strcpy(j->mensaje, "Reiniciando partida.");
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == 'm' || tecla == 'M')
    {
        j->partida_activa = 0;
        strcpy(j->mensaje, "Regresaste al menu principal.");
        j->estado = ESTADO_MENU;
    }
    else if (tecla == 'q' || tecla == 'Q')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido. Presiona R para reiniciar, M para menu o Q para salir.");
    }
}

static void manejar_derrota(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 'r' || tecla == 'R')
    {
        strcpy(j->mensaje, "Reiniciando partida.");
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == 'm' || tecla == 'M')
    {
        j->partida_activa = 0;
        strcpy(j->mensaje, "Regresaste al menu principal.");
        j->estado = ESTADO_MENU;
    }
    else if (tecla == 'q' || tecla == 'Q')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido. Presiona R para reiniciar, M para menu o Q para salir.");
    }
}

int main(void)
{
    Juego juego;

    juego_inicializar(&juego);

    while (juego.estado != ESTADO_SALIR)
    {
        switch (juego.estado)
        {
        case ESTADO_MENU:
            render_menu(&juego);
            manejar_menu(&juego, entrada_leer_tecla());
            break;
        case ESTADO_JUGANDO:
            render_juego(&juego);
            manejar_jugando(&juego, entrada_leer_tecla_con_timeout(250));
            break;
        case ESTADO_PAUSA:
            render_pausa(&juego);
            manejar_pausa(&juego, entrada_leer_tecla());
            break;
        case ESTADO_CONFIRMAR_MENU:
            render_confirmar_menu(&juego);
            manejar_confirmar_menu(&juego, entrada_leer_tecla());
            break;
        case ESTADO_INSTRUCCIONES:
            render_instrucciones(&juego);
            manejar_instrucciones(&juego, entrada_leer_tecla());
            break;
        case ESTADO_VICTORIA:
            render_victoria(&juego);
            manejar_victoria(&juego, entrada_leer_tecla());
            break;
        case ESTADO_DERROTA:
            render_derrota(&juego);
            manejar_derrota(&juego, entrada_leer_tecla());
            break;
        default:
            break;
        }
    }

    return 0;
}
