#include <iostream>
#include <SDL.h>
#include "gl.h"
#include "camera.h"
#include "uniforms.h"
#include "shaders.h"

void render(const Uniforms& uniforms, const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    // 1. Vertex Shader
    // vertex -> trasnformedVertices

    std::vector<glm::vec3> transformedVertices;
    for(glm::vec3 vertex : vertices) {
        glm::vec3 transformedVertex = vertexShader(vertex, uniforms);
        transformedVertices.push_back(transformedVertex);
    }

    // 2. Primitive Assembly
    // transformedVertices -> triangles
    std::vector<std::vector<glm::vec3>> triangles = primitiveAssembly(transformedVertices);

    // 3. Rasterize
    // triangles -> Fragments
    std::vector<Fragment> fragments;
    for (const std::vector<glm::vec3>& triangleVertices : triangles) {
        std::vector<Fragment> rasterizedTriangle = triangle(
                triangleVertices[0],
                triangleVertices[1],
                triangleVertices[2]
        );

        fragments.insert(
                fragments.end(),
                rasterizedTriangle.begin(),
                rasterizedTriangle.end()
        );
    }

    // 4. Fragment Shader
    // Fragments -> colors

    for (Fragment fragment : fragments) {
        Color fragColor = fragmentShader(fragment);
        point(fragment.position.x, fragment.position.y, fragColor);
    }
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

        // Create uniforms
        Uniforms uniforms;
        uniforms.model = createModelMatrix();
        uniforms.view = createViewMatrix();
        uniforms.projection = createProjectionMatrix();
        uniforms.viewport = createViewportMatrix();

        clear();

        // Render
        render(uniforms, vertices, faces);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
