#pragma once
#include "Renderer.hpp"

namespace Engine
{
	class Entity: public Transform
	{
	public:
		static Entity* LoadModel(Model* pModelOut, std::string pModelPath,
					   Shader* pShaderOut, std::string pShaderPath,
					   bool pLoadTextures = true);

	private:
		Entity* m_parent = nullptr;
		std::unique_ptr<std::vector<Entity*>> m_children = nullptr;

		Model* m_model = nullptr;
		Shader* m_shader = nullptr;	// TODO: Remove this

		void UpdateModel() const;

	public:
		Entity();
		Entity(Entity* pParent);

		#pragma region Setters
		void SetParent(Entity* pParent);
		void SetTransform(glm::mat4 pValue) override;
		void Translate(glm::vec3 pValue) override;
		void SingleColour(bool pState);
		void SetScale(glm::vec3 pValue);
		void JustColour(Colour pCol);
		#pragma endregion

		Entity* GetParent() const { return m_parent; }
		std::vector<Entity*>* GetChildren() const { return m_children.get(); }
	};
}
