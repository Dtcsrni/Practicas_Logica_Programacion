#include "juego.h"
#include "render.h"
#include "entrada.h"
#include "juego.c" // Incluir la implementación del juego para tener acceso a sus funciones
#include "render.c"
#include "entrada.c"
#include "consola.c"

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
    if (tecla == '1')
    {
        // Al elegir jugar reiniciamos la partida y cambiamos el estado a jugando
        juego_reiniciar_partida(j);
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == '2')
    {
        j->estado = ESTADO_SALIR;
    }
}
// Maneja teclas cuando el juego está en pausa
static void manejar_pausa(Juego *j, char tecla)
{
    if (tecla == 'p' || tecla == 'P')
    {
        j->estado = ESTADO_JUGANDO;
    }
    else if (tecla == 'm' || tecla == 'M')
    {
        j->estado = ESTADO_MENU;
    }
}
// Maneja teclas cuando el juego está en operación
static void manejar_jugando(Juego *j, char tecla)
{
    // Salida directa desde la partida
    if (tecla == 'q' || tecla == 'Q')
    {
        j->estado = ESTADO_SALIR;
        return;
    }
    if (tecla == 'p' || tecla == 'P')
    {
        j->estado = ESTADO_PAUSA;
        return;
    }
    // Calculo de movimiento
    int dx = 0;
    int dy = 0;
    if (tecla_a_vector(tecla, &dx, &dy))
    {
        juego_intentar_mover(j, dx, dy);
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
            render_menu();
            manejar_menu(&juego, entrada_leer_tecla());
            break;
        case ESTADO_JUGANDO:
            render_juego(&juego);
            manejar_jugando(&juego, entrada_leer_tecla());
            break;
        case ESTADO_PAUSA:
            render_pausa();
            manejar_pausa(&juego, entrada_leer_tecla());
            break;
        default:
            break;
        }
    }
    return 0;
}