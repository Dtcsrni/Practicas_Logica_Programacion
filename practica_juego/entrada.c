#include "entrada.h"
#include <conio.h>
#include <windows.h>

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

char entrada_leer_tecla_con_timeout(int timeout_ms)
{
    const int espera_ms = 10;
    int transcurrido = 0;

    while (transcurrido < timeout_ms)
    {
        if (_kbhit())
        {
            return entrada_leer_tecla();
        }
        Sleep(espera_ms);
        transcurrido += espera_ms;
    }

    return '\0';
}
