#include <sprite.h>

sprite2D::sprite2D(const char* pathToSprite)
{
    int w, h;
    objGen::generateTexture(texture, pathToSprite, w, h);
    const float horizontalDistance = static_cast<float>(w);
    const float verticalDistance = static_cast<float>(h);
    Transform = transform({0.0f, 0.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f});

    const std::vector<float> vertices = {
        horizontalDistance, verticalDistance, 0.0f,   1.0f, 1.0f,
        horizontalDistance, -verticalDistance, 0.0f,  1.0f, 0.0f,
        -horizontalDistance, -verticalDistance, 0.0f, 0.0f, 0.0f,                     
        -horizontalDistance, verticalDistance, 0.0f,  0.0f, 1.0f
    };
    const std::vector<unsigned int> indicies = {
        2, 0, 3,
        0, 2, 1
    };

    objGen::generateVAO(VAO);
    objGen::generateVBO(VBO, 0, vertices, GL_STATIC_DRAW, texture);
    objGen::generateEBO(EBO, indicies, GL_STATIC_DRAW);
}

void sprite2D::assignShader(unsigned int shaderToAssign)
{
    shader = shaderToAssign;
}

// TODO implement some sort of batched rendering later on.
void sprite2D::drawSprite()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    unsigned int transLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(Transform.genTransformationIdentityMatrix())); // This method is REALLY inefficient
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Animated Sprite 2D implementation
animatedSprite2D::animatedSprite2D(const char* pathToSprite, int horizontalCuts, int verticalCuts)
{
    int w, h;
    int ch = horizontalCuts;
    int cv = verticalCuts;
    spriteState[0] = ch;
    spriteState[1] = cv;
    spriteState[2] = 0;
    objGen::generateTexture(texture, pathToSprite, w, h);
    const float horizontalDistance = static_cast<float>(w) / ch;
    const float verticalDistance = static_cast<float>(h) / cv;
    Transform = transform({0.0f, 0.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f});

    const float xStep = horizontalDistance / static_cast<float>(w);
    const float yStep = verticalDistance / static_cast<float>(h);
    texCoordStep[0] = xStep;
    texCoordStep[1] = yStep;

    const std::vector<float> vertices = {
        horizontalDistance, verticalDistance, 0.0f,   xStep, yStep,
        horizontalDistance, -verticalDistance, 0.0f,  xStep, 0.0f,
        -horizontalDistance, -verticalDistance, 0.0f, 0.0f, 0.0f,
        -horizontalDistance, verticalDistance, 0.0f,  0.0f, yStep
    };
    const std::vector<unsigned int> indicies = {
        2, 0, 3,
        0, 2, 1
    };

    objGen::generateVAO(VAO);
    objGen::generateVBO(VBO, 0, vertices, GL_STATIC_DRAW, texture);
    objGen::generateEBO(EBO, indicies, GL_STATIC_DRAW);

    setTexturePosition(0, 0);
}

void animatedSprite2D::assignShader(unsigned int shaderToAssign)
{
    shader = shaderToAssign;
}

void animatedSprite2D::setTexturePosition(unsigned int x, unsigned int y)
{
    texCoordOffset[0] = texCoordStep[0] * x;
    texCoordOffset[1] = texCoordStep[1] * y;
}
void animatedSprite2D::nextAnimationState()
{
    spriteState[2]++;
    if (spriteState[2] >= spriteState[0] * spriteState[1])
    {
        spriteState[2] = 0;   
    }
    float tmpXY[2] = {0, 0};
    tmpXY[1] = floor(spriteState[2] / spriteState[0]); // compute y
    tmpXY[0] = spriteState[2] - (spriteState[0] * tmpXY[1]); // compute x
    setTexturePosition(tmpXY[0], tmpXY[1]);
}
void animatedSprite2D::runAnimation(animator* animation)
{
    // Update function only updates once every 2ms so I wouldn't
    // take any chances with smaller values. Either way I assume it
    // would just auto cap to a 2ms update rate anyways.
    if (animation->animationSheet[2] < 2)
    {
        std::cout << "WARNING, DELAYS LESS THAT 2ms MAY CAUSE UNEXPECTED BEHAVIOR" << std::endl; // Why would you ever need a more than 500fps animation dumbass.
    }


    if(currentAnimation == nullptr || currentAnimation != animation)
    {
        currentAnimation = animation;
        spriteState[2] = currentAnimation->animationSheet[0] - 1;
        animation->animationIndex = 0;
    }
    else if (currentAnimation->internalTimer * 1000 > (float)currentAnimation->animationSheet[2])
    {
        if(currentAnimation->animationIndex > currentAnimation->animationSheet[1])
        {
            spriteState[2] = currentAnimation->animationSheet[0] - 1;
            animation->animationIndex = 0;
        }
        else
        {
            nextAnimationState();
            currentAnimation->internalTimer = 0.0f;
            ++animation->animationIndex;
        }
    }
    currentAnimation->internalTimer += context::deltaTime;
}

