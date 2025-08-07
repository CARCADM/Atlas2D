#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <iostream>

struct animator
{
    unsigned int animationSheet[3];
    unsigned int animationIndex;
    float internalTimer;

    animator();

    animator(unsigned int sP, unsigned int stride, unsigned int delay);
};

#endif