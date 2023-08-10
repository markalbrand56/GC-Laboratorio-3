cmake_minimum_required(VERSION 3.25)
project(Laboratorio_3)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR C:/Users/marka/Coding/Recursos/Librerías/SDL2/include)
set(SDL2_LIB_DIR C:/Users/marka/Coding/Recursos/Librerías/SDL2/lib/x64)

find_package(glm CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(Laboratorio_3 src/main.cpp
        src/fragment.h
        src/color.h
        src/uniforms.h
        src/shaders.h
        src/camera.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 glm::glm)
