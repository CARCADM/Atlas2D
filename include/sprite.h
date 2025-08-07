#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <context.h>
#include <objGen.h>
#include <animator.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <memory>
#include <transform.hpp>

// A quick and simple struct to store neccessary metadata for each texture.
struct texCoordMetadata
{
    int xPos;
    int yPos;
    int width;
    int height;

    texCoordMetadata(int xPos, int yPos, int width, int height)
    : xPos(xPos), yPos(yPos), width(width), height(height) {}
};

// Stores metadata for an image
struct imageData
{
    int width, height, nrChannels;
    unsigned char* data;

    imageData() = default;

    imageData(int width, int height, int nrChannels, unsigned char* data) 
    : width(width), height(height), nrChannels(nrChannels), data(data) {}
};

// Stores one piece of data for a heightmap
struct heightData
{
    int height, stride;

    heightData(int height, int stride)
    : height(height), stride(stride) {}
};

struct sprite
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int shader;
    unsigned int texture;
    transform Transform;

    static std::vector<imageData> textureAtlasData;
};

class sprite2D : public sprite
{
    public:
    sprite2D() = default;
    sprite2D(const char* pathToSprite);
    
    void assignShader(unsigned int shaderToAssign);

    void drawSprite();
};

class animatedSprite2D : public sprite
{
    float texCoordStep[2];
    float texCoordOffset[2];
    float spriteState[3];
    animator* currentAnimation;

    public:

    animatedSprite2D() = default;
    animatedSprite2D(const char* pathToSprite, int horizontalCuts, int verticalCuts);
    
    void assignShader(unsigned int shaderToAssign);

    void translate(float x, float y, float z);
    glm::vec4 getPosition();
    void setPosition(float x, float y, float z);

    void scale(float x, float y);

    void rotate(float r); 

    void setTexturePosition(unsigned int x, unsigned int y);
    void nextAnimationState();
    // First two ints represents starting pos, the last value specifies length
    void runAnimation(animator* animation);

    void drawSprite();
};

struct staticSprite2D
{
    // Static stored raw data
    static std::vector<float> combinedVertexData;
    static std::vector<float> combinedTexCoordData;
    static std::vector<unsigned int> combinedIndiciesData;

    static imageData atlasImageData;

    // Static stored buffer data
    static unsigned int staticVBO;
    static unsigned int staticVAO;
    static unsigned int staticEBO;
    static unsigned int staticTexture;
    static unsigned int textureAtlasBuffer;

    staticSprite2D() = default;
    staticSprite2D(const char* pathToSprite, glm::vec2 vertexOffset);

    static void releaseAllImageData();

    static void genTextureAtlas(int texAtlasWidth, int texAtlasHeight);

    static void buildTextureAtlas(std::vector<texCoordMetadata>& xyPos, int& width, int& height);

    static void addImageData(const char* pathToImageData, int& width, int& height);

    static void generateStaticVAO();

    static void drawStaticSprites(unsigned int shader);
};

#endif