#pragma region 
#pragma once
#include <string>

using std::string;
#pragma endregion

namespace Engine
{
	class Texture
	{
	public:
		Texture();
		Texture(string pLocation);
		void Destroy(bool pValidate);
		/**
		 * @brief Currently loads a set of hard-coded images from file
		 */
		void LoadImages();
		
		bool GetLoaded() const { return m_textureLoaded; }

		unsigned int m_idTEX0 = 0U, m_idTEX1 = 0U, m_idTEX2 = 0U;
		//unsigned int m_idTex = 0U;

	private:
		/**
		 * @brief Loads an image from a file into vram (I'm assuming) as a texture
		 *
		 * @param texID Which id to load the texture into
		 * @param location Where the image is stored
		 * @param transparent Whether the image has an alpha channel
		 */
		static void LoadTexture(unsigned int* pTexID, const char* pLocation, bool pTransparent);

		//uint32_t BitMap;

		bool m_textureLoaded = false;
	};
}
