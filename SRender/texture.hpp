#pragma once
#include <string>
#include <vector>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** The texture class is an encapsulation for a texture on the gpu
	 * @todo Overhaul whole class
	 * @todo Allow destroying of a texture so that another one can replace it
	*/
	struct texture
	{
		enum class type: uint8_t
		{
			diffuse,
			specular
		};

	private:
		int32_t m_id = 0;
		std::string m_file;
		type m_type = type::diffuse;

	public:
		/** TODO: This is a mess, the vector is only ever accessed
		 * from model and is then cleaned up by renderer */
		static std::vector<texture*> s_loadedTextures;

		static void deleteAll() noexcept;
		_NODISCARD static uint32_t getTexCount() noexcept;

		_NODISCARD int32_t getId() const noexcept;
		_NODISCARD type getType() const noexcept;
		_NODISCARD std::string getFile() const noexcept;

		texture(
			std::string _path,
			type _type = type::diffuse
		);

		bool operator==(const int32_t &_other) const noexcept;
		bool operator!=(const int32_t &_other) const noexcept;
		bool operator==(const std::string &_other) const noexcept;
		bool operator!=(const std::string &_other) const noexcept;
	};
}
