#include <stdlib.h>
#include <stdio.h>

#include "hero.h"
// Asumimos que estos archivos ya han sido o serán migrados a C
#include "engine/graphics/animation.h"
#include "engine/input/controller.h"

// --- Constantes para las acciones ---
// Es mejor definirlas aquí o en un header de input si son globales.
#define ACTION_LEFT  1
#define ACTION_RIGHT 2

// --- Definición de la estructura interna de Hero ---
// Esta es la implementación "privada" de la estructura.
struct Hero {
    SDL_Renderer* renderer;
    Controller* controller;
    Animation* heroAnimation;

    SDL_Rect destHeroRect;
    double heroXPosition; // Usamos double para un movimiento más suave
};

// --- Implementación de las funciones ---

Hero* hero_create(SDL_Renderer* renderer, Controller* controller) {
    // Usamos calloc para asegurarnos de que toda la memoria se inicialice a cero/NULL
    Hero* hero = (Hero*)calloc(1, sizeof(Hero));
    if (hero == NULL) {
        fprintf(stderr, "Error: No se pudo alocar memoria para Hero.\n");
        return NULL;
    }

    hero->renderer = renderer;
    hero->controller = controller;

    return hero;
}

int hero_init(Hero* hero) {
    if (hero == NULL) return -1;

    // Crear la animación (asumiendo que existe una API en C para Animation)
    hero->heroAnimation = animation_create("../src/img/pepe.png", hero->renderer, 6, 1.0f, 98, 84);
    if (animation_init(hero->heroAnimation) != 0) {
        // animation_init debería imprimir su propio error
        hero_destroy(hero); // Liberamos lo que se haya podido crear
        return -1;
    }

    // Inicializar la posición y el rectángulo de destino
    hero->destHeroRect.x = 0;
    hero->destHeroRect.y = 150;
    hero->destHeroRect.w = 150;
    hero->destHeroRect.h = 150;
    hero->heroXPosition = 0.0;

    return 0; // Éxito
}

void hero_update(Hero* hero, double delta_time) {
    if (hero == NULL) return;

    // Actualizar posición basada en el input (usando la API de Controller en C)
    if (controller_get_button(hero->controller, ACTION_RIGHT)) {
        hero->heroXPosition += 150 * delta_time; // Aumentamos la velocidad para que se note más
    } else if (controller_get_button(hero->controller, ACTION_LEFT)) {
        hero->heroXPosition -= 150 * delta_time;
    }
    hero->destHeroRect.x = (int)hero->heroXPosition;

    // Actualizar la animación
    animation_update(hero->heroAnimation, delta_time);
}

void hero_render(const Hero* hero) {
    if (hero == NULL) return;

    // Renderizar la animación en la posición calculada
    animation_render(hero->heroAnimation, &hero->destHeroRect);
}

void hero_destroy(Hero* hero) {
    if (hero == NULL) return;

    // Liberar los recursos internos
    animation_destroy(hero->heroAnimation);

    // Finalmente, liberar la memoria de la estructura Hero
    free(hero);
}