#pragma once
#include "Model.hpp"

namespace Engine
{
	class Entity: public Transform
	{
	private:
		static Entity* s_root;

	private:
		Entity* m_parent = nullptr;
		std::unique_ptr<std::vector<Entity*>> m_children = nullptr;

		Model* m_model = nullptr;
		Shader* m_shader = nullptr;	// TODO: Remove this

	public:
		Entity();
		Entity(Entity* pParent);

		Model* LoadModel(std::string pModelPath, std::string pShaderPath, bool pLoadTextures = true);
		void Translate(glm::vec3 pValue) override;
		
		Entity* GetParent() const { return m_parent; }
		std::vector<Entity*>* GetChildren() const { return m_children.get(); }
	};
}
