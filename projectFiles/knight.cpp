#include <sprite.h>
#include <gameScript.h>
#include <renderer.h>
#include <knight.h>

animatedSprite2D mySprite;
animator rightA;
animator leftA;

void knight::wake()
{
    mySprite = animatedSprite2D("../pixilart-sprite.png", 10, 1);
    mySprite.assignShader(renderer::getBaseShader());
    mySprite.Transform.objectPosition.setPosition(context::getInstance().getWidth()/2, context::getInstance().getHeight()/2, 0.0f);
    mySprite.Transform.objectScale.scalar(3.0f, 3.0f);
    rightA = animator(6, 2, 25);
    leftA = animator(8, 2, 25);
}
void knight::update(double deltaTime)
{
    mySprite.drawSprite();
    if (renderer::getKeyHeld(GLFW_KEY_A))
    {
        mySprite.runAnimation(&leftA);
        mySprite.Transform.objectPosition.translate(-2.0f, 0.0f, 0.0f);
    }
    else if (renderer::getKeyHeld(GLFW_KEY_D))
    {
        mySprite.runAnimation(&rightA);
        mySprite.Transform.objectPosition.translate(2.0f, 0.0f, 0.0f);
    }
    else
    {
        mySprite.setTexturePosition(0, 0);
    }
    if(mySprite.Transform.objectPosition.x > context::getInstance().getWidth() + 50)
    {
        mySprite.Transform.objectPosition.setPosition(-50, context::getInstance().getHeight()/2);
    }
    else if(mySprite.Transform.objectPosition.x < -50)
    {
        mySprite.Transform.objectPosition.setPosition(context::getInstance().getWidth() + 50, context::getInstance().getHeight()/2);
    }

    if(renderer::getKeyHeld(GLFW_KEY_R))
    {
        // Begin rotation
        mySprite.Transform.objectRotation.setRotation(M_PI);
    }
}