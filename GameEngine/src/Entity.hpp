#pragma once
#include "Colour.hpp"

namespace Engine
{
	class Entity;

	/** Entity base is used to allow root to work */
	class EntityBase: public Transform
	{
	protected:
		std::vector<Entity*> m_childrenRef = std::vector<Entity*>();

		EntityBase() = default;

	public:
		~EntityBase() = default;

		void AddChild(Entity* inChild) noexcept;
		void RemoveChild(const Entity* inChild) noexcept;

		_NODISCARD constexpr
		std::vector<Entity*> GetChildren() const noexcept;
	};

	/** An entity is any object in world space, each entity must have a parent*/
	class Entity: public EntityBase
	{
		friend struct EntityLoader;
	public:
		_NODISCARD static
		Entity* CreateWithModel(
			std::string pModelPath,
			std::string pShaderPath,
			Model*& pModelOut,
			Shader*& pShaderOut,
			const bool inLoadTextures = true
		) noexcept;

	private:
		EntityBase* m_parentRef = nullptr;
		Model* m_modelRef = nullptr;

		void UpdateModel() const noexcept;

	public:
		Entity();
		Entity(EntityBase* inParent);
		~Entity() {}

		void LoadModel(
			std::string pModelPath,
			std::string pShaderPath,
			Model*& pModelOut,
			Shader*& pShaderOut,
			const bool inLoadTextures = true
		) noexcept;

		#pragma region Setters
		void SetTransform(const glm::mat4* inValue) noexcept override;
		void SetPosition(const glm::vec3 inValue) noexcept override;
		void Translate(const glm::vec3 inValue) noexcept override;

		void SetParent(EntityBase* inParent) noexcept;
		void RenderOnlyColour(const bool inState) noexcept;
		void SetScale(const glm::vec3 inValue) noexcept;
		void SentTint(const Colour inCol) noexcept;
		#pragma endregion

		_NODISCARD constexpr
		EntityBase& GetParent() const noexcept;
	};

	/** Root is a special, static entity that only has children */
	class Root: public EntityBase
	{
	public:
		static Root* GetRoot() noexcept
		{
			static Root* s_root = new Root();
			return s_root;
		}

		#pragma region Constructors
		~Root() = default;

	private:
		Root() = default;
		// Delete copy/move so extra instances can't be created/moved.
		Root(Root const&) = delete;
		Root& operator=(Root const&) = delete;
		Root(Root&&) = delete;
		Root& operator=(Root&&) = delete;
		#pragma endregion
	};
}
