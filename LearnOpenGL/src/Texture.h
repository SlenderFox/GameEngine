#pragma once
#define STB_IMAGE_IMPLEMENTATION

namespace Engine
{
	class Texture
	{
	public:
		Texture();
		~Texture() {}

		void LoadImages();

	private:
		unsigned int m_idTEX0, m_idTEX1;
		int m_texWidth, m_texHeight, m_texColChannels;
		unsigned char* m_imageData;
	};
}
