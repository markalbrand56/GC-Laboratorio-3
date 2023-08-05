#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "fragment.h"
#include "uniforms.h"

glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    // Return the transformed vertex as a vec3

    glm::vec4 v = glm::vec4(vertex, 1.0f);

    v = v * uniforms.model * uniforms.view * uniforms.projection * uniforms.viewport;

    return {v.x, v.y, v.z};
}