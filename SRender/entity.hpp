#pragma once
#include "model.hpp"
#include "colour.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
class entity;

/** Entity base is used to allow root to work */
class entityBase : public transform
{
protected:
	std::vector<entity*> m_childrenRef = std::vector<entity*>();

	entityBase() = default;

public:
	~entityBase() = default;

	void addChild(entity *_child) noexcept;
	void removeChild(const entity *_child) noexcept;

	_NODISCARD constexpr std::vector<entity*> getChildren() const noexcept;
};

/** An entity is any object in world space, each entity must have a parent
 * @todo Overload entity copy constructor to "load models better"
 * @todo Look into ability to swap models
*/
class entity : public entityBase
{
private:
	entityBase *m_parentRef = nullptr;
	model *m_modelRef = nullptr;

	void updateModel() const noexcept;

public:
	entity();
	entity(entityBase *_parent);
	entity(
		std::string _modelPath,
		std::string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures = true
	);
	~entity() {}

	void loadModel(
		std::string _modelPath,
		std::string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures = true
	);

	void setTransform(const glm::mat4 *_value) noexcept override;
	void setPosition(const glm::vec3 _value) noexcept override;
	void translate(const glm::vec3 _value) noexcept override;

	void setParent(entityBase *_parent) noexcept;
	void renderOnlyColour(const bool _state) noexcept;
	void setScale(const glm::vec3 _value) noexcept;
	void sentTint(const colour _colour) noexcept;

	_NODISCARD constexpr entityBase &getParent() const noexcept;
};

/** Root is a special, static entity that only has children */
class root : public entityBase
{
public:
	_NODISCARD static root *getRoot() noexcept
	{
		static root *s_root = new root();
		return s_root;
	}

	~root() = default;

private:
	root() = default;
	// Delete copy/move so extra instances can't be created/moved.
	root(root const&) = delete;
	root &operator=(root const&) = delete;
	root(root&&) = delete;
	root &operator=(root&&) = delete;
};
}
