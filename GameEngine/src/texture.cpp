#pragma region
#include "texture.hpp"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "debug.hpp"

using std::string;
using std::vector;
#pragma endregion

namespace engine
{
	// Forward declaration
	class application { public: _NODISCARD static const bool gladLoaded() noexcept; };

	// Static

	uint32_t texture::s_textureIds[32];
	uint32_t texture::s_textureCount = 0;
	vector<texture*> texture::s_loadedTextures = vector<texture*>();

	void texture::unloadAll() noexcept
	{
		if (application::gladLoaded())
		{ glDeleteTextures(s_textureCount, s_textureIds); }
	}

	int32_t texture::loadTextureFromFile(const string *inPath) noexcept
	{
		debug::send(
			"Loading texture " + std::to_string(s_textureCount) + ": \"" + *inPath + "\"...",
			debug::type::Process,
			debug::impact::Large,
			debug::stage::Mid,
			false,
			false
		);

		if (s_textureCount > 31)
		{
			debug::send(
				"Failed to load texture: Exceeded max texture count (max = 32)",
				debug::type::Note,
				debug::impact::Large,
				debug::stage::Mid,
				true
			);
			return -1;
		}

		// Makes sure the images are oriented correctly when loaded
		stbi_set_flip_vertically_on_load(true);

		int texWidth = 0, texHeight = 0, numComponents = 0;
		unsigned char *imageData = stbi_load(inPath->c_str(), &texWidth, &texHeight, &numComponents, 0);
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
				debug::send(
					"Failed to load texture: Too many components",
					debug::type::Note,
					debug::impact::Large,
					debug::stage::Mid,
					true
				);
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

			debug::send("Success!");

			// Returns the id before incrementing for the next texture
			return s_textureCount++;
		}
		else
		{
			debug::send(
				"Failed to load texture: No file found",
				debug::type::Note,
				debug::impact::Large,
				debug::stage::Mid,
				true
			);
			return -1;
		}
	}

	constexpr uint32_t texture::getTexCount() noexcept
	{ return s_textureCount; }

	// Member

	texture::texture(
		string inPath,
		texType inType
	) noexcept
		: m_file(inPath)
		, m_type(inType)
	{
		m_id = loadTextureFromFile(&m_file);
	}

	//void texture::destroy()
	//{
	//	// This is currently bad as it leaves a gap in the loaded textures
	//	glDeleteTextures(1, &s_textureIds[m_id]);
	//}

	int32_t texture::getId() const noexcept
	{ return m_id; }

	texture::texType texture::getType() const noexcept
	{ return m_type; }

	std::string texture::getFile() const noexcept
	{ return m_file; }
}
