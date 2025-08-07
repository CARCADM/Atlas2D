#include <orthoCamera.h>

camera2D::camera2D()
{
    projection = glm::ortho(
        0.0f, static_cast<float>(context::getInstance().getWidth()),
        static_cast<float>(context::getInstance().getHeight()), 0.0f,
        -1.0f, 1.0f
    );
}

void camera2D::recalculateProjectionMatrix()
{
    projection = glm::ortho(
        0.0f, static_cast<float>(context::getInstance().getWidth()),
        static_cast<float>(context::getInstance().getHeight()), 0.0f,
        -1.0f, 1.0f
    );}

glm::mat4 camera2D::getProjection()
{
    return projection;
}

void camera2D::applyProjection(unsigned int shader)
{
    unsigned int projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}