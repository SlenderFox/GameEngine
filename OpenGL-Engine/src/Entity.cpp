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
	// Static

	Entity* Entity::LoadModel(Model* pModelOut, std::string pModelPath,
	 Shader* pShaderOut, std::string pShaderPath, bool pLoadTextures)
	{
		Entity* result = new Entity();
		uint8_t ID;
		result->m_shader = Renderer::GetInstance()->AddNewShader(ID, pShaderPath);
		Renderer::GetInstance()->LoadLightsIntoShader(*result->m_shader);
		result->m_shader->SetMat4("u_model", result->GetTransform());
		result->m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(result->GetTransform())));
		result->m_model = Renderer::GetInstance()->AddNewModel(ID, pModelPath, result->m_shader, pLoadTextures);
		pShaderOut = result->m_shader;
		pModelOut = result->m_model;
		return result;
	}

	// Member

	Entity::Entity(): m_parent(nullptr)
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Entity::Entity(Entity* pParent): m_parent(pParent)
	{
		m_children = make_unique<vector<Entity*>>();
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

	void Entity::Scale(vec3 pValue)
	{
		m_shader->SetVec3("u_scale", pValue);
	}

	void Entity::UpdateModel() const
	{
		m_shader->SetMat4("u_model", GetTransform());
		m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
	}
}
