#pragma region
//#include "entity.hpp"	// Included upstream by renderer.hpp
#include "renderer.hpp"

using std::string;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::vector;
using std::make_unique;
using glm::transpose;
using glm::inverse;
#pragma endregion

namespace engine
{
	// Blame https://stackoverflow.com/a/40937193/15035125 for this
	struct entityLoader
	{
		static inline
		void BackgroundLoadModel(
			string *pModelPath,
			string *pShaderPath,
			entity *pEntity,
			const bool pLoadTextures = true
		) noexcept
		{
			uint8_t ID;
			pEntity->m_modelRef = renderer::addNewModel(ID, pModelPath, pShaderPath, pLoadTextures);
			renderer::loadLightsIntoShader(pEntity->m_modelRef->getShaderRef());
			pEntity->updateModel();
		}
	};

	void entityBase::addChild(entity *pChild) noexcept
	{ m_childrenRef.push_back(pChild); }

	void entityBase::removeChild(const entity *pChild) noexcept
	{
		for (auto it = m_childrenRef.begin(); it != m_childrenRef.end(); ++it)
		{
			if (*it == pChild)
			{
				m_childrenRef.erase(it);
				break;
			}
		}
	}

	constexpr std::vector<entity*>entityBase::getChildren() const noexcept
	{ return m_childrenRef; }

	// Static

	entity *entity::createWithModel(
		string pModelPath,
		string pShaderPath,
		model *&pModelOut,
		shader *&pShaderOut,
		const bool pLoadTextures
	) noexcept
	{
		entity *result = new entity();
		entityLoader::BackgroundLoadModel(&pModelPath, &pShaderPath, result, pLoadTextures);
		pModelOut = result->m_modelRef;
		pShaderOut = result->m_modelRef->getShaderRef();
		return result;
	}

	// Member

	entity::entity()
	: m_parentRef(root::getRoot())
	{ }

	entity::entity(entityBase *pParent)
	: m_parentRef(pParent)
	{ }

	void entity::updateModel() const noexcept
	{
		if (!m_modelRef) return;
		m_modelRef->getShaderRef()->setMat4("u_model", getTransform());
		m_modelRef->getShaderRef()->setMat3("u_transposeInverseOfModel",
			(mat3)transpose(inverse(getTransform())));
	}

	void entity::loadModel(
		string pModelPath,
		string pShaderPath,
		model *&pModelOut,
		shader *&pShaderOut,
		const bool pLoadTextures
	) noexcept
	{
		// Currently this does nothing about the previous model and shader
		// but does not cause a memory leak as they are managed by renderer
		entityLoader::BackgroundLoadModel(&pModelPath, &pShaderPath, this, pLoadTextures);
		pModelOut = m_modelRef;
		pShaderOut = m_modelRef->getShaderRef();
	}

	#pragma region Setters
	void entity::setTransform(const mat4 *pValue) noexcept
	{
		transform::setTransform(pValue);
		updateModel();
	}

	void entity::setPosition(const vec3 pValue) noexcept
	{
		transform::setPosition(pValue);
		updateModel();
	}

	void entity::translate(const vec3 pValue) noexcept
	{
		transform::translate(pValue);
		updateModel();
	}

	void entity::setParent(entityBase *pParent) noexcept
	{
		// Look into the potential for this to return without changing anything
		if (!pParent)
		{
			m_parentRef = nullptr;
			return;
		}

		// Remove from previous parents children
		if (m_parentRef) { m_parentRef->removeChild(this); }

		// Assign new parent and join it's children
		m_parentRef = pParent;
		m_parentRef->addChild(this);
	}

	void entity::renderOnlyColour(const bool pState) noexcept
	{ m_modelRef->getShaderRef()->setBool("u_justColour", pState); }

	void entity::setScale(const vec3 pValue) noexcept
	{ m_modelRef->getShaderRef()->setFloat3("u_scale", pValue); }

	void entity::sentTint(const colour pCol) noexcept
	{ m_modelRef->getShaderRef()->setFloat3("u_colour", pCol.rgb()); }
	#pragma endregion

	constexpr entityBase &entity::getParent() const noexcept
	{ return *m_parentRef; }
}
