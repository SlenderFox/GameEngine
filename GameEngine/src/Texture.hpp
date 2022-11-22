#pragma once
#include <string>
#include <vector>

namespace Engine
{
	struct Texture
	{
		enum class TexType: uint8_t
		{
			Diffuse,
			Specular
		};

		static std::vector<Texture*> s_loadedTextures;	// Memory handled by renderer

		static void UnloadAll() noexcept;
		static int32_t LoadTextureFromFile(const std::string* pPath) noexcept;
		constexpr static uint32_t GetNumTex() noexcept { return s_textureCount; }

		Texture(std::string pPath, TexType pType = TexType::Diffuse);
		//void Destroy();

		constexpr uint32_t GetId() const noexcept { return m_id; }
		constexpr TexType GetType() const noexcept { return m_type; }
		constexpr std::string GetFile() const noexcept { return m_file; }

	private:
		static uint32_t s_textureIds[32];	// List of all texture ids
		static uint32_t s_textureCount;		// How many textures have been loaded

		uint32_t m_id = 0;
		TexType m_type = TexType::Diffuse;
		std::string m_file;
	};
}
