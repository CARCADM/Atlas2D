#include <objGen.h>

int objGen::generateVBO(unsigned int& VBO, int attribute, const std::vector<float> vertices, GLenum usage)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), usage);
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attribute);
    return 0;
}

int objGen::generateVBO(unsigned int& VBO, int attribute, const std::vector<float> vertices, GLenum usage, unsigned int texture)
{
    // Assumes texture attribute is one attribute over to prevent excessive use of parameters
    // and encourage users to write some resemblance of a readable shader structure.
    const int textureAttribute = attribute + 1;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), usage);
    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(textureAttribute);
    return 0;
}


void objGen::generateVAO(unsigned int& VAO)
{
    // NOTE do not change this to unbind the VAO or unholy things will happen to the program.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void objGen::generateEBO(unsigned int& EBO, std::vector<unsigned int> indicies, GLenum usage) // <-- Bane of my existience
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(int), indicies.data(), usage);
}

int objGen::generateTexture(unsigned int& texture2D, const char* texturePath, int& width, int& height)
{
    glGenTextures(1, &texture2D);
    glBindTexture(GL_TEXTURE_2D, texture2D);

    // We do this to prevent artifacting for textures with an alpha channel due to texture wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int nrChannels;
    // TODO allow for other color settings other than RGBA so we can suppourt other cool filetypes.
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if(!data)
    {
        std::cout << "Failed to load image data..." << std::endl;
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return 0;
}