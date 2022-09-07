#pragma region
#pragma once
#include "glm/glm.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

#pragma endregion

namespace Engine
{
	class Transform
	{
	protected:
		mat4 m_transform = mat4(1);

	public:
		#pragma region Constructors
		Transform();
		Transform(mat4 pValue);

		Transform(const Transform& pOther);
		Transform(Transform&& pOther) noexcept;
		Transform& operator=(const Transform& pOther);
		Transform& operator=(Transform&& pOther) noexcept;
		#pragma endregion
		#pragma region Setters
		virtual void SetTransform(mat4 pValue);
		virtual void SetPosition(vec3 pValue);
		virtual void Translate(vec3 pValue);
		virtual void SetAxes(vec3 pRight, vec3 pUp, vec3 pForward);
		virtual void SetRight(vec3 pRight);
		virtual void SetUp(vec3 pUp);
		virtual void SetForward(vec3 pForward);
		#pragma endregion
		#pragma region Getters
		virtual mat4 GetTransform() const;
		virtual vec4 GetPosition() const;
		virtual vec3 GetRight() const;
		virtual vec3 GetUp() const;
		virtual vec3 GetForward() const;
		#pragma endregion
	};
}
