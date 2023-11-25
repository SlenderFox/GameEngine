#include "texture.hpp"
#include "renderer.hpp"
#include "exception.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "debug.hpp"

using std::string;
namespace srender
{
// Static
std::map<uint8_t, texture*> l_loadedTextures = std::map<uint8_t, texture*>();

void texture::init() noexcept
{
	// Makes sure the images are oriented correctly when loaded
	stbi_set_flip_vertically_on_load(true);

	// Create 32 textures
	for (uint8_t i = 0; i < 32; ++i)
	{	l_loadedTextures[i] = new texture(i); }
}

void texture::terminate() noexcept
{
	for (auto &[key, value] : l_loadedTextures)
	{	delete value; }
	l_loadedTextures.clear();
}

texture *texture::loadNew(string _path, type _type)
{
	// Look for a texture that has not been loaded into
	for (auto &[key, value] : l_loadedTextures)
	{
		if (!value->m_loaded)
		{
			try
			{	value->load(_path, _type); }
			// Pass exception down
			catch (textureException &e)
			{	throw e; }
			return value;
		}
	}
	throw textureException("Run out of textures");
}

uint8_t texture::size() noexcept
{	return (uint8_t)l_loadedTextures.size(); }

texture *texture::at(uint8_t _val)
{	return l_loadedTextures[_val]; }

// Member

texture::texture(uint8_t _index) noexcept : m_index(_index)
{}

bool texture::getLoaded() const noexcept
{	return m_loaded; }

int32_t texture::getLocation() const noexcept
{	return m_index; }

std::string texture::getFilePath() const noexcept
{	return m_filePath; }

texture::type texture::getType() const noexcept
{	return m_type; }

void texture::load(string _filePath, type _type)
{
	m_filePath = _filePath;
	m_type = _type;

	debug::send(
		"Loading texture " + std::to_string(l_loadedTextures.size()) + ": \"" + m_filePath + "\"...",
		debug::type::process, debug::impact::large, debug::stage::mid, false, false
	);

	int texWidth = 0, texHeight = 0, numComponents = 0;
	uint8_t *imageData = stbi_load(
		m_filePath.c_str(),
		&texWidth,
		&texHeight,
		&numComponents,
		0
	);

	if (!imageData)
	{
		debug::send(
			"Failed! No file found",
			debug::type::note, debug::impact::large, debug::stage::mid
		);

		throw textureException("No file found");
	}

	// Generates a texture object in vram
	renderer::setActiveTexture(m_index);
	renderer::genTexture(&m_id);
	// Remember this works like a pointer to the object using the ID
	renderer::bindTexture2D(m_id);

	// Sets some parameters to the currently bound texture object
	renderer::setTex2DParamSWrapToEdge();
	renderer::setTex2DParamTWrapTOBorder();
	renderer::setTex2DParamMinFilterLinearMipMapLinear();
	renderer::setTex2DParamMagFilterNearest();
	//float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//renderer::setBorderColour(borderColour);

	renderer::loadTexture(texWidth, texHeight, numComponents, imageData);
	renderer::genMipmap();

	// Frees the image memory
	stbi_image_free(imageData);

	m_loaded = true;
	debug::send("Success!");
}

void texture::destroy() noexcept
{
	m_loaded = false;
	renderer::deleteTextures(&m_id);
}

bool texture::operator==(const uint8_t &_other) const noexcept
{	return m_index == _other; }

bool texture::operator!=(const uint8_t &_other) const noexcept
{	return m_index != _other; }

bool texture::operator==(const string &_other) const noexcept
{	return m_filePath == _other; }

bool texture::operator!=(const string &_other) const noexcept
{	return m_filePath != _other; }
}
