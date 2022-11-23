#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Transform
	{
		glm::mat4 m_transform;

	public:
		#pragma region Constructors
		constexpr Transform(): m_transform(glm::mat4(1)) {}
		constexpr Transform(const glm::mat4 pValue): m_transform(pValue) {}
		virtual ~Transform() {}

		// Copy, takes lvalue
		constexpr Transform(Transform const& pOther)
		{
			m_transform = pOther.m_transform;
		}
		constexpr Transform& operator=(Transform const& pOther)
		{
			this->m_transform = pOther.m_transform;
			return *this;
		}
		// Move, take rvalue
		constexpr Transform(Transform&& pOther) noexcept
		{
			m_transform = std::move(pOther.m_transform);
		}
		constexpr Transform& operator=(Transform&& pOther) noexcept
		{
			this->m_transform = std::move(pOther.m_transform);
			return *this;
		}
		#pragma endregion

		#pragma region Setters
		virtual constexpr void SetTransform(const glm::mat4* pValue) noexcept { m_transform = *pValue; }
		virtual void SetPosition(const glm::vec3 pValue) noexcept;
		virtual void Translate(const glm::vec3 pValue) noexcept;
		virtual void SetAxes(
			const glm::vec3 pRight,
			const glm::vec3 pUp,
			const glm::vec3 pForward) noexcept;
		virtual void SetRight(const glm::vec3 pRight) noexcept;
		virtual void SetUp(const glm::vec3 pUp) noexcept;
		virtual void SetForward(const glm::vec3 pForward) noexcept;
		#pragma endregion

		#pragma region Getters
		constexpr glm::mat4 GetTransform() const noexcept { return m_transform; }
		constexpr glm::vec4 GetPosition() const noexcept { return m_transform[3]; }
		glm::vec4 GetRight() const noexcept;
		glm::vec4 GetUp() const noexcept;
		glm::vec4 GetForward() const noexcept;
		#pragma endregion
	};
}
