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
		Transform(glm::mat4 pValue);
		virtual ~Transform() {}

		// Copy, takes lvalue
		Transform(Transform const& pOther);
		Transform& operator=(Transform const& pOther);
		// Move, take rvalue
		Transform(Transform&& pOther) noexcept;
		Transform& operator=(Transform&& pOther) noexcept;
#		pragma endregion

#		pragma region Setters
		virtual constexpr void SetTransform(glm::mat4* pValue) noexcept { m_transform = *pValue; }
		virtual void SetPosition(	glm::vec3 pValue) noexcept;
		virtual void Translate(		glm::vec3 pValue) noexcept;
		virtual void SetAxes(		glm::vec3 pRight,
											glm::vec3 pUp,
											glm::vec3 pForward) noexcept;
		virtual void SetRight(		glm::vec3 pRight) noexcept;
		virtual void SetUp(			glm::vec3 pUp) noexcept;
		virtual void SetForward(	glm::vec3 pForward) noexcept;
#		pragma endregion

#		pragma region Getters
		constexpr glm::mat4 GetTransform() const noexcept { return m_transform; }
		constexpr glm::vec4 GetPosition() const noexcept { return m_transform[3]; }
		// Why the fuck do these three need to be virtual, nothing overrides them
		virtual constexpr glm::vec4 GetRight() const noexcept;
		virtual constexpr glm::vec4 GetUp() const noexcept;
		virtual constexpr glm::vec4 GetForward() const noexcept;
#		pragma endregion
	};
}