// TODO implement some sort of batched rendering later on.
// NOTE This method is a little bitch. Don't fall for it's simplicity
void animatedSprite2D::drawSprite()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    unsigned int textureOffsetLocation = glGetUniformLocation(shader, "texCoordOffset");
    glUniform2fv(textureOffsetLocation, 1, glm::value_ptr(glm::vec2(texCoordOffset[0], texCoordOffset[1])));
    unsigned int transLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(Transform.genTransformationIdentityMatrix())); // This line of code is less efficient than the one in sprite2D's version of draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Static Sprite 2D implementation
// You may treat the staticSprite2D constructor as a static constructor (So while not required only one instance of staticSprite2D needs to exist and you can just keep recalling the constructor)
staticSprite2D::staticSprite2D(const char* pathToSprite, glm::vec2 vertexOffset)
{
    int width, height;
    addImageData(pathToSprite, width, height);

    float horizontalDistance = static_cast<float>(width);
    float verticalDistance = static_cast<float>(height);

    const float verticies[12] {
        horizontalDistance + vertexOffset.x, verticalDistance + vertexOffset.y, 0.0f, // Top right
        horizontalDistance + vertexOffset.x, -verticalDistance + vertexOffset.y, 0.0f, // Bottom right
        -horizontalDistance + vertexOffset.x, -verticalDistance + vertexOffset.y, 0.0f, // Bottom left
        -horizontalDistance + vertexOffset.x, verticalDistance + vertexOffset.y, 0.0f // Top left
    };

    if(combinedIndiciesData.size() % 6 != 0)
    {
        std::cout << "CRITICAL ERROR: UNSIGNED INDICIE DATA IS NOT PROPERLY CONFIGURED IN STATIC VECTOR" << std::endl;
        return;
    }
    unsigned int combinedIndiciesOffset = (combinedIndiciesData.size() / 6) * 4; // Index data will always be divisible by 6 because of safety check above.
    const unsigned int indicies[6] {
        2 + combinedIndiciesOffset, 0 + combinedIndiciesOffset, 3 + combinedIndiciesOffset,
        0 + combinedIndiciesOffset, 2 + combinedIndiciesOffset, 1 + combinedIndiciesOffset
    };

    combinedIndiciesData.insert(combinedIndiciesData.end(), std::begin(indicies), std::end(indicies));
    combinedVertexData.insert(combinedVertexData.end(), std::begin(verticies), std::end(verticies));
}

