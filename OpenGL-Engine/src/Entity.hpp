#pragma once
#include "Model.hpp"

namespace Engine
{
	class Entity : public Transform
	{
	// Static
	private:
		static Entity* s_root;

	// Member
	private:
		Entity* m_parent = nullptr;
		std::unique_ptr<std::vector<Entity*>> m_children = nullptr;

		Model* m_model = nullptr;

	public:
		Entity();
		Entity(Entity* pParent);

		Model* LoadModel(std::string pModelPath, std::string pShaderPath);
		void Translate(glm::vec3 pValue) override;
		
		Entity* GetParent() const { return m_parent; }
		std::vector<Entity*>* GetChildren() const { return m_children.get(); }
		Model* GetAttachedModel();
	};
}
