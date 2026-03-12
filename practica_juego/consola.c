#include "consola.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void consola_limpiar_simple(void)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        putchar('\n');
    }

    system("cls");
}

void consola_ir_inicio(void)
{
    COORD posicion;

    posicion.X = 0;
    posicion.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion);
}