// Uses the skyline algorithm to pack the atlases in a fancy way.
// NOTE: This is my own implementation because the college professor was too much of a fucking nerd to teach it to me.
void staticSprite2D::genTextureAtlas(int texAtlasWidth, int texAtlasHeight)
{
    std::vector<texCoordMetadata> atlasTextureMetadata;
    atlasTextureMetadata.reserve(sprite::textureAtlasData.size());
    std::vector<heightData> heightMap;
    heightMap.reserve(20);
    heightMap.emplace_back(heightData(0, texAtlasWidth));
    std::vector<imageData>& images = sprite::textureAtlasData;
    int summedWidth = 0;

    for(const imageData& image : images)
    {
        int i = 0;
        bool foundOptimalSpot = false;
        summedWidth = 0;
        for(heightData& heightMapData : heightMap)
        {
            if(heightMapData.stride >= image.width)
            {
                foundOptimalSpot = true;
                atlasTextureMetadata.emplace_back(texCoordMetadata(summedWidth, heightMapData.height, image.width, image.height));
                heightMap[i].stride -= image.width;
                heightMap.insert(heightMap.begin() + i, heightData(heightMapData.height + image.height, image.width));
                break;
            }
            summedWidth += heightMapData.stride;
            ++i;
        }
        if(!foundOptimalSpot) // We now know for sure there is no possible spot we can place the image such that there is no fragmentation. Now we need to find how many images we have to sacrifice.
        {
            summedWidth = 0;
            int tallestHeight = 0;
            int j = 0;
            for (heightData& heightMapData : heightMap)
            {
                summedWidth += heightMapData.stride;
                if (tallestHeight < heightMapData.height) tallestHeight = heightMapData.height;
                if(summedWidth >= image.width)
                {
                    for(int k = 0; k < j - 1; ++k) // Removes all of the sacrificed heightmap strides in place of the new one (though we only need to modify the last one)
                    {
                        heightMap.erase(heightMap.begin());
                    }
                    summedWidth -= heightMap[0].stride;
                    heightMap[0].stride -= (image.width - summedWidth); // adjusts the heightmap to be the proper stride
                    atlasTextureMetadata.emplace_back(texCoordMetadata(0, tallestHeight, image.width, image.height));
                    heightMap.insert(heightMap.begin(), heightData(tallestHeight + image.height, image.width));
                    break;
                }
                ++j;
            }
        }

        // Implementation of coalescing
        for(int n = 1; n < heightMap.size(); ++n)
        {
            if(heightMap[n].height == heightMap[n - 1].height)
            {
                heightMap[n - 1].stride += heightMap[n].stride;
                heightMap.erase(heightMap.begin() + n);
            }
        }
    }
    
    int fitWidth, fitHeight;

    buildTextureAtlas(atlasTextureMetadata, fitWidth, fitHeight); // This method will use the data found in the logic here to generate image data in the form of a texture atlas.
    releaseAllImageData();

    // Now we can use the metadata to assign texcoord data
    for(auto& metadata : atlasTextureMetadata)
    {
        float startX = (float)metadata.xPos / (float)fitWidth; // Calculate start x-coord
        float endX = (float)(metadata.xPos + metadata.width) / (float)fitWidth; // Calculate end x-coord

        float startY = (float)metadata.yPos / (float)fitHeight; // Calculate start y-coord
        float endY = (float)(metadata.yPos + metadata.height) / (float)fitHeight; // Calculate end y-coord
        
        float texCoord[8] {
            endX, endY,
            endX, startY,
            startX, startY,
            startX, endY
        };
        combinedTexCoordData.insert(combinedTexCoordData.end(), std::begin(texCoord), std::end(texCoord));
    }

    // Finally we can create a working spirte by combining VAOs
    generateStaticVAO();
}

// This method essentially uses memcpy to copy the memory of the images all into one bid data sheet where we can then
// use stbi_image_write to write a png (Or later any image type) onto this.
void staticSprite2D::buildTextureAtlas(std::vector<texCoordMetadata>& xyPos, int& width, int& height)
{
    // Because of xyPos we can get the best possible width and height by shrinking to fit
    int atlasWidth = 0;
    int atlasHeight = 0;
    for(const texCoordMetadata& metadata : xyPos)
    {
        // Use a linear search algorithm to find the values with the largest value when calculating xPos + width and yPos + height.
        if (metadata.width + metadata.xPos > atlasWidth) atlasWidth = metadata.width + metadata.xPos;
        if (metadata.height + metadata.yPos > atlasHeight) atlasHeight = metadata.height + metadata.yPos;
    }

    // Allocate the char data assuming nrChannels of size 4 (for now).
    unsigned char* atlasData = new unsigned char[atlasWidth * atlasHeight * 4];
    memset(atlasData, 0, atlasWidth * atlasHeight * 4); // fill the image with transparency data
    // Iterate through every single height value and assign the proper width.
    for(int i = 0; i < atlasHeight; ++i)
    {
        int j = 0;
        for(const texCoordMetadata metadata : xyPos)
        {
            // This means we are within the range so we can use memcpy to copy data to the atlasData.
            if(i >= metadata.yPos && i < metadata.yPos + metadata.height)
            {
                memcpy(atlasData + (atlasWidth * i + metadata.xPos) * 4, sprite::textureAtlasData[j].data + (metadata.width * (i - metadata.yPos)) * 4, metadata.width * 4); // NOTE multiply the atlasWidth with height for the right offset.
            }
            ++j;
        }
    }

    width = atlasWidth;
    height = atlasHeight;

    atlasImageData = imageData(atlasWidth, atlasHeight, 4, atlasData);

    stbi_write_png("TextureAtlas.png", atlasWidth, atlasHeight, 4, atlasData, atlasWidth * 4); // We're doing this just for visual purposes (To make sure everything is working)
}

