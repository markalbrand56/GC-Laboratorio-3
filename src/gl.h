// gl.h
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "color.h"
#include "fragment.h"


struct Face {
    std::vector<std::array<int, 3>> vertexIndices;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Color currentColor = {255, 255, 255, 255}; // Initially set to white
Color clearColor = {0, 0, 0, 255}; // Initially set to black


void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setColor(const Color& color) {
    currentColor = color;
}


// Function to clear the framebuffer with the clearColor
void clear() {
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);
}

// Function to set a specific pixel in the framebuffer to the currentColor
void point(int x, int y, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

std::vector<Fragment> line(Vertex A, Vertex B) {
    int x1 = A.position.x;
    int y1 = A.position.y;
    int x2 = B.position.x;
    int y2 = B.position.y;
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */


    std::vector<Fragment> lineFragments;

    while (true) { /* loop */
        lineFragments.push_back(Fragment{glm::ivec2(x1, y1), A.color});
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { /* e_xy + e_x > 0 */
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { /* e_xy + e_y < 0 */
            err += dx;
            y1 += sy;
        }
    }

    return lineFragments;
}

std::vector<Fragment> triangle(Vertex a, Vertex b, Vertex c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    std::vector<Fragment> triangleFragments;

    std::vector<Fragment> line1 = line(a, b);
    std::vector<Fragment> line2 = line(b, c);
    std::vector<Fragment> line3 = line(c, a);

    triangleFragments.insert(
            triangleFragments.end(),
            line1.begin(),
            line1.end()
    );

    triangleFragments.insert(
            triangleFragments.end(),
            line2.begin(),
            line2.end()
    );

    triangleFragments.insert(
            triangleFragments.end(),
            line3.begin(),
            line3.end()
    );

    return triangleFragments;
}



