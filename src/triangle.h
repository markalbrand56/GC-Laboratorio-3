#pragma once
#include "gl.h"

glm::vec3 barycentricCoordinates(const glm::vec3& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    glm::vec3 s[2];
    for (int i = 0; i < 2; i++) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    glm::vec3 u = glm::cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2) {
        return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }
    return glm::vec3(-1, 1, 1);
}

std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    // Calculate the bounding box of the triangle
    float minX = std::min(std::min(A.x, B.x), C.x);
    float minY = std::min(std::min(A.y, B.y), C.y);
    float maxX = std::max(std::max(A.x, B.x), C.x);
    float maxY = std::max(std::max(A.y, B.y), C.y);

    std::vector<Fragment> fragments;

    // Iterate over each point in the bounding box
    for (float y = minY; y <= maxY; ++y) {
        for (float x = minX; x <= maxX; ++x) {
            // Calculate barycentric coordinates for the point
            glm::vec3 P = glm::vec3(x, y, 0);
            glm::vec3 barycentric = barycentricCoordinates(P, a.position, b.position, c.position);

            // If the point's barycentric coordinates are all between 0 and 1, it lies within the triangle
            if (
                    barycentric.x >= 0 && barycentric.x <= 1 &&
                    barycentric.y >= 0 && barycentric.y <= 1 &&
                    barycentric.z >= 0 && barycentric.z <= 1)
            {
                // Interpolate the point's color using its barycentric coordinates
                Color color = a.color * barycentric.x + b.color * barycentric.y + c.color * barycentric.z;
//                std::cout << color.r << " " << color.g << " " << color.b << std::endl;

                P.z = barycentric.x * a.position.z + barycentric.y * b.position.z + barycentric.z * c.position.z;

                // Add the point to the fragment list
                fragments.push_back(Fragment{P, color});
            }
        }
    }
    return fragments;
}