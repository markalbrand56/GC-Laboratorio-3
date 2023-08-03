#include <iostream>
#include <SDL.h>
#include "gl.h"
#include <glm/gtc/matrix_transform.hpp> // Include the matrix transformation functions from GLM
#include <glm/gtx/rotate_vector.hpp> // Include the rotate_vector function from GLM

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

            const float size = 50.0f;

            A *= size;
            B *= size;
            C *= size;

            // Apply rotation
            const float angle = glm::pi<float>();
//            A = glm::rotate(A, angle, glm::vec3(0.0f, 0.0f, 1.0f));
//            B = glm::rotate(B, angle, glm::vec3(0.0f, 0.0f, 1.0f));
//            C = glm::rotate(C, angle, glm::vec3(0.0f, 0.0f, 1.0f));

            A = glm::rotateZ(A, angle);
            B = glm::rotateZ(B, angle);
            C = glm::rotateZ(C, angle);

            A = glm::rotateX(A, angle/4);
            B = glm::rotateX(B, angle/4);
            C = glm::rotateX(C, angle/4);

            A = glm::rotateY(A, angle/16);
            B = glm::rotateY(B, angle/16);
            C = glm::rotateY(C, angle/16);

            // Ajustar a escala
            A.x += 320.0f;
            A.y += 240.0f;
            B.x += 320.0f;
            B.y += 240.0f;
            C.x += 320.0f;
            C.y += 240.0f;


            triangle(A, B, C);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
