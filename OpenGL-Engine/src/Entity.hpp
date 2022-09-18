#pragma once
#include "Colour.hpp"

namespace Engine
{
	class Entity: public Transform
	{
	public:
		static Entity* CreateWithModel(std::string pModelPath, std::string pShaderPath,
		 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures = true);

	protected:
		Entity* m_parentRef = nullptr;
		std::vector<Entity*> m_childrenRef;

		Model* m_modelRef = nullptr;
		Shader* m_shaderRef = nullptr;	// TODO: Remove this, already stored in model

		void UpdateModel() const;

	public:
		Entity();
		Entity(Entity* pParent);
		~Entity() {}

		void AddChild(Entity* pChild);
		void RemoveChild(Entity* pChild);

		#pragma region Setters
		void LoadModel(std::string pModelPath, std::string pShaderPath,
		 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures = true);
		void SetParent(Entity* pParent);
		void SetTransform(glm::mat4 pValue) override;
		void Translate(glm::vec3 pValue) override;
		void RenderOnlyColour(bool pState);
		void SetScale(glm::vec3 pValue);
		void SetColourInShader(Colour pCol);
		#pragma endregion

		Entity& GetParent() const { return *m_parentRef; }
		virtual std::vector<Entity*> GetChildren() const { return m_childrenRef; }
	};

	/// @brief Root is a special, static entity that only has children
	class Root: public Entity
	{
	public:
		static Root* GetRoot()
		{
			static Root* s_root = new Root();
			return s_root;
		}

		~Root() {}
		
	private:
		std::vector<Entity*> m_childrenRef = std::vector<Entity*>();

		#pragma region Constructors
		Root() = default;
		// Delete copy/move so extra instances can't be created/moved.
		Root(const Root&) = delete;
		Root& operator=(const Root&) = delete;
		Root(Root&&) = delete;
		Root& operator=(Root&&) = delete;
		#pragma endregion

	public:
		std::vector<Entity*> GetChildren() const override { return m_childrenRef; }
	};
}
