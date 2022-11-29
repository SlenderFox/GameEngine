#pragma region
//#include "Entity.hpp"	// Included upstream by Renderer.hpp
#include "Renderer.hpp"

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

namespace Engine
{
	// Blame https://stackoverflow.com/a/40937193/15035125 for this
	struct EntityLoader
	{
		static inline
		void BackgroundLoadModel(
			string* pModelPath,
			string* pShaderPath,
			Entity* pEntity,
			const bool pLoadTextures = true
		) noexcept
		{
			uint8_t ID;
			pEntity->m_modelRef = Renderer::AddNewModel(ID, pModelPath, pShaderPath, pLoadTextures);
			Renderer::LoadLightsIntoShader(pEntity->m_modelRef->GetShaderRef());
			pEntity->UpdateModel();
		}
	};

	void EntityBase::AddChild(Entity* pChild) noexcept
	{ m_childrenRef.push_back(pChild); }

	void EntityBase::RemoveChild(const Entity* pChild) noexcept
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

	constexpr std::vector<Entity*>EntityBase::GetChildren() const noexcept
	{ return m_childrenRef; }

	// Static

	Entity* Entity::CreateWithModel(
		string pModelPath,
		string pShaderPath,
		Model*& pModelOut,
		Shader*& pShaderOut,
		const bool pLoadTextures
	) noexcept
	{
		Entity* result = new Entity();
		EntityLoader::BackgroundLoadModel(&pModelPath, &pShaderPath, result, pLoadTextures);
		pModelOut = result->m_modelRef;
		pShaderOut = result->m_modelRef->GetShaderRef();
		return result;
	}

	// Member

	Entity::Entity()
	: m_parentRef(Root::GetRoot())
	{ }

	Entity::Entity(EntityBase* pParent)
	: m_parentRef(pParent)
	{ }

	void Entity::UpdateModel() const noexcept
	{
		if (!m_modelRef) return;
		m_modelRef->GetShaderRef()->SetMat4("u_model", GetTransform());
		m_modelRef->GetShaderRef()->SetMat3("u_transposeInverseOfModel",
			(mat3)transpose(inverse(GetTransform())));
	}

	void Entity::LoadModel(
		string pModelPath,
		string pShaderPath,
		Model*& pModelOut,
		Shader*& pShaderOut,
		const bool pLoadTextures
	) noexcept
	{
		// Currently this does nothing about the previous model and shader
		// but does not cause a memory leak as they are managed by Renderer
		EntityLoader::BackgroundLoadModel(&pModelPath, &pShaderPath, this, pLoadTextures);
		pModelOut = m_modelRef;
		pShaderOut = m_modelRef->GetShaderRef();
	}

	#pragma region Setters
	void Entity::SetTransform(const mat4* pValue) noexcept
	{
		Transform::SetTransform(pValue);
		UpdateModel();
	}

	void Entity::SetPosition(const vec3 pValue) noexcept
	{
		Transform::SetPosition(pValue);
		UpdateModel();
	}

	void Entity::Translate(const vec3 pValue) noexcept
	{
		Transform::Translate(pValue);
		UpdateModel();
	}

	void Entity::SetParent(EntityBase* pParent) noexcept
	{
		// Look into the potential for this to return without changing anything
		if (!pParent)
		{
			m_parentRef = nullptr;
			return;
		}

		// Remove from previous parents children
		if (m_parentRef) { m_parentRef->RemoveChild(this); }

		// Assign new parent and join it's children
		m_parentRef = pParent;
		m_parentRef->AddChild(this);
	}

	void Entity::RenderOnlyColour(const bool pState) noexcept
	{ m_modelRef->GetShaderRef()->SetBool("u_justColour", pState); }

	void Entity::SetScale(const vec3 pValue) noexcept
	{ m_modelRef->GetShaderRef()->SetVec3("u_scale", pValue); }

	void Entity::SentTint(const Colour pCol) noexcept
	{ m_modelRef->GetShaderRef()->SetVec3("u_colour", pCol.RGBvec3()); }
	#pragma endregion

	constexpr EntityBase& Entity::GetParent() const noexcept
	{ return *m_parentRef; }
}
