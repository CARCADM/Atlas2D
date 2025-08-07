#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <context.h>
#include <iostream>

class camera2D
{
    glm::mat4 projection;

    public:
    camera2D();

    void recalculateProjectionMatrix();

    glm::mat4 getProjection();

    void applyProjection(unsigned int shader);
};  


#endif