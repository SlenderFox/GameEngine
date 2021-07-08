#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	Camera();
	Camera(mat4 pTransform);
	Camera(vec3 pFrom, vec3 pTo, vec3 pUp);
	~Camera() {}

	void LookAt(vec3 pFrom, vec3 pTo, vec3 pUp);
	void LookAt(vec3 pFrom, vec3 pTo);

	mat4 GetTransform() const { return m_localTransform; }
	void SetTransform(mat4 pValue) { m_localTransform = pValue; }
	void SetPosition(vec3 pValue) { m_localTransform[3] = glm::vec4(pValue, m_localTransform[3][3]); }

private:
	vec3 m_localUp;
	mat4 m_localTransform;
	mat4 m_view;
	mat4 m_projection;
};
