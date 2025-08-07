#ifndef GAME_SCRIPT_H
#define GAME_SCRIPT_H

#include <iostream>
#include <context.h>
#include <memory>
#include <vector>

// Main inheritable class
struct gameScript
{ 

    static std::vector<std::unique_ptr<gameScript>> scriptRegistry;

    gameScript();

    virtual void wake();
    
    virtual void update(double deltaTime);

    static void runWakeFunctions();

    static void runUpdateFunctionsOnce(double deltaTime);

    static std::pair<float, float> pixelCoordsToScreenSpace(const int width, const int height, const int x, const int y);

    static std::pair<int, int> screenSpaceToPixelCoords(const int width, const int height, const float x, const float y);
};

#endif