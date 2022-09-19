#pragma region
#include "Entity.hpp"

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
	// Forward declaration
	class Renderer
	{
	public:
		static Renderer* GetInstance();
		Model* AddNewModel(uint8_t &id, std::string pLocation, Shader* pShaderRef = nullptr, bool pLoadTextures = true);
		Shader* AddNewShader(uint8_t &id, std::string pLocation);
		void LoadLightsIntoShader(const Shader& pShader);
	};

	// Static

	Entity* Entity::CreateWithModel(std::string pModelPath, std::string pShaderPath,
	 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures)
	{
		Renderer* renderer = Renderer::GetInstance();
		Entity* result = new Entity();
		uint8_t ID;
		result->m_shaderRef = renderer->AddNewShader(ID, pShaderPath);
		renderer->LoadLightsIntoShader(*result->m_shaderRef);
		result->m_shaderRef->SetMat4("u_model", result->GetTransform());
		result->m_shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(result->GetTransform())));
		result->m_modelRef = renderer->AddNewModel(ID, pModelPath, result->m_shaderRef, pLoadTextures);
		pModelOut = result->m_modelRef;
		pShaderOut = result->m_shaderRef;
		return result;
	}

	// Member

	Entity::Entity()
	{
		m_childrenRef = vector<Entity*>();
		SetParent(nullptr);
	}

	Entity::Entity(Entity* pParent)
	{
		m_childrenRef = vector<Entity*>();
		SetParent(pParent);
	}

	void Entity::AddChild(Entity* pChild)
	{
		m_childrenRef.push_back(pChild);
	}

	void Entity::RemoveChild(Entity* pChild)
	{
		for (vector<Entity*>::iterator it = m_childrenRef.begin(); it != m_childrenRef.end(); ++it)
		{
			if (*it == pChild)
			{
				m_childrenRef.erase(it);
				break;
			}
		}
	}

	void Entity::UpdateModel() const
	{
		m_shaderRef->SetMat4("u_model", GetTransform());
		m_shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
	}

	#pragma region Setters
	void Entity::LoadModel(std::string pModelPath, std::string pShaderPath,
		 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures)
	{
		Renderer* renderer = Renderer::GetInstance();
		uint8_t ID;
		m_shaderRef = renderer->AddNewShader(ID, pShaderPath);
		renderer->LoadLightsIntoShader(*m_shaderRef);
		m_shaderRef->SetMat4("u_model", GetTransform());
		m_shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
		m_modelRef = renderer->AddNewModel(ID, pModelPath, m_shaderRef, pLoadTextures);
		pModelOut = m_modelRef;
		pShaderOut = m_shaderRef;
	}

	void Entity::SetParent(Entity* pParent)
	{
		// This may change to not changing anything
		if (!pParent)
		{
			m_parentRef = nullptr;
			return;
		}

		// Remove from previous parents children
		if (m_parentRef)
			m_parentRef->RemoveChild(this);
		
		// Assign new parent and join it's children
		m_parentRef = pParent;
		m_parentRef->AddChild(this);
	}

	void Entity::SetTransform(mat4 pValue)
	{
		Transform::SetTransform(pValue);
		UpdateModel();
	}

	void Entity::Translate(vec3 pValue)
	{
		Transform::Translate(pValue);
		UpdateModel();
	}

	void Entity::RenderOnlyColour(bool pState)
	{
		m_shaderRef->SetBool("u_justColour", pState);
	}

	void Entity::SetScale(vec3 pValue)
	{
		m_shaderRef->SetVec3("u_scale", pValue);
	}

	void Entity::SetColourInShader(Colour pCol)
	{
		m_shaderRef->SetVec3("u_colour", pCol.RGBvec3());
	}
	#pragma endregion
}
