#include <gameScript.h>
#include <renderer.h>
#include <knight.h>
#include <staticSprites.h>

int main()
{
    knight oKnight;
    staticSprites oStaticSprites;
    gameScript::scriptRegistry.emplace_back(std::make_unique<staticSprites>(oStaticSprites));
    gameScript::scriptRegistry.emplace_back(std::make_unique<knight>(oKnight));
    renderer::startRenderLoop();
    return 0;
}