#pragma once
#include "Colour.hpp"

namespace Engine
{
	class Entity: public Transform
	{
	public:
		static Entity* CreateWithModel(std::string pModelPath, std::string pShaderPath,
		 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures = true) noexcept;

	private:
		Entity* m_parentRef = nullptr;
		Model* m_modelRef = nullptr;

		void UpdateModel() const noexcept;

	protected:
		std::vector<Entity*> m_childrenRef;

	public:
		Entity();
		Entity(Entity* pParent);
		~Entity() {}

		void AddChild(Entity* pChild) noexcept;
		void RemoveChild(Entity* pChild) noexcept;

		#pragma region Setters
		void SetTransform(glm::mat4 pValue) noexcept override;
		void Translate(glm::vec3 pValue) noexcept override;

		void LoadModel(std::string pModelPath, std::string pShaderPath,
		 Model*& pModelOut, Shader*& pShaderOut, bool pLoadTextures = true) noexcept;
		void SetParent(Entity* pParent) noexcept;
		void RenderOnlyColour(bool pState) noexcept;
		void SetScale(glm::vec3 pValue) noexcept;
		void SetColourInShader(Colour pCol) noexcept;
		#pragma endregion

		constexpr Entity& GetParent() const noexcept { return *m_parentRef; }
		constexpr std::vector<Entity*> GetChildren() const noexcept { return m_childrenRef; }
	};

	/// @brief Root is a special, static entity that only has children
	class Root: public Entity
	{
	public:
		static Root* GetRoot() noexcept
		{
			static Root* s_root = new Root();
			return s_root;
		}

		#pragma region Constructors
		Root() = default;
		~Root() {}
		// Delete copy/move so extra instances can't be created/moved.
		Root(const Root&) = delete;
		Root& operator=(const Root&) = delete;
		Root(Root&&) = delete;
		Root& operator=(Root&&) = delete;
		#pragma endregion
	};
}
