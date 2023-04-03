//#include "entity.hpp"	// Included upstream by renderer.hpp
#include "renderer.hpp"

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

	// Blame https://stackoverflow.com/a/40937193/15035125 for this
	struct entityLoader
	{
		static inline void BackgroundLoadModel(
			string *_modelPath,
			string *_shaderPath,
			entity *_entity,
			const bool _loadTextures = true
		) noexcept
		{
			uint8_t ID;
			_entity->m_modelRef = renderer::addNewModel(ID, _modelPath, _shaderPath, _loadTextures);
			renderer::loadLightsIntoShader(_entity->m_modelRef->getShaderRef());
			_entity->updateModel();
		}
	};

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

	constexpr std::vector<entity*>entityBase::getChildren() const noexcept
	{	return m_childrenRef; }

	// Static

	entity *entity::createWithModel(
		string _modelPath,
		string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures
	) noexcept
	{
		entity *result = new entity();
		_modelPath = application::getAppLocation() + _modelPath;
		_shaderPath = application::getAppLocation() + _shaderPath;
		entityLoader::BackgroundLoadModel(&_modelPath, &_shaderPath, result, _loadTextures);
		_outModel = result->m_modelRef;
		_outShader = result->m_modelRef->getShaderRef();
		return result;
	}

	// Member

	entity::entity()
	: m_parentRef(root::getRoot())
	{ }

	entity::entity(entityBase *_parent)
	: m_parentRef(_parent)
	{ }

	void entity::updateModel() const noexcept
	{
		if (!m_modelRef) return;
		m_modelRef->getShaderRef()->setMat4("u_model", getTransform());
		m_modelRef->getShaderRef()->setMat3("u_transposeInverseOfModel",
			(mat3)transpose(inverse(getTransform())));
	}

	void entity::loadModel(
		string _modelPath,
		string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures
	) noexcept
	{
		// Currently this does nothing about the previous model and shader
		// but does not cause a memory leak as they are managed by renderer
		_modelPath = application::getAppLocation() + _modelPath;
		_shaderPath = application::getAppLocation() + _shaderPath;
		entityLoader::BackgroundLoadModel(&_modelPath, &_shaderPath, this, _loadTextures);
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
