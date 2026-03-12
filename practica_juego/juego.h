#ifndef JUEGO_H
#define JUEGO_H

//Definir dimensiones fijas del mapa para la sesion
#define ALTO 10
#define ANCHO 20
#define TROFEOS_PARA_GANAR 20
#define MAX_ENEMIGOS 5
#define ENEMIGOS_INICIALES 1

#define MUNICION_INICIAL 6
#define MUNICION_MAXIMA 12
#define OBSTACULOS_MIN 10
#define OBSTACULOS_MAX 30

//Definir enumacion para representar el estado actual del programa
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

typedef enum{
    ARMA_CUCHILLO = 0,
    ARMA_PISTOLA = 1
} TipoArma;

//Definir estructura utilitaria para almacenar estado del juego
typedef struct {
    char mapa[ALTO][ANCHO + 1];
    int jugador_x;
    int jugador_y;
    int inicio_x;
    int inicio_y;
    int vidas;
    int pasos;
    int choques;
    int partida_activa;
    char ultima_tecla;
    char mensaje[128];
    int meta_x;
    int meta_y;
    int enemigos_x[MAX_ENEMIGOS];
    int enemigos_y[MAX_ENEMIGOS];
    int enemigos_activos[MAX_ENEMIGOS];
    int enemigos_trofeos[MAX_ENEMIGOS];
    int total_enemigos;
    int trofeo_activo;
    int trofeos_capturados;
    int proxima_meta_vida;
    EstadoJuego estado;
    int dir_x;
    int dir_y;
    TipoArma arma_actual;
    int municion;
    int enemigos_eliminados;
} Juego;

//Funciones para inicializar y actualizar el juego
void juego_inicializar(Juego *j);
void juego_reiniciar_partida(Juego *j);
void juego_intentar_mover(Juego *j, int dx, int dy);
void juego_mover_enemigo(Juego *j);

int juego_es_pared(const Juego *j, int x, int y);
int juego_hay_enemigo_en(const Juego *j, int x, int y);
int juego_posicion_ocupada(const Juego *j, int x, int y);
int juego_agregar_enemigo_aleatorio(Juego *j);

void juego_agregar_trofeo(Juego *j, int x, int y);
void juego_agregar_trofeo_aleatorio(Juego *j);

void juego_generar_mapa_dinamico(Juego *j);
int juego_indice_enemigo_en(const Juego *j, int x, int y);
void juego_eliminar_enemigo(Juego *j, int indice);
void juego_disparar_pistola(Juego *j);
void juego_atacar_cuchillo(Juego *j);
#endif // JUEGO_H
