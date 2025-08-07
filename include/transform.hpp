#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Substructs
struct position
{
    float x, y, z;

    position() = default;
    position(float x, float y, float z) 
    : x(x), y(y), z(z) {}

    inline void translate(float x, float y, float z);
    inline void translate(float x, float y);

    inline void setPosition(float x, float y, float z);
    inline void setPosition(float x, float y);

    inline void moveX(float x);
    inline void moveY(float y);
    inline void moveZ(float z);
};

inline void position::translate(float x, float y, float z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}
inline void position::translate(float x, float y)
{
    this->x += x;
    this->y += y;
}

inline void position::setPosition(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
inline void position::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

inline void position::moveX(float x) { position::x += x; }
inline void position::moveY(float y) { position::y += y; }
inline void position::moveZ(float z) { position::z += z; }

struct rotation
{
    float x;

    rotation() = default;

    rotation(float rotation)
    : x(rotation) {};

    inline void rotate(float rotationRadians);

    inline void setRotation(float rotationToSetRadians);
};

inline void rotation::rotate(float rotationRadians)
{
    x += rotationRadians;
}

inline void rotation::setRotation(float rotationToSetRadians)
{
    x = rotationToSetRadians;
}

struct scale
{
    float x, y, z;

    scale() = default;
    
    scale(float x, float y, float z) 
    : x(x), y(y), z(z) {}

    inline void addScale(float x, float y, float z);
    inline void addScale(float x, float y);

    inline void scalar(float x, float y, float z);
    inline void scalar(float x, float y);
    
    inline void setScale(float x, float y, float z);
    inline void setScale(float x, float y);

    // Rescaling for just X Y and Z individually would've been a PITA
};

inline void scale::addScale(float x, float y, float z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}
inline void scale::addScale(float x, float y)
{
    this->x += x;
    this->y += y;
}

inline void scale::scalar(float x, float y, float z)
{
    this->x *= x;
    this->y *= y;
    this->z *= z;
}
inline void scale::scalar(float x, float y)
{
    this->x *= x;
    this->y *= y;
}

inline void scale::setScale(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
inline void scale::setScale(float x, float y)
{
    this->x = x;
    this->y = y;
}

// Main struct (What the dev will be using)
struct transform
{
    position objectPosition;
    rotation objectRotation;
    scale objectScale;

    transform() = default;

    transform(position pos, rotation rot, scale sca)
    : objectPosition(pos), objectRotation(rot), objectScale(sca) {}

    transform(float pos[3], float rot, float sca[3])
    : objectPosition(position(pos[0], pos[1], pos[2])), objectRotation(rotation(rot)), objectScale(scale(sca[0], sca[1], sca[2])) {} // I initially used lambdas here which is just depressing.

    inline glm::mat4 genTransformationIdentityMatrix();
};

inline glm::mat4 transform::genTransformationIdentityMatrix()
{
    glm::mat4 identityMatrix(1.0f);
    identityMatrix = glm::translate(identityMatrix, glm::vec3(this->objectPosition.x, this->objectPosition.y, this->objectPosition.z));
    identityMatrix = glm::rotate(identityMatrix, this->objectRotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
    identityMatrix = glm::scale(identityMatrix, glm::vec3(this->objectScale.x, this->objectScale.y, this->objectScale.z));
    return identityMatrix;
}

#endif