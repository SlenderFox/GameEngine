#include <fstream>
#include <sstream>
#include "shader.hpp"
#include "default_shader.hpp"
#include "renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "debug.hpp"

using std::string;
using std::stringstream;
using std::ifstream;

namespace srender
{
shader::shader(const string *_shaderPath)
{	load(_shaderPath); }

shader::~shader()
{	destroy(); }

void shader::destroy() noexcept
{
	if (m_shaderLoaded)
	{
		renderer::deleteShaderProgram(m_idProgram);
		m_shaderLoaded = false;
	}
}

void shader::load(const std::string *_shaderPath)
{
	if (m_shaderLoaded)
	{
		debug::send("ERROR::SHADER::ATTEMPTING_TO_OVERLOAD_SHADER");
		return;
	}
	// If no path is given, will use fallback shader
	m_shaderPath = _shaderPath ? *_shaderPath : "";
	loadShader(shaderType::vertex);
	loadShader(shaderType::fragment);
	createShaderProgram();
}

void shader::use() const noexcept
{	renderer::useShaderProgram(m_idProgram); }

constexpr bool shader::isLoaded() const noexcept
{	return m_shaderLoaded; }

void shader::loadShader(const shaderType _type)
{
	bool m_usingFallback = false;
	string codeString;

	if (_type == shaderType::program)
	{
		debug::send(
			"ERROR::SHADER::LOADING_INCORRECT_SHADER_TYPE",
			debug::type::note, debug::impact::large, debug::stage::mid
		);
		return;
	}

	if (m_shaderPath == "")
	{
		m_usingFallback = true;
		debug::send(
			"SHADER::NO_PATH_PROVIDED::USING_FALLBACK_CODE",
			debug::type::note, debug::impact::large, debug::stage::mid
		);
	}
	else
	{
		string path = m_shaderPath + byType(_type, string(".vert"), string(".frag"));

		debug::send(
			"Compiling shader \"" + path + "\"...",
			debug::type::process, debug::impact::small, debug::stage::mid, false, false
		);

		// Try to retrieve the vertex/fragment source code from filePath
		try
		{
			ifstream fileStream;
			stringstream codeStream;
			// Ensure ifstream objects can throw exceptions
			fileStream.exceptions(ifstream::failbit | ifstream::badbit);

			fileStream.open(path);
			codeStream << fileStream.rdbuf();
			fileStream.close();

			// Convert stream into string
			codeString = codeStream.str();
		}
		catch (ifstream::failure &e)
		{
			string msg = "ERROR::SHADER::"
				+ byType(_type, string("VERTEX"), string("FRAGMENT"))
				+ "::FAILURE_TO_READ_FILE::USING_FALLBACK_CODE:";

			debug::send(msg, debug::type::note, debug::impact::large, debug::stage::mid, true);
			debug::send(
				"\"" + string(e.what()) + "\"",
				debug::type::note, debug::impact::small, debug::stage::mid
			);

			m_usingFallback = true;
		}
	}

	if (!m_usingFallback)
	{
		if (
			!compileShader(
				byType(_type, &m_idVertex, &m_idFragment),
				_type,
				codeString.c_str()
			)
		)
		{
			m_usingFallback = true;
		}
	}

	// Separated to allow bool to potentially change
	if (m_usingFallback)
	{
		debug::send(
			"Compiling fallback code...",
			debug::type::process, debug::impact::small, debug::stage::mid, false, false
		);

		bool result;
		{
			const char *vertexFallback = VERTEX_FALLBACK;
			const char *fragmentFallback = FRAGMENT_FALLBACK;

			result = compileShader(
				byType(_type, &m_idVertex, &m_idFragment),
				_type,
				byType<const char*&>(_type, vertexFallback, fragmentFallback)
			);
		}

		if (!result)
		{
			string msg = "ERROR::SHADER::"
				+ byType(_type, string("VERTEX"), string("FRAGMENT"))
				+ "::FALLBACK_CODE_FAILURE";
			debug::send(msg, debug::type::note, debug::impact::large, debug::stage::mid, true);

			exit(2);
		}
	}

	debug::send("Success!");
}

bool shader::compileShader(
	uint32_t *_id,
	shaderType _type,
	const char *_code
) noexcept
{
	assert(_type != shaderType::program && "Incorrect shaderType passed");

	// Creates a shader object and assigns to an id
	switch (_type)
	{
	case shaderType::vertex:
		*_id = renderer::createVertexShader();
		break;
	case shaderType::fragment:
		*_id = renderer::createFragmentShader();
		break;
	default:
		debug::send(
			"ERROR::SHADER::ATTEMPTING_TO_COMPILE_UNKNOWN_SHADER_TYPE",
			debug::type::note, debug::impact::large, debug::stage::mid, true
		);
		return false;
	}

	// Loads the shader code into the shader object
	renderer::loadShaderSource(*_id, _code);
	// Compiles the shader at run-time
	renderer::compileShader(*_id);
	// Performs error checking on the shader
	return checkForErrors(*_id, _type);
}

void shader::createShaderProgram() noexcept
{
	// Creates a shader program object assigned to id, this sets it as the active shader
	m_idProgram = renderer::createShaderProgram(m_idVertex, m_idFragment);
	// Performs error checking on the shader program
	if (!checkForErrors(m_idProgram, shaderType::program))
	{
		renderer::deleteShaderProgram(m_idProgram);
		return;
	}
	// We no longer need the vertex and fragment shaders
	renderer::deleteShader(m_idVertex);
	renderer::deleteShader(m_idFragment);
	// Sets the shader as the active one
	renderer::useShaderProgram(m_idProgram);
	m_shaderLoaded = true;
}

bool shader::checkForErrors(
	const uint32_t _shaderID,
	const shaderType _type
) const noexcept
{
	// Boolean output as int32
	int32_t success;
	char infoLog[512];

	if (_type == shaderType::program)
	{
		// Retrieves the compile status of the given shader by id
		renderer::getProgramiv(_shaderID, &success);
		if (!success)
		{
			// In the case of a failure it loads the log and outputs
			renderer::getProgramInfoLog(_shaderID, infoLog, 512);
			debug::send(
				"ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" + string(infoLog),
				debug::type::note, debug::impact::large, debug::stage::mid, true, false
			);
			return false;
		}
	}
	else
	{
		// Retrieves the compile status of the given shader by id
		renderer::getShaderiv(_shaderID, &success);
		if (!success)
		{
			// In the case of a failure it loads the log and outputs
			renderer::getShaderInfoLog(_shaderID, infoLog, 512);
			string msg = "ERROR::SHADER::"
				+ byType(_type, string("VERTEX"), string("FRAGMENT"))
				+ "::COMPILATION_FAILED:\n"
				+ string(infoLog);
			debug::send(msg, debug::type::note,debug::impact::large, debug::stage::mid, true, false);
			return false;
		}
	}
	return true;
}

template<typename T>
inline T shader::byType(
	const shaderType _type,
	T _vertex,
	T _fragment
) const noexcept
{
	assert(_type != shaderType::program && "Incorrect shaderType passed");
	return (_type == shaderType::vertex ? _vertex : _fragment);
}

void shader::setBool(string _name, const bool _value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setBool(m_idProgram, location, _value);
}

void shader::setInt(string _name, const int32_t _value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setInt(m_idProgram, location, _value);
}

void shader::setUint(string _name, const uint32_t _value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setUint(m_idProgram, location, _value);
}

void shader::setFloat(string _name, const float _value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setFloat(m_idProgram, location, _value	);
}

void shader::setFloat2(string _name, const glm::vec2 *_value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setFloat2(m_idProgram, location, &(*_value)[0]);
}
void shader::setFloat2(string _name, const glm::vec2 _value) const noexcept
{	setFloat2(_name, &_value); }

void shader::setFloat3(string _name, const glm::vec3 *_value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setFloat3(m_idProgram, location, &(*_value)[0]);
}
void shader::setFloat3(string _name, const glm::vec3 _value) const noexcept
{	setFloat3(_name, &_value); }

void shader::setFloat4(string _name, const glm::vec4 *_value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setFloat4(m_idProgram, location, &(*_value)[0]);
}
void shader::setFloat4(string _name, const glm::vec4 _value) const noexcept
{	setFloat4(_name, &_value); }

void shader::setMat3(string _name, const glm::mat3 *_value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setMat3(m_idProgram, location, &(*_value)[0][0]);
}
void shader::setMat3(string _name, const glm::mat3 _value) const noexcept
{	setMat3(_name, &_value); }

void shader::setMat4(string _name, const glm::mat4 *_value) const noexcept
{
	int32_t location = renderer::getUniformLocation(m_idProgram, _name.c_str());
	if (location < 0)
	{
		debug::send("Attempting to set unknown uniform \""
			+ _name
			+ "\", location: "
			+ std::to_string(location)
		);
		return;
	}
	renderer::setMat4(m_idProgram, location, &(*_value)[0][0]);
}
void shader::setMat4(string _name, const glm::mat4 _value) const noexcept
{	setMat4(_name, &_value); }
}
