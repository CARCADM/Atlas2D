#include <gameScript.h>

gameScript::gameScript() {}

void gameScript::wake() 
{
    std::cout << "WARNING DEFAULT FUNCTION IS BEING CALLED" << std::endl;
}

void gameScript::update(double deltaTime) 
{
    std::cout << "WARNING DEFAULT FUNCTION IS BEING CALLED" << std::endl;
}

void gameScript::runWakeFunctions()
{
    for(int i = 0; i < scriptRegistry.size(); ++i)
    {
        scriptRegistry[i]->wake();
    }    
}

void gameScript::runUpdateFunctionsOnce(double deltaTime)
{
    for(int i = 0; i < scriptRegistry.size(); ++i)
    {
        scriptRegistry[i]->update(deltaTime);
    }
}

std::pair<float, float> gameScript::pixelCoordsToScreenSpace(const int width, const int height, const int x, const int y) { return {x / width, y / height}; }

std::pair<int, int> gameScript::screenSpaceToPixelCoords(const int width, const int height, const float x, const float y) { return {x * width, y * height}; }

std::vector<std::unique_ptr<gameScript>> gameScript::scriptRegistry = {};