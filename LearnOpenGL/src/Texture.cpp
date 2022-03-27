#pragma region
#include "Texture.hpp"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif
#pragma endregion

namespace Engine
{
	Texture::Texture()
	{
		// Makes sure the images are oriented correctly when loaded
		stbi_set_flip_vertically_on_load(true);
	}

	Texture::Texture(string pLocation)
	{
		// Makes sure the images are oriented correctly when loaded
		stbi_set_flip_vertically_on_load(true);

		//glGenTextures(1, &m_idTex);
	}

	void Texture::Destroy(bool pValidate)
	{
		if (pValidate)
		{
			glDeleteTextures(1, &m_idTEX0);
			glDeleteTextures(1, &m_idTEX1);
			//glDeleteTextures(1, &m_idTEX2);
			
			//glDeleteTextures(1, &m_idTex);
		}
	}
	
	void Texture::LoadImages()
	{
		glActiveTexture(GL_TEXTURE0);
		m_idTEX0 = LoadTexture("../Assets/textures/container2.png");
		glActiveTexture(GL_TEXTURE1);
		m_idTEX1 = LoadTexture("../Assets/textures/container2_specular.png");
		//glActiveTexture(GL_TEXTURE2);
		//LoadTexture(&m_idTEX2, "../Assets/textures/awesomeface.png", true);

		//m_textureLoaded = true;
	}

	uint8_t Texture::LoadTexture(const char *pPath)
	{
		float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		unsigned int texID;
		// Generates a texture object in vram
		glGenTextures(1, &texID);
		#ifdef _DEBUG
		 if (texID > UINT8_MAX)
		 {
			cout << "Texture ID has exceeded UINT8_MAX: " << texID << " for file path: " << pPath << endl;
		 	return UINT8_MAX;
		 }
		#endif
		// Remember this works like a pointer to the object using the ID
		glBindTexture(GL_TEXTURE_2D, texID);
		// Sets some parameters to the currently bound texture object
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int texWidth = 0, texHeight = 0, numComponents = 0;
		unsigned char* imageData = stbi_load(pPath, &texWidth, &texHeight, &numComponents, 0);
		if (imageData)
		{
			GLenum format;
			switch (numComponents)
			{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
				default: cout << "Failed to load texture " << pPath << endl; return UINT8_MAX;
			}

			/*Applies the image to the texture object and creates the mipmaps
			* p1: What object we are applying to
			* p2: Specifies which mipmap level we are applying to (0 for base)
			* p3: What format to store the texture as
			* p4/5: The width and height of the texture
			* p6: Border (legacy stuff, leave as 0)
			* p7: What format the image is
			* p8: The datatype being passed in (in this case a char)
			* p9: The image data being passed in
			*/
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Frees the image memory
			stbi_image_free(imageData);
			return (uint8_t)texID;
		}
		#ifdef _DEBUG
		 else
		 {
		 	cout << "Failed to load texture " << pPath << endl;
			return UINT8_MAX;
		 }
		#endif
	}
}
