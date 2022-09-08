#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Transform
	{
	// TODO: Make m_transform private and the rest protected
	protected:
		glm::mat4 m_transform = glm::mat4(1);

	public:
		#pragma region Constructors
		Transform();
		Transform(glm::mat4 pValue);

		Transform(const Transform& pOther);
		Transform(Transform&& pOther) noexcept;
		Transform& operator=(const Transform& pOther);
		Transform& operator=(Transform&& pOther) noexcept;
		#pragma endregion
		#pragma region Setters
		virtual void SetTransform(glm::mat4 pValue);
		virtual void SetPosition(glm::vec3 pValue);
		virtual void Translate(glm::vec3 pValue);
		virtual void SetAxes(glm::vec3 pRight, glm::vec3 pUp, glm::vec3 pForward);
		virtual void SetRight(glm::vec3 pRight);
		virtual void SetUp(glm::vec3 pUp);
		virtual void SetForward(glm::vec3 pForward);
		#pragma endregion
		#pragma region Getters
		glm::mat4 GetTransform() const { return m_transform; }
		virtual glm::vec4 GetPosition() const;
		virtual glm::vec4 GetRight() const;
		virtual glm::vec4 GetUp() const;
		virtual glm::vec4 GetForward() const;
		#pragma endregion
	};
}
