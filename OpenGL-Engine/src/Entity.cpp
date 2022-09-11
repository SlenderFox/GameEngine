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
		Model* AddNewModel(unsigned int &id, std::string pLocation, Shader* pShaderRef = nullptr);
		Shader* AddNewShader(unsigned int &id, std::string pLocation);
		void LoadLightsIntoShader(Shader* pShader);
	};

	Entity* Entity::s_root = nullptr;

	Entity::Entity(): m_parent(s_root)
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Entity::Entity(Entity* pParent): m_parent(pParent)
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Model* Entity::LoadModel(std::string pModelPath, std::string pShaderPath)
	{
		unsigned int ID;
		m_shader = Renderer::GetInstance()->AddNewShader(ID, pShaderPath);
		Renderer::GetInstance()->LoadLightsIntoShader(m_shader);
		m_shader->SetMat4("u_model", GetTransform());
		m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
		return Renderer::GetInstance()->AddNewModel(ID, pModelPath, m_shader);
	}

	void Entity::Translate(vec3 pValue)
	{
		Transform::Translate(pValue);
		m_shader->SetMat4("u_model", GetTransform());
		m_shader->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
	}
}
