#include "juego.h"
#include "render.h"
#include "entrada.h"
#include "consola.h"
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

// Funcion auxiliar para convertir una tecla a desplazamiento
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
    return 0; // Tecla no reconocida
}

static void manejar_menu(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == '1')
    {
        // Al elegir jugar reiniciamos la partida y cambiamos el estado a jugando
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == '2')
    {
        j->estado = ESTADO_INSTRUCCIONES;
        strcpy(j->mensaje, "Consulta los controles antes de jugar.");
    }
    else if (tecla == '3')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido.");
    }
}
// Maneja teclas cuando el juego está en pausa
static void manejar_pausa(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    if (tecla == 'p' || tecla == 'P')
    {
        strcpy(j->mensaje, "Partida reanudada.");
        j->estado = ESTADO_JUGANDO;
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
// Maneja teclas cuando el juego está en operación
static void manejar_jugando(Juego *j, char tecla) 
{
    registrar_tecla(j, tecla);

    if (tecla == 'p' || tecla == 'P')
    {
        strcpy(j->mensaje, "Partida pausada.");
        j->estado = ESTADO_PAUSA;
        return;
    }

    int dx, dy;
    if (tecla_a_vector(tecla, &dx, &dy))
    {
        int nuevo_x = j->jugador_x + dx;
        int nuevo_y = j->jugador_y + dy;

        // Verificar colisiones con paredes
        if (j->mapa[nuevo_y][nuevo_x] == '#')
        {
            j->choques++;
            strcpy(j->mensaje, "Chocaste contra una pared!");
        }
        else
        {
            j->jugador_x = nuevo_x;
            j->jugador_y = nuevo_y;
            j->pasos++;

            // Verificar si se alcanzó la meta
            if (j->jugador_x == j->meta_x && j->jugador_y == j->meta_y)
            {
                strcpy(j->mensaje, "¡Has alcanzado la meta! Presiona R para reiniciar, M para menu o Q para salir.");
                j->estado = ESTADO_VICTORIA;
            }
            else
            {
                strcpy(j->mensaje, "Te moviste.");
            }
        }
    }
    else
    {
        strcpy(j->mensaje, "Comando no valido. Usa WASD para moverte o P para pausar.");
    }
}
static void manejar_victoria(Juego *j, char tecla)
{
    registrar_tecla(j, tecla);

    // Manejo de victoria
    if (tecla == 'r' || tecla == 'R')
    {
        strcpy(j->mensaje, "Reiniciando partida.");
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
        return;
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
    // Ciclo principal del juego.
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
            manejar_jugando(&juego, entrada_leer_tecla());
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
            render_victoria(&juego); // Reutilizamos el render del juego para mostrar el mensaje de victoria
            manejar_victoria(&juego, entrada_leer_tecla());
            break;
        default:
            break;
        }
    }
    return 0;
}
