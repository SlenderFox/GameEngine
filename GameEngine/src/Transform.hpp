#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Transform
	{
		glm::mat4 m_transform;

	public:
		Transform() noexcept;
		Transform(const glm::mat4 inValue) noexcept;
		virtual ~Transform() = default;
		// Copy, takes lvalue
		Transform(Transform const& inOther) noexcept;
		Transform& operator=(Transform const& inOther) noexcept;
		// Move, take rvalue
		Transform(Transform&& inOther) noexcept;
		Transform& operator=(Transform&& inOther) noexcept;

		virtual void SetTransform(const glm::mat4* inValue) noexcept;
		virtual void SetPosition(const glm::vec3 inValue) noexcept;
		virtual void Translate(const glm::vec3 inValue) noexcept;
		virtual void SetAxes(
			const glm::vec3 inRight,
			const glm::vec3 inUp,
			const glm::vec3 inForward
		) noexcept;
		virtual void SetRight(const glm::vec3 inRight) noexcept;
		virtual void SetUp(const glm::vec3 inUp) noexcept;
		virtual void SetForward(const glm::vec3 inForward) noexcept;

		_NODISCARD glm::mat4 GetTransform() const noexcept;
		_NODISCARD glm::vec4 GetPosition() const noexcept;
		_NODISCARD glm::vec4 GetRight() const noexcept;
		_NODISCARD glm::vec4 GetUp() const noexcept;
		_NODISCARD glm::vec4 GetForward() const noexcept;
	};
}
