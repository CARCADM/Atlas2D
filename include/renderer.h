#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sprite.h>
#include <orthoCamera.h>
#include <gameScript.h>
#include <iostream>
#include <filesystem>
#include <typeinfo>
#include <memory>

static camera2D oCamera;
static GLFWwindow* gameWindow = NULL;
static unsigned int baseShaderID = 0;
static double deltaPrevious = 0.0;
static const double minFrameTime = 1.0 / 500.0;
static std::vector<std::pair<int, int>> keyBuffer;

// Simple inline data struct
// Created everytime a new render loop is started (Which a smart person would only do once)
struct rendererContext
{
    const char* projectName;
    int windowWidth;
    int windowHeight;
    bool isResizeable;

    rendererContext() = default;
    rendererContext(const char* name, int width, int height, bool canBeResized)
    : projectName(name), windowWidth(width), windowHeight(height), isResizeable(canBeResized) {}
};

static rendererContext mainRenderContext = rendererContext("Project Window", 1920, 1080, true);

struct renderer
{
    static void startRenderLoop(rendererContext customRenderContext);
    static void startRenderLoop();

    static bool getKeyPressed(int keyCode);
    static bool getKeyReleased(int keyCode);
    static bool getKeyHeld(int keyCode);

    static unsigned int getBaseShader();

    static void assignBaseShader(unsigned int newBaseShader);
};

#endif