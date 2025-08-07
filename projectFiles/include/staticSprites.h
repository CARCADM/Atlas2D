#ifndef STATIC_SPRITES_H
#define STATIC_SPRITES_H

class staticSprites : public gameScript
{
    void wake() override;

    void update(double deltaTime) override;
};

#endif