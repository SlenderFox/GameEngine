#pragma once

namespace Engine
{
	class Texture
	{
	public:
		/**
		 * @brief Currently loads a set of hard-coded images from file
		 */
		void LoadImages();
		unsigned int m_idTEX0 = 0, m_idTEX1 = 0, m_idTEX2 = 0;

	private:
		/**
		 * @brief Loads an image from a file into vram (I'm assuming) as a texture
		 *
		 * @param texID Which id to load the texture into
		 * @param location Where the image is stored
		 * @param transparent Whether the image has an alpha channel
		 */
		void LoadTexture(unsigned int* texID, const char* location, bool transparent);

		int m_texWidth = 0, m_texHeight = 0, m_texColChannels = 0;
		unsigned char* m_imageData = 0;
	};
}
