#pragma once
#define STB_IMAGE_IMPLEMENTATION

namespace Engine
{
	class Texture
	{
	public:
		void LoadImages();
		unsigned int m_idTEX0 = 0, m_idTEX1 = 0, m_idTEX2 = 0;

	private:
		void LoadTexture(unsigned int* texID, const char* location, bool transparent);

		int m_texWidth = 0, m_texHeight = 0, m_texColChannels = 0;
		unsigned char* m_imageData = 0;
	};
}
