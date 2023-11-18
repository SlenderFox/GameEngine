//#include "entity.hpp"	// Included upstream by graphics.hpp
#include "graphics.hpp"

using std::string;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::vector;
using glm::transpose;
using glm::inverse;

namespace srender
{
// Forward declaration
class application { public: _NODISCARD static std::string getAppLocation() noexcept; };

void entityBase::addChild(entity *_child) noexcept
{	m_childrenRef.push_back(_child); }

void entityBase::removeChild(const entity *_child) noexcept
{
	for (auto it = m_childrenRef.begin(); it != m_childrenRef.end(); ++it)
	{
		if (*it == _child)
		{
			m_childrenRef.erase(it);
			break;
		}
	}
}

constexpr std::vector<entity*> entityBase::getChildren() const noexcept
{	return m_childrenRef; }

// Member

entity::entity()
: m_parentRef(root::getRoot())
{ }

entity::entity(entityBase *_parent)
: m_parentRef(_parent)
{ }

entity::entity(
	std::string _modelPath,
	std::string _shaderPath,
	model *&_outModel,
	shader *&_outShader,
	const bool _loadTextures
)
{	loadModel(_modelPath, _shaderPath, _outModel, _outShader, _loadTextures); }

void entity::updateModel() const noexcept
{
	if (!m_modelRef)
	{	return; }

	m_modelRef->getShaderRef()->setMat4("u_model", getTransform());
	m_modelRef->getShaderRef()->setMat3(
		"u_transposeInverseOfModel",
		(mat3)transpose(inverse(getTransform()))
	);
}

void entity::loadModel(
	string _modelPath,
	string _shaderPath,
	model *&_outModel,
	shader *&_outShader,
	const bool _loadTextures
)
{
	// Currently this does nothing about the previous model and shader
	// but only half causes a memory leak as they are managed by graphics
	_modelPath = application::getAppLocation() + _modelPath;
	_shaderPath = application::getAppLocation() + _shaderPath;
	m_modelRef = graphics::addNewModel(&_modelPath, &_shaderPath, _loadTextures);
	graphics::loadLightsIntoShader(m_modelRef->getShaderRef());
	updateModel();
	_outModel = m_modelRef;
	_outShader = m_modelRef->getShaderRef();
}

void entity::setTransform(const mat4 *_value) noexcept
{
	transform::setTransform(_value);
	updateModel();
}

void entity::setPosition(const vec3 _value) noexcept
{
	transform::setPosition(_value);
	updateModel();
}

void entity::translate(const vec3 _value) noexcept
{
	transform::translate(_value);
	updateModel();
}

void entity::setParent(entityBase *_parent) noexcept
{
	// Look into the potential for this to return without changing anything
	if (!_parent)
	{
		m_parentRef = nullptr;
		return;
	}

	// Remove from previous parents children
	if (m_parentRef)
	{	m_parentRef->removeChild(this); }

	// Assign new parent and join it's children
	m_parentRef = _parent;
	m_parentRef->addChild(this);
}

void entity::renderOnlyColour(const bool _state) noexcept
{	m_modelRef->getShaderRef()->setBool("u_justColour", _state); }

void entity::setScale(const vec3 _value) noexcept
{	m_modelRef->getShaderRef()->setFloat3("u_scale", _value); }

void entity::sentTint(const colour _colour) noexcept
{	m_modelRef->getShaderRef()->setFloat3("u_colour", _colour.rgb()); }

constexpr entityBase &entity::getParent() const noexcept
{	return *m_parentRef; }
}
