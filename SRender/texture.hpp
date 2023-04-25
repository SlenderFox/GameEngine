#pragma once
#include <string>
#include <map>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** The texture class is an encapsulation for a texture on the gpu  |
	 * Textures are stored in key-value pairs where the key is from 0-31 correlating to
	 * the GL texture objects on the gpu. Each texture also has an id which is given by
	 * openGL and used for the deletion of the texture, annoyingly this number starts at 1.
	*/
	struct texture
	{
		/** Each texture has a different type for how it is used */
		enum class type: uint8_t
		{
			diffuse,
			specular
		};

		/** Deletes all loaded textures */
		static void init() noexcept;
		static void terminate() noexcept;
		static texture *loadNew(std::string _path, type _type = type::diffuse);
		_NODISCARD static uint8_t size() noexcept;
		_NODISCARD static texture *at(uint8_t _val);

	private:
		bool m_loaded = false;
		/** The id is the openGL supplied identifier used for deleting the texture */
		uint32_t m_id = 0U;
		/** This is the same as the key-value pair and is actually which texture this on the gpu */
		uint8_t m_index = 0U;
		/** Where the source of the image is located, used to prevent duplication */
		std::string m_filePath;
		/** The type of texture is used for how the texture is loaded into the shader */
		type m_type = type::diffuse;

		texture(uint8_t _index) noexcept;

	public:
		void load(std::string _filePath, type _type = type::diffuse);
		void destroy() noexcept;

		_NODISCARD bool getLoaded() const noexcept;
		_NODISCARD int32_t getLocation() const noexcept;
		_NODISCARD std::string getFilePath() const noexcept;
		_NODISCARD type getType() const noexcept;

		bool operator==(const uint8_t &_other) const noexcept;
		bool operator!=(const uint8_t &_other) const noexcept;
		bool operator==(const std::string &_other) const noexcept;
		bool operator!=(const std::string &_other) const noexcept;
	};
}
