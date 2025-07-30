#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h> // Para usar 'bool', 'true' y 'false' en C

// --- Declaraciones opacas para dependencias ---
// Esto evita tener que incluir otros headers aquí, manteniendo la compilación rápida.
struct Keyboard;

// --- Declaración de la estructura opaca 'Game' ---
// El código cliente (como main.c) no necesita saber los detalles internos
// de la estructura 'Game', solo cómo interactuar con ella a través de un puntero.
typedef struct Game Game;

// --- Prototipos de Funciones de la API del Juego ---

/**
 * @brief Crea y reserva memoria para una nueva instancia del juego.
 * @return Un puntero a la nueva instancia de Game, o NULL si falla la alocación.
 */
Game* game_create(void);

/**
 * @brief Inicializa SDL, la ventana, el renderer y los recursos del juego.
 * @param game Puntero a la instancia del juego.
 * @param title Título de la ventana.
 * @param width Ancho de la ventana.
 * @param height Alto de la ventana.
 * @return 0 si la inicialización fue exitosa, -1 si hubo un error.
 */
int game_init(Game* game, const char* title, int width, int height);

/**
 * @brief Verifica si el bucle del juego debe continuar.
 * @param game Puntero a la instancia del juego.
 * @return true si el juego está en ejecución, false en caso contrario.
 */
bool game_is_running(const Game* game);

/**
 * @brief Procesa todos los eventos de SDL pendientes (teclado, ratón, ventana, etc.).
 * @param game Puntero a la instancia del juego.
 */
void game_handle_events(Game* game);

/**
 * @brief Actualiza la lógica del juego.
 * @param game Puntero a la instancia del juego.
 * @param delta_time El tiempo transcurrido en segundos desde el último fotograma.
 */
void game_update(Game* game, double delta_time);

/**
 * @brief Dibuja el estado actual del juego en la pantalla.
 * @param game Puntero a la instancia del juego.
 */
void game_render(const Game* game);

/**
 * @brief Libera todos los recursos asociados con el juego y la propia instancia.
 * @param game Puntero a la instancia del juego a destruir.
 */
void game_destroy(Game* game);

#endif // GAME_H