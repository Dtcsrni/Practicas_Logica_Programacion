/*Protitpo mínimo para desarrollo de videojuego
Objetivos:
-Mostrar un mapa fijo en consola por medio de ASCII
-Dibujar al jugador @ en una posición (x, y)
-Leer comandos WASD para mover al jugador
-Evitar que el jugador atraviese paredes
*/
#include <stdio.h>
#include <string.h>

#define ALTO 10
#define ANCHO 20

/*Mapa fijo: cada cadena representa una fila del nivel
'#' representa una pared
'.' representa el piso
'*' representa la moneda
*/

static const char *MAPA_FIJO[ALTO] = {
    "####################",
    "#.............*....#",
    "#.######.########.#",
    "#.#....#.#......#.#",
    "#.#.##.#.#.####.#.#",
    "#...##...#....#...#",
    "#####.#########.###",
    "#.....#.......#...#",
    "#.#####.#####.###.#",
    "####################"};
static void consola_limpiar_simple(void)
{
    // Metodo simple para imprimir saltos de linea y separar
    // el cuadro (frame) anterior
    for (int i = 0; i < 40; i++)
    {
        printf("\n");
    }
}
static int es_pared(const char mapa[ALTO][ANCHO + 1], int x, int y)
{
    // Si nos salimos del mapa, lo tratamos como pared para simplificar
    if (x < 0 || x >= ANCHO || y < 0 || y >= ALTO)
    {
        return mapa[y][x] == '#'; // Es pared
    }
    else
    {
        return 0; // Es pared
    }
}

static void renderizar(const char mapa[ALTO][ANCHO + 1], int jugador_x, int jugador_y, int pasos)
{
    consola_limpiar_simple();
    // Encabezado del frame
    printf("SESION 1  | Pasos: %d\n\n", pasos);
    for (int y = 0; y < ALTO; y++)
    {
        for (int x = 0; x < ANCHO; x++)
        {
            // Dibujar primero al jugador por encima del mapa
            if (x == jugador_x && y == jugador_y)
                putchar('@');
            else
                putchar(mapa[y][x]);
        }
        putchar('\n');
    }
    puts("\n Controles: W A S D + Enter | Q para salir");
}

int main()
{
    // Copiamos el mapa fijo a una matriz editable
    // Esto prepara el camino para renderizar monedas y otros cambios en el mapa
    int nx = 0; // Nueva posición tentativa del jugador en x
    int ny = 0;
    char mapa[ALTO][ANCHO + 1]; //+1 para el caracter nulo de fin de cadena
    for (int y = 0; y < ALTO; y++)
    {
        strncpy(mapa[y], MAPA_FIJO[y], ANCHO + 1);
    }
    int xJugador = 1; // Posición inicial del jugador
    int yJugador = 1;
    int pasos = 0; // Contador de pasos
    char tecla;
    while (1)
    {           // Bucle principal del juego
        nx = 0; // Nueva posición tentativa del jugador en x
        ny = 0; // Nueva posición tentativa del jugador en y
        // Primer paso: Renderizamos el mapa en pantalla
        renderizar(mapa, xJugador, yJugador, pasos);
        printf(">");
        //%c igora saltos de linea anteriores
        // Segundo paso: Leemos el comando del usuario
        tecla = ' '; // Limpiar la variable antes de leer

        if (scanf("%c", &tecla) != 1)
            break; // Si hay un error de lectura, salimos del bucle
        if (tecla == 'q' || tecla == 'Q')
            break;
        int dx = 0, dy = 0; // Desplazamiento del jugador

        // Interpretamos la tecla como un vector de movimiento
        if (tecla == 'w' || tecla == 'W')
            dy = -1; // Arriba
        else if (tecla == 's' || tecla == 'S')
            dy = 1; // Abajo
        else if (tecla == 'a' || tecla == 'A')
            dx = -1; // Izquierda
        else if (tecla == 'd' || tecla == 'D')
            dx = 1; // Derecha

        nx = xJugador + dx; // Nueva posición tentativa del jugador en x
        ny = yJugador + dy; // Nueva posición tentativa del jugador en y
        // Regla: solo movemos si NO hay una pared en la nueva posición
        if (!es_pared(mapa, nx, ny))
        {
            xJugador = nx;
            yJugador = ny;
            pasos++; // Solo contamos pasos si el jugador se mueve
        }
        else
        {
            puts("¡No puedes atravesar paredes!");
            nx = xJugador; // Mantener la posición actual
            ny = yJugador;
        }
    }
    puts("Fin del prototipo. Gracias por jugar!");
    return 0;
}
