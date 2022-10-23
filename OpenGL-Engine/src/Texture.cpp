#pragma region
#include "Texture.hpp"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#ifdef _DEBUG
 #include "Debug.hpp"
#endif

using std::string;
#pragma endregion

namespace Engine
{
	// Forward declaration
	class Application { public: static const bool GladLoaded() noexcept; };

	// Static

	uint32_t Texture::s_textureIds[32];
	uint16_t Texture::s_textureCount = 0;

	void Texture::UnloadAll() noexcept
	{
		if (Application::GladLoaded())
			glDeleteTextures(s_textureCount, s_textureIds);
	}

	int32_t Texture::LoadTextureFromFile(const std::string& pPath) noexcept
	{
#		ifdef _DEBUG
		 Debug::BigProcess("Loading texture " + std::to_string(s_textureCount) + ": \"" + pPath + "\"...", false, false);
#		endif

		if (s_textureCount > 31)
		{
#			ifdef _DEBUG
			 Debug::BigNote("Failed to load texture: Exceeded max texture count (max = 32)", true);
#			endif
			return -1;
		}

		// Makes sure the images are oriented correctly when loaded
		stbi_set_flip_vertically_on_load(true);

		int texWidth = 0, texHeight = 0, numComponents = 0;
		unsigned char* imageData = stbi_load(pPath.c_str(), &texWidth, &texHeight, &numComponents, 0);
		if (imageData)
		{
			float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			// Generates a texture object in vram
			glActiveTexture(GL_TEXTURE0 + s_textureCount);
			glGenTextures(1, &s_textureIds[s_textureCount]);
			// Remember this works like a pointer to the object using the ID
			glBindTexture(GL_TEXTURE_2D, s_textureIds[s_textureCount]);
			// Sets some parameters to the currently bound texture object
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			GLenum format;
			switch (numComponents)
			{
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			default: 
#				ifdef _DEBUG
				 Debug::BigNote("Failed to load texture: Too many components", true);
#				endif
				return -1;
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

#			ifdef _DEBUG
			 Debug::Send("Success!");
#			endif

			// Returns the id before incrementing for the next texture
			return s_textureCount++;
		}
		else
		{
#			ifdef _DEBUG
			 Debug::BigNote("Failed to load texture: No file found", true);
#			endif
			return -1;
		}
	}

	// Member

	Texture::Texture(const string& pPath, TexType pType): m_file(pPath), m_type(pType)
	{
		m_id = LoadTextureFromFile(pPath);
	}

	//void Texture::Destroy()
	//{
	//	// This is currently bad as it leaves a gap in the loaded textures
	//	glDeleteTextures(1, &s_textureIds[m_id]);
	//}
}
