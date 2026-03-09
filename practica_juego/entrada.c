#include "entrada.h"
#include <conio.h>

char entrada_leer_tecla(void)
{
    int tecla = _getch();

    if (tecla == 0 || tecla == 224)
    {
        (void)_getch();
        return '\0';
    }

    return (char)tecla;
}
