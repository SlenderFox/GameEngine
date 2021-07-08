#include "Camera.h"

Camera::Camera()
{
	m_localTransform = mat4(1.0f);
	m_localUp = vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(mat4 pTransform)
{
	m_localTransform = pTransform;
	m_localUp = vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
{
	m_localTransform = glm::lookAt(pFrom, pTo, pUp);
	m_localUp = pUp;
}

void Camera::LookAt(vec3 pFrom, vec3 pTo, vec3 pUp)
{
	m_localTransform = glm::lookAt(pFrom, pTo, pUp);
	m_localUp = pUp;
}

void Camera::LookAt(vec3 pFrom, vec3 pTo)
{
	m_localTransform = glm::lookAt(pFrom, pTo, m_localUp);
}
