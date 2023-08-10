// shaders.h
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "fragment.h"
#include "uniforms.h"
#include "gl.h"
#include <glm/gtc/matrix_transform.hpp>


glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;


    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

    // Return the transformed vertex as a vec3
    return glm::vec3(screenVertex);
}

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

Color fragmentShader(const Fragment& fragment) {
    // Example: Assign a constant color to each fragment
    Color fragColor(255, 0, 0, 255); // Red color with full opacity

    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)

    return fragColor;
}

glm::mat4 createModelMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Aplicar transformaciones a la matriz de modelo
    modelMatrix = glm::translate(modelMatrix, translation);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación en X
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en Z
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}

glm::mat4 createProjectionMatrix() {
    float fovInDegrees = 45.0f;
    float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
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