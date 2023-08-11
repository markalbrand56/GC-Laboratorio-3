// shaders.h
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "fragment.h"
#include "uniforms.h"
#include "gl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <random>


glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& u) {
    glm::vec4 v = glm::vec4(vertex.x, vertex.y, vertex.z, 1);


    glm::vec4 r =  u.viewport * u.projection * u.view * u.model * v;


    return glm::vec3(r.x/r.w, r.y/r.w, r.z/r.w);
};

std::vector<std::vector<glm::vec3>> primitiveAssembly (
    const std::vector<glm::vec3>& transformedVertices
) {
    // Assemble the transformed vertices into triangles
    // Return a vector of triangles, where each triangle is a vector of 3 vertices (vec3)

    std::vector<std::vector<glm::vec3>> triangles;

    for (int i = 0; i < transformedVertices.size(); i += 3) {
        triangles.push_back({
            transformedVertices[i],
            transformedVertices[i + 1],
            transformedVertices[i + 2]
        });
    }

    return triangles;
}

std::vector<Fragment> rasterize(const std::vector<std::vector<glm::vec3>>& assembledVertices) {
    std::vector<Fragment> fragments;

    for (const std::vector<glm::vec3>& triangleVertices : assembledVertices) {
        std::vector<Fragment> triangleFragments = triangle(triangleVertices[0], triangleVertices[1], triangleVertices[2]);
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return fragments;
}

Color fragmentShader(Fragment fragment) {
    // Initialize a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    // Generate random values for the red, green, and blue channels
    int red = dis(gen);
    int green = dis(gen);
    int blue = dis(gen);

    return Color{red, green, blue};
};

glm::mat4 createModelMatrix() {
    return glm::mat4(1);
}

glm::mat4 createViewMatrix() {
    return glm::lookAt(
            // donde esta
            glm::vec3(0, 0, -5),
            // hacia adonde mira
            glm::vec3(0, 0, 0),
            // arriba
            glm::vec3(0, 1, 0)
    );
}

glm::mat4 createProjectionMatrix() {
    float fovInDegrees = 45.0f;
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    return glm::perspective(glm::radians(fovInDegrees), aspectRatio, nearClip, farClip);
}

glm::mat4 createViewportMatrix() {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}