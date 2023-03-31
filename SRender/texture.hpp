#pragma once
#include <string>
#include <vector>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** The texture object stores a reference to a texture on the gpu */
	struct texture
	{
		enum class texType: uint8_t
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
		texType m_type = texType::diffuse;

	public:
		// Memory handled by renderer
		static std::vector<texture*> s_loadedTextures;

		static void unloadAll() noexcept;
		_NODISCARD static int32_t loadTextureFromFile(const std::string *_path) noexcept;

		_NODISCARD static uint32_t getTexCount() noexcept;

		texture(
			std::string _path,
			texType _type = texType::diffuse
		) noexcept;

		//void destroy();

		_NODISCARD int32_t getId() const noexcept;
		_NODISCARD texType getType() const noexcept;
		_NODISCARD std::string getFile() const noexcept;
	};
}
