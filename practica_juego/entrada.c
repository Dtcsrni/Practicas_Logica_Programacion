#include "entrada.h"
#include <stdio.h>

char entrada_leer_tecla(void)
{
    char tecla;
    // El espacio inicial hace que scanf ignore saltos de linea previos.
    if (scanf(" %c", &tecla) != 1)
    {
        return 'q';
    }
    return tecla;
}
