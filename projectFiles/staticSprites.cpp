#include <gameScript.h>
#include <renderer.h>
#include <sprite.h>
#include <staticSprites.h>

staticSprite2D gameStaticSprites;

void staticSprites::wake()
{
    gameStaticSprites = staticSprite2D("../test/Stardrop_Tea.png", glm::vec2(context::getInstance().getWidth()/2, context::getInstance().getHeight()/2));
    gameStaticSprites = staticSprite2D("../test/Aquamarine.png", glm::vec2(context::getInstance().getWidth()/2, context::getInstance().getHeight()/2 - 60));

    gameStaticSprites.genTextureAtlas(256, 256);
}

void staticSprites::update(double deltaTime)
{
    gameStaticSprites.drawStaticSprites(renderer::getBaseShader());
}