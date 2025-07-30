#include <stdio.h>
#include <stdlib.h> // Para malloc y free
#include <SDL_image.h>
#include "game.h"
#include "engine/graphics/texture.h"
#include "engine/input/input.h"
#include "hero.h"

struct Game {
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Texture* logoTexture;
    SDL_Rect destLogoRect;
    Hero* hero;
    InputHandler* inputHandler;
    Keyboard* keyboard;
};


Game* game_create(void) {

    Game* game = (Game*)calloc(1, sizeof(Game));
    if (game == NULL) {
        return NULL; // Error de alocación de memoria
    }
    return game;
}

int game_init(Game* game, const char* title, int width, int height) {
    if (game == NULL) return -1;

    // 1. Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error inicializando SDL: %s\n", SDL_GetError());
        return -1;
    }

    // 2. Crear Ventana y Renderer
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &game->window, &game->renderer) != 0) {
        fprintf(stderr, "Error creando ventana y renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_SetWindowTitle(game->window, title);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // Fondo blanco

    // 3. Cargar texturas y objetos (traducción de Game::init)

    // Logo
    game->logoTexture = texture_create("../src/img/logo.png", game->renderer);
    if (texture_init(game->logoTexture) != 0) {
        // El error ya se imprime dentro de texture_init
        return -1; // Salimos si la textura no se pudo cargar
    }
    game->destLogoRect = (SDL_Rect){0, 0, 150, 150};

    // Sistema de Input
    game->inputHandler = input_handler_create();
    game->keyboard = keyboard_create();

    // Mapear teclas (reemplaza a keyboard->addButtonMap)
    keyboard_add_button_map(game->keyboard, ACTION_RIGHT, SDLK_d);
    keyboard_add_button_map(game->keyboard, ACTION_LEFT, SDLK_a);

    input_handler_add_controller(game->inputHandler, (Controller*)game->keyboard);

    // Héroe
    game->hero = hero_create(game->renderer, (Controller*)game->keyboard);
    if (hero_init(game->hero) != 0) {
        return -1;
    }

    game->isRunning = true;
    printf("Motor de juego inicializado correctamente.\n");
    return 0; // Éxito
}

void game_handle_events(Game* game) {
    SDL_Event event;

    // Primero, reseteamos el estado de los botones del frame anterior
    input_handler_reset_events(game->inputHandler);

    // Luego, procesamos todos los eventos nuevos en la cola
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->isRunning = false;
        }
        // Pasamos el evento al manejador de input para que actualice los controladores
        input_handler_handle_event(game->inputHandler, &event);
    }
}

void game_update(Game* game, double delta_time) {
    if (game == NULL || !game->isRunning) return;

    // Actualizamos todos los objetos del juego
    hero_update(game->hero, delta_time);
}

void game_render(const Game* game) {
    if (game == NULL) return;

    // Limpiar pantalla
    SDL_RenderClear(game->renderer);

    // --- Dibujar todo aquí ---
    texture_render(game->logoTexture, &game->destLogoRect);
    hero_render(game->hero);
    // -------------------------

    // Mostrar en pantalla
    SDL_RenderPresent(game->renderer);
}

void game_destroy(Game* game) {
    if (game == NULL) return;

    printf("Liberando recursos...\n");

    // Liberamos en orden inverso a la creación
    hero_destroy(game->hero);
    texture_destroy(game->logoTexture);

    // El input handler no tiene recursos que liberar, pero el keyboard sí
    keyboard_destroy(game->keyboard);
    input_handler_destroy(game->inputHandler);

    // Liberamos los recursos de SDL
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();

    // Finalmente, liberamos la memoria de la estructura principal
    free(game);
}

bool game_is_running(const Game* game) {
    // Si game es NULL, no está corriendo.
    return game != NULL && game->isRunning;
}