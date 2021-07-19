#pragma once
#define STB_IMAGE_IMPLEMENTATION

namespace Engine
{
	class Texture
	{
	public:
		void LoadImages();
		unsigned int m_idTEX0 = 0, m_idTEX1 = 0;

	private:
		int m_texWidth = 0, m_texHeight = 0, m_texColChannels = 0;
		unsigned char* m_imageData = 0;
	};
}
