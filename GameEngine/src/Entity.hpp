#pragma once
#include "Colour.hpp"

namespace Engine
{
	class Entity;

	class EntityBase: public Transform
	{
	protected:
		std::vector<Entity*> m_childrenRef;

	public:
		void AddChild(Entity* pChild) noexcept;
		void RemoveChild(Entity* pChild) noexcept;

		constexpr std::vector<Entity*> GetChildren() const noexcept { return m_childrenRef; }
	};

	class Entity: public EntityBase
	{
		friend struct EntityLoader;
	public:
		static Entity* CreateWithModel(
			std::string pModelPath,
			std::string pShaderPath,
			Model*& pModelOut,
			Shader*& pShaderOut,
			bool pLoadTextures = true) noexcept;

	private:
		EntityBase* m_parentRef = nullptr;
		Model* m_modelRef = nullptr;

		void UpdateModel() const noexcept;

	public:
		Entity();
		Entity(EntityBase* pParent);
		~Entity() {}

		void LoadModel(
			std::string pModelPath,
			std::string pShaderPath,
			Model*& pModelOut,
			Shader*& pShaderOut,
			bool pLoadTextures = true) noexcept;

#		pragma region Setters
		void SetTransform(glm::mat4 pValue) noexcept override;
		void Translate(glm::vec3 pValue) noexcept override;

		void SetParent(EntityBase* pParent) noexcept;
		void RenderOnlyColour(bool pState) noexcept;
		void SetScale(glm::vec3 pValue) noexcept;
		void SetColourInShader(Colour pCol) noexcept;
#		pragma endregion

		constexpr EntityBase& GetParent() const noexcept { return *m_parentRef; }
	};

	/// @brief Root is a special, static entity that only has children
	class Root: public EntityBase
	{
	public:
		static Root* GetRoot() noexcept
		{
			static Root* s_root = new Root();
			return s_root;
		}

#		pragma region Constructors
		Root() = default;
		~Root() {}
		// Delete copy/move so extra instances can't be created/moved.
		Root(Root const&) = delete;
		Root& operator=(Root const&) = delete;
		Root(Root&&) = delete;
		Root& operator=(Root&&) = delete;
#		pragma endregion
	};
}
