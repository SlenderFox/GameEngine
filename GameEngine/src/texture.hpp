#pragma once
#include <string>
#include <vector>

namespace engine
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
		texType m_type = texType::diffuse;
		std::string m_file;

	public:
		// Memory handled by renderer
		static std::vector<texture*> s_loadedTextures;

		static void unloadAll() noexcept;
		static int32_t loadTextureFromFile(const std::string *inPath) noexcept;

		constexpr static
		uint32_t getTexCount() noexcept;

		texture(
			std::string inPath,
			texType inType = texType::diffuse
		) noexcept;

		//void destroy();

		_NODISCARD int32_t getId() const noexcept;
		_NODISCARD texType getType() const noexcept;
		_NODISCARD std::string getFile() const noexcept;
	};
}
