#ifndef KNIGHT_H
#define KNIGHT_H

class knight : public gameScript
{
    void wake() override;

    void update(double deltaTime) override;
};

#endif