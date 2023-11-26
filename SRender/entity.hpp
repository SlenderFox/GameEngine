#pragma once
#include "light.hpp"
#include "model.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** An entity is any object in world space, each entity must have a parent.
 * @todo Add a component system to attach things like model, camera, or light.
 * @todo Look into ability to swap models.
*/
class entity
{
	/* Components */
	transform m_transform = transform(); // Currently mandatory
	light *m_light = nullptr; // Optional

	entity *m_parentRef = nullptr;
	model *m_modelRef = nullptr;

	/** Contains a list of all entities childed to this entity. */
	std::vector<entity*> m_childrenRef = std::vector<entity*>();

	static void addToVector(std::vector<entity*> &_vector, entity *_child) noexcept;
	static void removeFromVector(std::vector<entity*> &_vector, const entity *_child) noexcept;

	void updateModel() const noexcept;

public:
	_NODISCARD static std::vector<entity*> getRootChildren() noexcept;

	entity();
	entity(entity *_parent);
	~entity();

	/** Directly sets the transform matrix, calls updateModel
	 * @param _value The desired transform
	 */
	void setTransform(const glm::mat4 *_value) noexcept;
	/** Directly sets the position in the transform matrix, calls updateModel
	 * @param _value The desired position
	 */
	void setPosition(const glm::vec3 _value) noexcept;
	/** Directly sets the forward in the transform matrix, calls updateModel
	 * @param _value The desired forward
	 */
	void setForward(const glm::vec3 _value) noexcept;
	/** Applies a translation to the transform matrix, calls updateModel
	 * @param _value The translation vector
	 */
	void translate(const glm::vec3 _value) noexcept;

	void setParent(entity *_parent) noexcept;
	void renderOnlyColour(const bool _state) noexcept;
	void setScale(const glm::vec3 _value) noexcept;
	void sentTint(const colour _colour) noexcept;

	void addChild(entity *_child) noexcept;
	void removeChild(const entity *_child) noexcept;

	_NODISCARD entity &getParent() const noexcept;
	_NODISCARD std::vector<entity*> getChildren() const noexcept;

	_NODISCARD const transform getTransform() const noexcept;

	void componentModelLoad(
		std::string _modelPath,
		std::string _shaderPath,
		model *&_outModel,
		shader *&_outShader,
		const bool _loadTextures = true
	);

	void componentLightLoad(
		const light::type _type,
		const colour _colour = colour::white()
	) noexcept;

	_NODISCARD model *componentModelGet() const noexcept;
	_NODISCARD light *componentLightGet() const noexcept;
};
}
