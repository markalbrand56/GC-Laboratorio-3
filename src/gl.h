#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
#include <algorithm>


// Define a Color struct to hold the RGB values of a pixel
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

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
void point(const glm::vec3& vertex) {
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
//    SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderDrawPoint(renderer, static_cast<int>(vertex.x), static_cast<int>(vertex.y));
}

void line(glm::vec3 start, glm::vec3 end) {
    int x1 = round(start.x), y1 = round(start.y);
    int x2 = round(end.x), y2 = round(end.y);

    // Draw a line between (x1, y1) and (x2, y2) using point. Bresenham's line algorithm
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        point(glm::vec3(x1, y1, 0.0f));
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    line(A, B);
    line(B, C);
    line(C, A);
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