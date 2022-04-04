#pragma region 
#pragma once
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <string>

using std::string;
#pragma endregion

namespace Engine
{
	enum class TexType : uint8_t
	{
		diffuse,
		specular
	};

	class Texture
	{
	public:
		static void UnloadAll(bool pValidate);
		static unsigned int GetNumTex();

		Texture(const char* pPath, TexType pType);
		void Destroy();

		unsigned int GetId() const;
		string GetType() const;
		
	private:
		/**
		 * @brief Loads an image from a file into vram (I'm assuming) as a texture
		 * 
		 * @param pPath The location of the image file
		 * @return uint8_t The ID for the texture (max 32 textures so this will be more than enough)
		 */
		static uint8_t LoadTexture(const char* pPath);

		static unsigned int s_idTex[32];	// List of all texture ids
		static unsigned int s_numTex;		// How many textures have been loaded

		unsigned int m_id;
		TexType m_type;
	};
}
