#include <context.h>

context& context::getInstance() {
    static context instance;
    return instance;
}

void context::setWidth(unsigned int width) { this->width = width; }
void context::setHeight(unsigned int height) {this->height = height; }

unsigned int context::getWidth() { return width; }
unsigned int context::getHeight() { return height; }

double context::deltaTime = 0.0;
