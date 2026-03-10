#ifndef JUEGO_H
#define JUEGO_H

//Definir dimensiones fijas del mapa para la sesion
#define ALTO 10
#define ANCHO 20
#define TROFEOS_PARA_GANAR 10


//Definir enumación para representar el estado actual del programa
//Gracias a la enumación, podemos conocer en todo momento el estado del sistema
typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_CONFIRMAR_MENU,
    ESTADO_INSTRUCCIONES,
    ESTADO_SALIR,
    ESTADO_VICTORIA,
    ESTADO_DERROTA
} EstadoJuego;


//Definir estructura utilitaria para almacenar estado del juego
typedef struct {
    char mapa[ALTO][ANCHO+1]; //+1 para el caracter nulo de fin de linea
    int jugador_x;
    int jugador_y;
    int pasos;
    int choques;
    int partida_activa;
    char ultima_tecla;
    char mensaje[128];
    int meta_x; 
    int meta_y;
    int enemigo_x;
    int enemigo_y;
    int enemigo_activo;
    int trofeo_activo;
    int trofeos_capturados;
    EstadoJuego estado;
} Juego;

//Funciones para inicializar y actualizar el juego
void juego_inicializar(Juego *j);
void juego_reiniciar_partida(Juego *j);

void juego_intentar_mover(Juego *j, int dx, int dy);

int juego_es_pared(const Juego *j, int x, int y);

void juego_agregar_trofeo(Juego *j, int x, int y);
void juego_agregar_trofeo_aleatorio(Juego *j);
void juego_mover_enemigo(Juego *j);
int juego_posicion_ocupada(const Juego *j, int x, int y);
int juego_agregar_enemigo_aleatorio(Juego *j);
#endif // JUEGO_H

