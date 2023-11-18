#pragma once
#include <stdexcept>

namespace srender
{
/** Exceptions for the texture class
 * https://isocpp.org/wiki/faq/exceptions#ctors-can-throw
 */
class textureException : public std::runtime_error
{
public:
	textureException(std::string _description) noexcept : std::runtime_error(_description) {}
};

class graphicsException : public std::runtime_error
{
public:
	graphicsException(std::string _description) noexcept : std::runtime_error(_description) {}
};
}
