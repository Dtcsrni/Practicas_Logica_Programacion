#include "juego.h"
#include "render.h"
#include "entrada.h"
#include "juego.c" // Incluir la implementación del juego para tener acceso a sus funciones
#include "render.c"
#include "entrada.c"
#include "consola.c"
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

    // Salida directa desde la partida
    if (tecla == 'q' || tecla == 'Q')
    {
        strcpy(j->mensaje, "Saliendo del juego.");
        j->estado = ESTADO_SALIR;
        return;
    }
    if (tecla == 'p' || tecla == 'P')
    {
        strcpy(j->mensaje, "Juego pausado.");
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
    else
    {
        strcpy(j->mensaje, "Comando no valido.");
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
        default:
            break;
        }
    }
    return 0;
}
