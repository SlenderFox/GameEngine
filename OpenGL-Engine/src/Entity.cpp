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

	Entity* Entity::LoadModel(Model* pModelOut, std::string pModelPath,
	 Shader* pShaderOut, std::string pShaderPath, bool pLoadTextures)
	{
		Renderer* renderer = Renderer::GetInstance();
		Entity* result = new Entity();
		uint8_t ID;
		result->m_shader = renderer->AddNewShader(ID, pShaderPath);
		renderer->LoadLightsIntoShader(*result->m_shader);
		result->m_shader->SetMat4("u_model", result->GetTransform());
		result->m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(result->GetTransform())));
		result->m_model = renderer->AddNewModel(ID, pModelPath, result->m_shader, pLoadTextures);
		pShaderOut = result->m_shader;
		pModelOut = result->m_model;
		return result;
	}

	// Member

	Entity::Entity()
	{
		m_children = vector<Entity*>();
		SetParent(nullptr);
	}

	Entity::Entity(Entity* pParent)
	{
		m_children = vector<Entity*>();
		SetParent(pParent);
	}

	Entity::~Entity() {}

	void Entity::AddChild(Entity* pChild)
	{
		m_children.push_back(pChild);
	}

	void Entity::RemoveChild(Entity* pChild)
	{
		for (vector<Entity*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (*it == pChild)
			{
				m_children.erase(it);
				break;
			}
		}
	}

	void Entity::UpdateModel() const
	{
		m_shader->SetMat4("u_model", GetTransform());
		m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
	}

	#pragma region Setters
	void Entity::SetParent(Entity* pParent)
	{
		// This may change to not changing anything
		if (pParent == nullptr)
		{
			m_parent = nullptr;
			return;
		}

		// Remove from previous parents children
		if (m_parent != nullptr)
			m_parent->RemoveChild(this);
		
		// Assign new parent and join it's children
		m_parent = pParent;
		m_parent->AddChild(this);
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

	void Entity::SingleColour(bool pState)
	{
		m_shader->SetBool("u_justColour", pState);
	}

	void Entity::SetScale(vec3 pValue)
	{
		m_shader->SetVec3("u_scale", pValue);
	}

	void Entity::JustColour(Colour pCol)
	{
		m_shader->SetVec3("u_colour", pCol.RGBvec3());
	}
	#pragma endregion
}
