#include "entity.hpp"
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
std::vector<entity*> l_rootChildrenRef;

void addToVector(vector<entity*> &_vector, entity *_child) noexcept
{	_vector.push_back(_child); }

void removeFromVector(vector<entity*> &_vector, const entity *_child) noexcept
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

// Static

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
	if (m_model)
	{	delete m_model; }
	if (m_light)
	{	delete m_light; }
}

void entity::setTransform(const mat4 *_value) noexcept
{
	m_transform.setTransform(_value);
	updateModel();
}

void entity::setPosition(const vec3 _value) noexcept
{
	m_transform.setPosition(_value);
	m_light->setPosition(m_transform.getPosition());
	updateModel();
}

void entity::setForward(const vec3 _value) noexcept
{
	m_transform.setForward(_value);
	// Do this because the forward is not exactly _value
	m_light->setForward(m_transform.getForward());
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

void entity::setScale(const vec3 _value) noexcept
{
	m_transform.setScale(_value);
	updateModel();
}

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

// TODO finish reworking
void entity::addComponent(model *_model)
{
	//= Maybe instead, the current model is deleted and a new one is loaded
	assert(!m_model && "Entity already has a model component");
	m_model = _model;
	graphics::addNewModel(_model);
	updateModel();
}

// TODO finish reworking
void entity::addComponent(light *_light) noexcept
{
	assert(!m_light && "Entity already has a light component");
	m_light = _light;
	graphics::addNewLight(_light);
}

model *entity::getComponentModel() const noexcept
{	return m_model; }

light *entity::getComponentLight() const noexcept
{	return m_light; }
}
