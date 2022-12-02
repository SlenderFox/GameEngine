#pragma once
#include <string>
#include <vector>

namespace Engine
{
	/** The texture object stores a reference to a texture on the gpu */
	struct Texture
	{
		enum class TexType: uint8_t
		{
			Diffuse,
			Specular
		};

	private:
		// List of all texture ids
		static uint32_t s_textureIds[32];
		// How many textures have been loaded
		static uint32_t s_textureCount;

		int32_t m_id = 0;
		TexType m_type = TexType::Diffuse;
		std::string m_file;

	public:
		// Memory handled by renderer
		static std::vector<Texture*> s_loadedTextures;

		static void UnloadAll() noexcept;
		static int32_t LoadTextureFromFile(const std::string *inPath) noexcept;

		constexpr static
		uint32_t GetTexCount() noexcept;

		Texture(
			std::string inPath,
			TexType inType = TexType::Diffuse
		) noexcept;

		//void Destroy();

		_NODISCARD int32_t GetId() const noexcept;
		_NODISCARD TexType GetType() const noexcept;
		_NODISCARD std::string GetFile() const noexcept;
	};
}
