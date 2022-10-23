#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Transform
	{
	private:
		glm::mat4 m_transform;

	public:
#		pragma region Constructors
		Transform();
		Transform(const glm::mat4& pValue);
		virtual ~Transform() {}

		// Copy, takes lvalue
		Transform(const Transform& pOther);
		Transform& operator=(const Transform& pOther);
		// Move, take rvalue
		Transform(Transform&& pOther) noexcept;
		Transform& operator=(Transform&& pOther) noexcept;
#		pragma endregion

#		pragma region Setters
		virtual void SetTransform(const glm::mat4& pValue) noexcept;
		virtual void SetPosition(const glm::vec3& pValue) noexcept;
		virtual void Translate(const glm::vec3& pValue) noexcept;
		virtual void SetAxes(const glm::vec3& pRight, const glm::vec3& pUp, const glm::vec3& pForward) noexcept;
		virtual void SetRight(const glm::vec3& pRight) noexcept;
		virtual void SetUp(const glm::vec3& pUp) noexcept;
		virtual void SetForward(const glm::vec3& pForward) noexcept;
#		pragma endregion

#		pragma region Getters
		constexpr const glm::mat4& GetTransform() const noexcept { return m_transform; }
		constexpr virtual glm::vec4 GetPosition() const noexcept { return m_transform[3]; }
		constexpr virtual glm::vec4 GetRight() const noexcept;
		constexpr virtual glm::vec4 GetUp() const noexcept;
		constexpr virtual glm::vec4 GetForward() const noexcept;
#		pragma endregion
	};
}
