/*
  Prototipo minimo para desarrollo de videojuego - SESION 1

  Objetivos:
  - Mostrar un mapa fijo en consola usando caracteres simples
  - Dibujar al jugador '@' en una posicion (x, y)
  - Leer comandos WASD para mover al jugador
  - Evitar que el jugador atraviese paredes '#'

  Notas didacticas:
  - NO usamos ANSI ni caracteres de control para limpiar pantalla.
  - "Limpiar" se simula imprimiendo muchos saltos de linea.
  - La entrada se lee con scanf(" %c", ...) para ignorar '\n' y espacios previos.
*/

#include <stdio.h>
#include <string.h>

#define ALTO 10
#define ANCHO 20

/*
  Mapa fijo: cada cadena representa una fila del nivel.
  Reglas IMPORTANTES:
  - Cada fila debe tener EXACTAMENTE ANCHO caracteres (20).
  - Usamos '#' como pared y '.' como piso.
  - '*' es moneda (en esta sesion solo se dibuja, aun no da puntos).
*/
static const char *MAPA_FIJO[ALTO] = {
    "####################",  /* 20 */
    "#.............*....#",  /* 20 */
    "#.######.########..#",  /* 20 */
    "#.#....#.#......#..#",  /* 20 */
    "#.#.##.#.#.####.#..#",  /* 20 */
    "#...##...#....#....#",  /* 20 */
    "#####.##########.###",  /* 20 */
    "#.....#.......#....#",  /* 20 */
    "#.#####.#####.###..#",  /* 20 */
    "####################"   /* 20 */
};

/* "Limpieza" simple y compatible: imprime saltos de linea */
static void consola_limpiar_simple(void) {
    for (int i = 0; i < 40; i++) {
        putchar('\n');
    }
}

/* Regresa 1 si es pared, 0 si NO es pared */
static int es_pared(const char mapa[ALTO][ANCHO + 1], int x, int y) {
    /* Si nos salimos del mapa, lo tratamos como pared para simplificar */
    if (x < 0 || x >= ANCHO || y < 0 || y >= ALTO) {
        return 1; /* fuera del mapa = pared */
    }
    return (mapa[y][x] == '#');
}

/* Dibuja un frame completo */
static void renderizar(const char mapa[ALTO][ANCHO + 1], int jugador_x, int jugador_y, int pasos) {
    consola_limpiar_simple();

    /* HUD minimo */
    printf("SESION 1 | Pasos: %d\n\n", pasos);

    for (int y = 0; y < ALTO; y++) {
        for (int x = 0; x < ANCHO; x++) {
            /* El jugador se dibuja por encima del mapa */
            if (x == jugador_x && y == jugador_y) {
                putchar('@');
            } else {
                putchar(mapa[y][x]);
            }
        }
        putchar('\n');
    }

    puts("\nControles: W A S D + Enter | Q para salir");
}

int main(void) {
    /* Copiamos el mapa fijo a una matriz editable */
    char mapa[ALTO][ANCHO + 1];

    for (int y = 0; y < ALTO; y++) {
        /* Copiamos EXACTAMENTE ANCHO caracteres */
        strncpy(mapa[y], MAPA_FIJO[y], ANCHO);
        /* Forzamos terminacion de cadena */
        mapa[y][ANCHO] = '\0';
    }

    /* Posicion inicial del jugador (asegurate de que sea piso) */
    int xJugador = 1;
    int yJugador = 1;

    int pasos = 0;
    char tecla;

    while (1) {
        renderizar(mapa, xJugador, yJugador, pasos);

        printf("> ");

        /* IMPORTANTE: el espacio antes de %c ignora '\n' y espacios previos */
        if (scanf(" %c", &tecla) != 1) {
            break; /* error de lectura */
        }

        if (tecla == 'q' || tecla == 'Q') {
            break;
        }

        /* Convertimos tecla a desplazamiento (dx, dy) */
        int dx = 0, dy = 0;

        if (tecla == 'w' || tecla == 'W') dy = -1;      /* arriba */
        else if (tecla == 's' || tecla == 'S') dy = 1;  /* abajo */
        else if (tecla == 'a' || tecla == 'A') dx = -1; /* izquierda */
        else if (tecla == 'd' || tecla == 'D') dx = 1;  /* derecha */

        int nx = xJugador + dx;
        int ny = yJugador + dy;

        /* Solo movemos si NO hay pared */
        if (!es_pared(mapa, nx, ny)) {
            xJugador = nx;
            yJugador = ny;
            pasos++;
        } else {
            /* Mensaje opcional: puedes comentar esto si "ensucia" la consola */
            puts("No puedes atravesar paredes.");
        }
    }

    puts("Fin del prototipo. Gracias por jugar!");
    return 0;
}
