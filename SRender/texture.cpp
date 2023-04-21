#include "texture.hpp"
#include "renderer.hpp"
#include "exception.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "debug.hpp"

using std::string;
using std::vector;

namespace srender
{
	// Static

	/** List of all texture ids */
	uint32_t s_textureIds[32];
	/** How many textures have been loaded */
	uint32_t s_textureCount = 0;
	vector<texture*> texture::s_loadedTextures = vector<texture*>();

	void texture::deleteAll() noexcept
	{	renderer::deleteTextures(s_textureCount, s_textureIds); }

	uint32_t texture::getTexCount() noexcept
	{	return s_textureCount; }

	// Member

	int32_t texture::getId() const noexcept
	{	return m_id; }

	texture::type texture::getType() const noexcept
	{	return m_type; }

	std::string texture::getFile() const noexcept
	{	return m_file; }

	texture::texture(
		string _path,
		type _type
	)
		: m_file(_path)
		, m_type(_type)
	{
		debug::send(
			"Loading texture " + std::to_string(s_textureCount) + ": \"" + m_file + "\"...",
			debug::type::process, debug::impact::large, debug::stage::mid, false, false
		);

		if (s_textureCount > 31)
		{
			debug::send(
				"Failed to load texture: Exceeded max texture count (max = 32)",
				debug::type::note, debug::impact::large, debug::stage::mid, true
			);

			throw textureException("Exceeded max textures");
		}

		// Makes sure the images are oriented correctly when loaded
		stbi_set_flip_vertically_on_load(true);

		int texWidth = 0, texHeight = 0, numComponents = 0;
		uint8_t *imageData = stbi_load(
			m_file.c_str(),
			&texWidth,
			&texHeight,
			&numComponents,
			0
		);

		if (!imageData)
		{
			debug::send(
				"Failed to load texture: No file found",
				debug::type::note, debug::impact::large, debug::stage::mid, true
			);

			throw textureException("No file found");
		}

		// Generates a texture object in vram
		renderer::setActiveTexture(s_textureCount);
		renderer::genTexture(&s_textureIds[s_textureCount]);
		// Remember this works like a pointer to the object using the ID
		renderer::bindTexture2D(s_textureIds[s_textureCount]);
		// Sets some parameters to the currently bound texture object
		//float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		//renderer::setBorderColour(borderColour);
		renderer::setTex2DParamSWrapToEdge();
		renderer::setTex2DParamTWrapTOBorder();
		renderer::setTex2DParamMinFilterLinearMipMapLinear();
		renderer::setTex2DParamMagFilterNearest();

		renderer::loadTexture(texWidth, texHeight, numComponents, imageData);
		renderer::genMipmap();

		// Frees the image memory
		stbi_image_free(imageData);

		// Sets the id then increments, ready for the next texture
		m_id = s_textureCount++;

		s_loadedTextures.push_back(this);

		debug::send("Success!");
	}

	bool texture::operator==(const int32_t &_other) const noexcept
	{	return m_id == _other; }

	bool texture::operator!=(const int32_t &_other) const noexcept
	{	return m_id != _other; }

	bool texture::operator==(const string &_other) const noexcept
	{	return m_file == _other; }

	bool texture::operator!=(const string &_other) const noexcept
	{	return m_file != _other; }
}