// Just a quick helper method to free all of the image data in the imageData array
void staticSprite2D::releaseAllImageData()
{
    for(imageData& dataToRemove : sprite::textureAtlasData)
    {
        stbi_image_free(dataToRemove.data);
    }
    sprite::textureAtlasData.clear();
}   

// Adds a single piece of image data to the static image data array.
void staticSprite2D::addImageData(const char* pathToImage, int& width, int& height)
{
    int nrChannels;
    unsigned char* data = stbi_load(pathToImage, &width, &height, &nrChannels, STBI_rgb_alpha);
    if(!data)
    {
        std::cout << "WARNING: FAILED TO ADD IMAGE DATA" << std::endl;
        return;
    }
    sprite::textureAtlasData.emplace_back(imageData(width, height, nrChannels, data));
    // We don't release the data until it is properly used (Which is why this method should really only be used during code setup)
}

// This method adds a single static image, but you still need to generate the atlas and stuff.
void staticSprite2D::generateStaticVAO()
{
    // First we have to interlace the combined vertex data with the combined texcoord data.
    int j = 0;
    for(int i = 3; i <= combinedVertexData.size(); i += 3)
    {
        combinedVertexData.insert(combinedVertexData.begin() + i, combinedTexCoordData.begin() + j, combinedTexCoordData.begin() + j + 2);
        i += 2;
        j += 2;
    }

    // Generate buffers and the VAO (and bind them)
    glGenVertexArrays(1, &staticVAO);
    glBindVertexArray(staticVAO);

    glGenBuffers(1, &staticVBO);
    glGenBuffers(1, &staticEBO);

    glBindBuffer(GL_ARRAY_BUFFER, staticVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, staticEBO);

    // Do some texture stuff
    glGenTextures(1, &staticTexture);
    glBindTexture(GL_TEXTURE_2D, staticTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasImageData.width, atlasImageData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasImageData.data);
    delete[] atlasImageData.data;
    glGenerateMipmap(GL_TEXTURE_2D);

    // Finally we can generate the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * combinedVertexData.size(), combinedVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // And now the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, combinedIndiciesData.size() * sizeof(int), combinedIndiciesData.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void staticSprite2D::drawStaticSprites(unsigned int shader)
{
    glBindTexture(GL_TEXTURE_2D, staticTexture);
    glUseProgram(shader);
    unsigned int textureOffsetLocation = glGetUniformLocation(shader, "texCoordOffset");
    glUniform2fv(textureOffsetLocation, 1, glm::value_ptr(glm::vec2(0.0f, 0.0f)));
    unsigned int transLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
    glBindVertexArray(staticVAO);
    glDrawElements(GL_TRIANGLES, combinedIndiciesData.size(), GL_UNSIGNED_INT, 0);
}

// Lots of static data
std::vector<imageData> sprite::textureAtlasData = {};

std::vector<float> staticSprite2D::combinedVertexData = {};
std::vector<float> staticSprite2D::combinedTexCoordData = {};
std::vector<unsigned int> staticSprite2D::combinedIndiciesData = {};

imageData staticSprite2D::atlasImageData = imageData();

unsigned int staticSprite2D::staticVBO = 0;
unsigned int staticSprite2D::staticVAO = 0;
unsigned int staticSprite2D::staticEBO = 0;
unsigned int staticSprite2D::staticTexture = 0;
unsigned int staticSprite2D::textureAtlasBuffer = 0;