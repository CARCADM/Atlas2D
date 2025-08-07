#include <animator.h>

animator::animator() {}

animator::animator(unsigned int sP, unsigned int stride, unsigned int delay)
{
    unsigned int tmpArr[3] = {sP, stride - 1, delay};

    std::copy(std::begin(tmpArr), std::end(tmpArr), animationSheet);
    animationIndex = 0;
    internalTimer = 0.0f;
}