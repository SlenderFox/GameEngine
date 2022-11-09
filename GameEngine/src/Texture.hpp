#pragma once
#include <string>
#include <vector>

namespace Engine
{
	enum class TexType: uint8_t
	{
		diffuse,
		specular
	};

	class Texture
	{
	private:
		static uint32_t s_textureIds[32];	// List of all texture ids
		static uint32_t s_textureCount;		// How many textures have been loaded

	public:
		static std::vector<Texture*> s_loadedTextures;	// Memory handled by renderer

		static void UnloadAll() noexcept;
		static int32_t LoadTextureFromFile(const std::string* pPath) noexcept;
		constexpr static uint32_t GetNumTex() noexcept { return s_textureCount; }

	private:
		uint32_t m_id = 0;
		TexType m_type = TexType::diffuse;
		std::string m_file;

	public:
		Texture(std::string pPath, TexType pType = TexType::diffuse);
		//void Destroy();

		constexpr uint32_t GetId() const noexcept { return m_id; }
		constexpr TexType GetType() const noexcept { return m_type; }
		constexpr std::string GetFile() const noexcept { return m_file; }
	};
}
