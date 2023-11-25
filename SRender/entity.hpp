#pragma once
#include "model.hpp"
#include "colour.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** An entity is any object in world space, each entity must have a parent
 * @todo Add a component system to attach things like model, camera, or light
 * @todo Look into ability to swap models
*/
class entity : public transform
{
	entity *m_parentRef = nullptr;
	model *m_modelRef = nullptr;
	std::vector<entity*> m_childrenRef = std::vector<entity*>();

	static void addToVector(std::vector<entity*> &_vector, entity *_child) noexcept;
	static void removeFromVector(std::vector<entity*> &_vector, const entity *_child) noexcept;

	void updateModel() const noexcept;

public:
	_NODISCARD static std::vector<entity*> getRootChildren() noexcept;

	entity();
	entity(entity *_parent);
	entity(
		std::string _modelPath,
		std::string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures = true
	);
	entity(
		entity *_parent,
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

	void setParent(entity *_parent) noexcept;
	void renderOnlyColour(const bool _state) noexcept;
	void setScale(const glm::vec3 _value) noexcept;
	void sentTint(const colour _colour) noexcept;

	void addChild(entity *_child) noexcept;
	void removeChild(const entity *_child) noexcept;

	_NODISCARD constexpr entity &getParent() const noexcept;
	_NODISCARD constexpr std::vector<entity*> getChildren() const noexcept;
};
}
