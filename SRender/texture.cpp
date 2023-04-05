#include "texture.hpp"
#include "graphics.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "debug.hpp"

using std::string;
using std::vector;

// TODO: Overhaul

namespace srender
{
	// Static

	uint32_t texture::s_textureIds[32];
	uint32_t texture::s_textureCount = 0;
	vector<texture*> texture::s_loadedTextures = vector<texture*>();

	void texture::deleteAll() noexcept
	{	graphics::deleteTextures(s_textureCount, s_textureIds); }

	int32_t texture::loadTextureFromFile(const string *_path) noexcept
	{
		debug::send(
			"Loading texture " + std::to_string(s_textureCount) + ": \"" + *_path + "\"...",
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
		unsigned char *imageData = stbi_load(
			_path->c_str(),
			&texWidth,
			&texHeight,
			&numComponents,
			0
		);

		if (!imageData)
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

		// Generates a texture object in vram
		graphics::setActiveTexture(s_textureCount);
		graphics::genTexture(&s_textureIds[s_textureCount]);
		// Remember this works like a pointer to the object using the ID
		graphics::bindTexture2D(s_textureIds[s_textureCount]);
		// Sets some parameters to the currently bound texture object
		//float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		//graphics::setBorderColour(borderColour);
		graphics::setTex2DParamSWrapToEdge();
		graphics::setTex2DParamTWrapTOBorder();
		graphics::setTex2DParamMinFilterLinearMipMapLinear();
		graphics::setTex2DParamMagFilterNearest();

		graphics::loadTexture(texWidth, texHeight, numComponents, imageData);
		graphics::genMipmap();

		// Frees the image memory
		stbi_image_free(imageData);

		debug::send("Success!");

		// Returns the id before incrementing for the next texture
		return s_textureCount++;
	}

	uint32_t texture::getTexCount() noexcept
	{	return s_textureCount; }

	// Member

	texture::texture(
		string _path,
		texType _type
	) noexcept
		: m_file(_path)
		, m_type(_type)
	{
		m_id = loadTextureFromFile(&m_file);
	}

	int32_t texture::getId() const noexcept
	{	return m_id; }

	texture::texType texture::getType() const noexcept
	{	return m_type; }

	std::string texture::getFile() const noexcept
	{	return m_file; }
}
