#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "fragment.h"
#include "uniforms.h"

glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    // Return the transformed vertex as a vec3
}