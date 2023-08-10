#include <iostream>
#include <SDL.h>
#include "gl.h"
#include "camera.h"
#include "uniforms.h"
#include "shaders.h"

void render(const Uniforms& uniforms, const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    clear();

    std::vector<glm::vec3> transformedVertices;
    for (const auto& vertex : vertices) {
        transformedVertices.push_back(vertexShader(vertex, uniforms));
    }

    std::vector<std::vector<glm::vec3>> assembledVertices = primitiveAssembly(transformedVertices);

    std::vector<Fragment> fragments = rasterize(assembledVertices);

    for (const auto& fragment : fragments) {
        Color fragColor = fragmentShader(fragment);
        point(glm::vec3(fragment.position.x, fragment.position.y, 0.0f)); // Draw the pixel
    }

    SDL_RenderPresent(renderer);
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

    // Set up camera
    Camera camera;
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create uniforms
    Uniforms uniforms;
    uniforms.model = createModelMatrix(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    uniforms.view = createViewMatrix(camera);
    uniforms.projection = createProjectionMatrix();
    uniforms.viewport = createViewportMatrix();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        render(uniforms, vertices, faces);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
