// fragment.h
#pragma once
#include "glm/glm.hpp"
#include "color.h"

struct Fragment {
    glm::ivec2 position; // X and Y coordinates of the pixel (in screen space)
    Color color;
};

struct Vertex {
    glm::vec3 position;
    Color color;
};

