#include <iostream>
#include <SDL.h>
#include "gl.h"
#include <glm/gtc/matrix_transform.hpp> // Include the matrix transformation functions from GLM
#include <glm/gtx/rotate_vector.hpp> // Include the rotate_vector function from GLM
#include "fragment.h"
#include "uniforms.h"

// Variables to control the rotation of the model
const auto pi = glm::pi<float>();
float angleZ = pi;
float angleX = 0;
float angleY = pi/2;

void render(const Uniforms& uniforms, const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    // Clear the framebuffer
    clear();

    // Draw the model

    for (const auto& face : faces) {
        glm::vec3 A = vertices[face.vertexIndices[0][0] - 1];
        glm::vec3 B = vertices[face.vertexIndices[1][0] - 1];
        glm::vec3 C = vertices[face.vertexIndices[2][0] - 1];

        const float size = 65.0f;

        A *= size;
        B *= size;
        C *= size;

        A = glm::rotateZ(A, angleZ);
        B = glm::rotateZ(B, angleZ);
        C = glm::rotateZ(C, angleZ);

        A = glm::rotateX(A, angleX/4);
        B = glm::rotateX(B, angleX/4);
        C = glm::rotateX(C, angleX/4);
//
        A = glm::rotateY(A, angleY);
        B = glm::rotateY(B, angleY);
        C = glm::rotateY(C, angleY);

        // Ajustar a escala
        A.x += 320.0f;
        A.y += 240.0f;
        B.x += 320.0f;
        B.y += 240.0f;
        C.x += 320.0f;
        C.y += 240.0f;

        triangle(A, B, C);
    }
}

glm::mat4 createModelMatrix() {
    glm::mat4 model = glm::mat4(1);
    model = glm::rotate(model, angleZ, glm::vec3(0, 0, 1));
    model = glm::rotate(model, angleX, glm::vec3(1, 0, 0));
    model = glm::rotate(model, angleY, glm::vec3(0, 1, 0));
    return model;
}

glm::mat4 createViewMatrix() {
    glm::vec3 eye = glm::vec3(0, 0, 0);  // Donde esta la camara
    glm::vec3 center = glm::vec3(0, 0, 1);  // Hacia donde mira la camara
    glm::vec3 up = glm::vec3(0, 1, 0);  // Hacia donde esta el "cielo"
    glm::mat4 view = glm::lookAt(eye, center, up);
    return view;
}

glm::mat4 createProjectionMatrix() {
    float fovy = glm::radians(60.0f);
    float aspect = 640.0f / 480.0f;
    float near = 0.1f;
    float far = 100.0f;
    glm::mat4 projection = glm::perspective(fovy, aspect, near, far);
    return projection;
}

glm::mat4 createViewportMatrix() {
    glm::mat4 viewport = glm::mat4(1);
    viewport = glm::translate(viewport, glm::vec3(320, 240, 0));
    viewport = glm::scale(viewport, glm::vec3(320, 240, 1));
    return viewport;
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

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    uniforms.model = model;
    uniforms.model = view;
    uniforms.model = projection;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderPresent(renderer);

        angleY += 0.001f;  // Rotate the model around the Y axis continuously
        std::cout << "angleY: " << angleY << std::endl;

        render(uniforms, vertices, faces);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
