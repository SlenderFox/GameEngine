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

std::vector<entity*> l_rootChildrenRef;

// Static

void entity::addToVector(vector<entity*> &_vector, entity *_child) noexcept
{	_vector.push_back(_child); }

void entity::removeFromVector(vector<entity*> &_vector, const entity *_child) noexcept
{
	// Linear search
	for (auto it = _vector.begin(); it != _vector.end(); ++it)
	{
		if (*it == _child)
		{
			_vector.erase(it);
			break;
		}
	}
}

vector<entity*> entity::getRootChildren() noexcept
{	return l_rootChildrenRef; }

// Member

void entity::updateModel() const noexcept
{
	if (!m_model)
	{	return; }

	m_model->getShaderRef()->setMat4("u_model", m_transform.getTransform());
	m_model->getShaderRef()->setMat3(
		"u_transposeInverseOfModel",
		(mat3)transpose(inverse(m_transform.getTransform()))
	);
}

entity::entity()
{	setParent(nullptr); }

entity::entity(entity *_parent)
{	setParent(_parent); }

entity::~entity()
{
	if (m_light)
	{	delete m_light; }

	if (m_model)
	{	delete m_model; }
}

void entity::setTransform(const mat4 *_value) noexcept
{
	m_transform.setTransform(_value);
	updateModel();
}

void entity::setPosition(const vec3 _value) noexcept
{
	m_transform.setPosition(_value);
	updateModel();
}

void entity::setForward(const vec3 _value) noexcept
{
	m_transform.setForward(_value);
	updateModel();
}

void entity::translate(const vec3 _value) noexcept
{
	m_transform.translate(_value);
	updateModel();
}

void entity::setParent(entity *_parent) noexcept
{
	// Remove as child of previous parent
	if (m_parentRef)
	{	m_parentRef->removeChild(this); }
	else
	{	removeFromVector(l_rootChildrenRef, this); }

	// Assign new parent
	m_parentRef = _parent;

	// Join its children
	if (_parent)
	{	m_parentRef->addChild(this); }
	else
	{	addToVector(l_rootChildrenRef, this); }
}

void entity::renderOnlyColour(const bool _state) noexcept
{	m_model->getShaderRef()->setBool("u_justColour", _state); }

void entity::setScale(const vec3 _value) noexcept
{
	m_transform.setScale(_value);
	updateModel();
}

void entity::sentTint(const colour _colour) noexcept
{	m_model->getShaderRef()->setFloat3("u_colour", _colour.rgb()); }

void entity::addChild(entity *_child) noexcept
{	addToVector(m_childrenRef, _child); }

void entity::removeChild(const entity *_child) noexcept
{	removeFromVector(m_childrenRef, _child); }

entity &entity::getParent() const noexcept
{	return *m_parentRef; }

std::vector<entity*> entity::getChildren() const noexcept
{	return m_childrenRef; }

const transform entity::getTransform() const noexcept
{	return m_transform; }

void entity::addComponent(
	string _modelPath,
	string _shaderPath,
	const bool _loadTextures
)
{
	assert(!m_model && "Entity already has a model component");
	string appLoc = application::getAppLocation();
	_modelPath = appLoc + _modelPath;
	_shaderPath = appLoc + _shaderPath;
	string *modelPath_p = _modelPath == appLoc ? nullptr : &_modelPath;
	string *shaderPath_p = _shaderPath == appLoc ? nullptr : &_shaderPath;
	m_model = new model(modelPath_p, shaderPath_p, _loadTextures);
	graphics::addNewModel(m_model);
	updateModel();
}

void entity::addComponent(
	const light::type _type,
	const colour _colour
) noexcept
{
	assert(!m_light && "Entity already has a light component");
	m_light = new light(_type, _colour);
	graphics::addNewLight(this);
}

model *entity::getComponentModel() const noexcept
{	return m_model; }

light *entity::getComponentLight() const noexcept
{	return m_light; }
}
