#pragma once
#include "colour.hpp"

namespace srender
{
	class entity;

	/** Entity base is used to allow root to work */
	class entityBase: public transform
	{
	protected:
		std::vector<entity*> m_childrenRef = std::vector<entity*>();

		entityBase() = default;

	public:
		~entityBase() = default;

		void addChild(entity *inChild) noexcept;
		void removeChild(const entity *inChild) noexcept;

		_NODISCARD constexpr
		std::vector<entity*> getChildren() const noexcept;
	};

	/** An entity is any object in world space, each entity must have a parent*/
	class entity: public entityBase
	{
		friend struct entityLoader;

	public:
		_NODISCARD static
		entity *createWithModel(
			std::string pModelPath,
			std::string pShaderPath,
			model *&pModelOut,
			shader *&pShaderOut,
			const bool inLoadTextures = true
		) noexcept;

	private:
		entityBase *m_parentRef = nullptr;
		model *m_modelRef = nullptr;

		void updateModel() const noexcept;

	public:
		entity();
		entity(entityBase *inParent);
		~entity() {}

		void loadModel(
			std::string pModelPath,
			std::string pShaderPath,
			model *&pModelOut,
			shader *&pShaderOut,
			const bool inLoadTextures = true
		) noexcept;

		#pragma region Setters
		void setTransform(const glm::mat4 *inValue) noexcept override;
		void setPosition(const glm::vec3 inValue) noexcept override;
		void translate(const glm::vec3 inValue) noexcept override;

		void setParent(entityBase *inParent) noexcept;
		void renderOnlyColour(const bool inState) noexcept;
		void setScale(const glm::vec3 inValue) noexcept;
		void sentTint(const colour inCol) noexcept;
		#pragma endregion

		_NODISCARD constexpr
		entityBase &getParent() const noexcept;
	};

	/** Root is a special, static entity that only has children */
	class root: public entityBase
	{
	public:
		static root *getRoot() noexcept
		{
			static root *s_root = new root();
			return s_root;
		}

		#pragma region Constructors
		~root() = default;

	private:
		root() = default;
		// Delete copy/move so extra instances can't be created/moved.
		root(root const&) = delete;
		root &operator=(root const&) = delete;
		root(root&&) = delete;
		root &operator=(root&&) = delete;
		#pragma endregion
	};
}
