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

std::vector<Fragment> line(glm::vec3 A, glm::vec3 B) {
    int x1 = A.x;
    int y1 = A.y;
    int x2 = B.x;
    int y2 = B.y;
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */


    std::vector<Fragment> lineFragments;

    while (true) { /* loop */
        lineFragments.push_back(Fragment{glm::ivec2(x1, y1)});
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

std::vector<Fragment> triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    std::vector<Fragment> triangleFragments;

    std::vector<Fragment> line1 = line(A, B);
    std::vector<Fragment> line2 = line(B, C);
    std::vector<Fragment> line3 = line(C, A);

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


bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces) {
    // Open the OBJ file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening OBJ file: " << path << std::endl;
        return false;
    }

    // Variables to store the vertices and faces from the OBJ file
    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Split the line into tokens
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Process vertex data
        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
            // Process face data
        else if (type == "f") {
            Face face;
            std::string vertexStr;
            while (iss >> vertexStr) {
                std::array<int, 3> vertexIndices;
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::istringstream viss(vertexStr);
                viss >> vertexIndices[0] >> vertexIndices[1] >> vertexIndices[2];
                face.vertexIndices.push_back(vertexIndices);
            }
            faces.push_back(face);
        }
    }

    // Close the file
    file.close();

    // Return the extracted vertices and faces
    out_vertices = vertices;
    out_faces = faces;
    return true;
}