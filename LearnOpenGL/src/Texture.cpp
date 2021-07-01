#include "Texture.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include "stb_image.h"
#include <iostream>

using std::cout;
using std::endl;

Texture::Texture() { }

Texture::~Texture() { }

void Texture::LoadImages()
{
    // ----------Texture----------
// Generates two texture objects
    unsigned int id_TEX0, id_TEX1;
    glGenTextures(1, &id_TEX0);
    glGenTextures(1, &id_TEX1);
    // Rmember this worlks like a pointer to the object using the ID
    glBindTexture(GL_TEXTURE_2D, id_TEX0);

    // Sets some parameters to the currently bound texture object
    float borderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Makes sure the images are oriented correctly when loaded
    stbi_set_flip_vertically_on_load(true);

    // Loads the first texture into memory
    int texWidth, textHeight, texColChannels;
    unsigned char* data = stbi_load("../Assets/textures/container.jpg", &texWidth, &textHeight, &texColChannels, 0);
    if (data)
    {
        /*Applies the image to the texture object and creates the mipmaps
        * p1: What object we are applying to
        * p2: Specifies which mipmap level we are applying to (0 for base)
        * p3: What format to store the texture as
        * p4/5: The width and height of the texture
        * p6: Border (legacy stuff, leave as 0)
        * p7: What format the image is
        * p8: The datatype being passed in (this this case a char)
        * p9: The actual image being passed in
        */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture0" << endl;
    }
    // Frees the image memory
    stbi_image_free(data);

    // Binds the second texture
    glBindTexture(GL_TEXTURE_2D, id_TEX1);
    // Sets some parameters to the currently bound texture object
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loads the seconds texture into memory
    data = stbi_load("../Assets/textures/awesomeface.png", &texWidth, &textHeight, &texColChannels, 0);
    if (data)
    {
        // Applies the image to the texture object and creates the mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture1" << endl;
    }
    // Frees the image memory
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_TEX0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_TEX1);
}
