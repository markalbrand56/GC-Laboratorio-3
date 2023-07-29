#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "gl.h"

void render() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            point(i, j);
        }
    }
}

int main(int argc, char** argv) {
    init();
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
