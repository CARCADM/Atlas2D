cmake_minimum_required(VERSION 3.30)
project(Atlas2D)

set(CMAKE_WIN32_EXECUTABLE FALSE)

include_directories(include
    deps/include
    projectFiles/include
)

add_executable(Atlas2D
projectFiles/main.cpp 
projectFiles/knight.cpp
projectFiles/staticSprites.cpp
src/gameScript.cpp
src/sprite.cpp
src/context.cpp
src/orthoCamera.cpp
src/renderer.cpp
src/util/animator.cpp
src/util/objGen.cpp
src/util/shader.cpp 
src/util/stb_image.cpp
deps/src/glad.c)

set_target_properties(Atlas2D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

target_link_libraries(Atlas2D ${CMAKE_SOURCE_DIR}/deps/libs/libglfw3.a opengl32)