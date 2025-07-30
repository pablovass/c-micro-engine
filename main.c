#include <stdio.h>
#include <SDL.h>
#include "game.hh"

int main(int argc, char* argv[]) {

    Game* game = game_create();
    if (game == NULL) {
        fprintf(stderr, "Error: No se pudo crear el objeto del juego.\n");
        return -1;
    }

    if (game_init(game, "Mini-Engine C", 512, 512) != 0) {

        game_destroy(game);
        return -1;
    }

    Uint32 last_time = SDL_GetTicks();

    while (game_is_running(game)) {
        Uint32 current_time = SDL_GetTicks();

        double delta_time = (current_time - last_time) / 1000.0;
        last_time = current_time;


        game_handle_events(game);


        game_update(game, delta_time);


        game_render(game);
    }

    game_destroy(game);

    return 0;
}