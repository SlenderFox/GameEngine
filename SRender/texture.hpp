#pragma once
#include <string>
#include <vector>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** The texture object stores a reference to a texture on the gpu
	 * @todo Overhaul whole class
	 * @todo Allow destroying of a texture so that another one can replace it
	*/
	struct texture
	{
		enum class type: uint8_t
		{
			diffuse,
			specular
		};

	private:
		// List of all texture ids
		static uint32_t s_textureIds[32];
		// How many textures have been loaded
		static uint32_t s_textureCount;

		int32_t m_id = 0;
		std::string m_file;
		type m_type = type::diffuse;

	public:
		// Memory handled by renderer
		static std::vector<texture*> s_loadedTextures;

		static void deleteAll() noexcept;
		_NODISCARD static int32_t loadTextureFromFile(const std::string *_path) noexcept;
		_NODISCARD static uint32_t getTexCount() noexcept;

		_NODISCARD int32_t getId() const noexcept;
		_NODISCARD type getType() const noexcept;
		_NODISCARD std::string getFile() const noexcept;

		texture(
			std::string _path,
			type _type = type::diffuse
		) noexcept;

		bool operator==(const int32_t &_other) const noexcept;
		bool operator!=(const int32_t &_other) const noexcept;
		bool operator==(const std::string &_other) const noexcept;
		bool operator!=(const std::string &_other) const noexcept;
	};
}
