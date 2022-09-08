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

	Entity::Entity() : m_parent(s_root)
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Entity::Entity(Entity* pParent) : m_parent(pParent)
	{
		m_children = make_unique<vector<Entity*>>();
	}

	Model* Entity::LoadModel(std::string pModelPath, std::string pShaderPath)
	{
		unsigned int ID;
		Engine::Shader* shaderRef = Renderer::GetInstance()->AddNewShader(ID, "assets/shaders/default");
		Renderer::GetInstance()->LoadLightsIntoShader(shaderRef);
		shaderRef->SetMat4("u_model", GetTransform());
		shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
		return Renderer::GetInstance()->AddNewModel(ID, "assets/models/backpack/backpack.obj", shaderRef);
	}

	void Entity::Translate(vec3 pValue)
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
		Shader* ref = m_model->GetShaderRef();
		ref->SetMat4("u_model", GetTransform());
		ref->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(GetTransform())));
	}
}
