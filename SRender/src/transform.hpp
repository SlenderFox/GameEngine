#pragma once
#include "glm/glm.hpp"

namespace srender
{
	class transform
	{
		glm::mat4 m_transform;

	public:
		transform() noexcept;
		transform(const glm::mat4 inValue) noexcept;
		virtual ~transform() = default;
		// Copy, takes lvalue
		transform(transform const &inOther) noexcept;
		transform &operator=(transform const &inOther) noexcept;
		// Move, take rvalue
		transform(transform&& inOther) noexcept;
		transform &operator=(transform&& inOther) noexcept;

		virtual void setTransform(const glm::mat4 *inValue) noexcept;
		virtual void setPosition(const glm::vec3 inValue) noexcept;
		virtual void translate(const glm::vec3 inValue) noexcept;
		virtual void setAxes(
			const glm::vec3 inRight,
			const glm::vec3 inUp,
			const glm::vec3 inForward
		) noexcept;
		virtual void setRight(const glm::vec3 inRight) noexcept;
		virtual void setUp(const glm::vec3 inUp) noexcept;
		virtual void setForward(const glm::vec3 inForward) noexcept;

		_NODISCARD glm::mat4 getTransform() const noexcept;
		_NODISCARD glm::vec4 getPosition() const noexcept;
		_NODISCARD glm::vec4 getRight() const noexcept;
		_NODISCARD glm::vec4 getUp() const noexcept;
		_NODISCARD glm::vec4 getForward() const noexcept;
	};
}
