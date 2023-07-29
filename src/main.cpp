#include <iostream>
#include <SDL.h>
#include "gl.h"

void render() {
    std::vector<glm::vec3> vertices = {
            glm::vec3(320.0f, 240.0f, 0.0f),
            glm::vec3(220.0f, 140.0f, 0.0f),
            glm::vec3(420.0f, 140.0f, 0.0f)
    };

    triangle(vertices[0], vertices[1], vertices[2]);
}

int main(int argc, char** argv) {
    init();

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    // Load the OBJ file
    bool success = loadOBJ("../model/quinjet.obj", vertices, faces);
    if (!success) {
        std::cerr << "Error loading OBJ file!" << std::endl;
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        for (const auto& face : faces) {
            glm::vec3 A = vertices[face.vertexIndices[0][0] - 1];
            glm::vec3 B = vertices[face.vertexIndices[1][0] - 1];
            glm::vec3 C = vertices[face.vertexIndices[2][0] - 1];

            A *= 100.0f;
            B *= 100.0f;
            C *= 100.0f;

            triangle(A, B, C);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
