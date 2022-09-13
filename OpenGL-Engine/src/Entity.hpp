#pragma once
#include "Renderer.hpp"

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

		void UpdateModel() const;

	public:
		Entity();
		Entity(Entity* pParent);

		Model* LoadModel(std::string pModelPath, std::string pShaderPath, bool pLoadTextures = true);

		void SetTransform(glm::mat4 pValue) override;
		void Translate(glm::vec3 pValue) override;
		void Scale(glm::vec3 pValue);
		
		Entity* GetParent() const { return m_parent; }
		std::vector<Entity*>* GetChildren() const { return m_children.get(); }
	};
}
