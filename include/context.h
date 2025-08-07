#ifndef CONTEXT_H
#define CONTEXT_H

class context
{
    context() = default;
    ~context() = default;

    unsigned int width = 1920;
    unsigned int height = 1080;

    public:
    static double deltaTime;

    context(const context&) = delete;
    context& operator=(const context&) = delete;

    void setWidth(unsigned int width);
    void setHeight(unsigned int height);
    unsigned int getWidth();
    unsigned int getHeight();

    static context& getInstance();
};

#endif