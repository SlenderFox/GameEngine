#include "Camera.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers

Camera::Camera() : m_fovh(90)
{
	m_localUp = vec3(0.0f, 1.0f, 0.0f);
	m_localTransform = mat4(1.0f);
	float fovy = 2 * glm::atan(glm::tan(m_fovh * 0.5f));
	m_projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	// Test
	m_localTransform = glm::translate(m_localTransform, vec3(0.0f, 0.0f, -6.0f));
}

Camera::Camera(float pFovh) : m_fovh(pFovh)
{
	m_localUp = vec3(0.0f, 1.0f, 0.0f);
	m_localTransform = mat4(1.0f);
	m_projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
}

Camera::Camera(mat4 pTransform) : m_fovh(90)
{
	m_localUp = vec3(0.0f, 1.0f, 0.0f);
	m_localTransform = pTransform;
	m_projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
}

Camera::Camera(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 }) : m_fovh(90)
{
	LookAt(pFrom, pTo, pUp);
	m_projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
}

void Camera::LookAt(vec3 pFrom, vec3 pTo, vec3 pUp)
{
	m_localUp = pUp;
	m_localTransform = glm::lookAt(pFrom, pTo, pUp);
}

void Camera::LookAt(vec3 pFrom, vec3 pTo)
{
	m_localTransform = glm::lookAt(pFrom, pTo, m_localUp);
}

void Camera::SetClearColour(vec4 pValue)
{
	glClearColor(pValue.x, pValue.y, pValue.z, pValue.a);
}

void Camera::SetClearColour(vec3 pValue)
{
	glClearColor(pValue.x, pValue.y, pValue.z, 1.0f);
}

void Camera::SetClearColour(vec3 pValue, float pA)
{
	glClearColor(pValue.x, pValue.y, pValue.z, pA);
}

void Camera::SetClearColour(float pRed, float pGreen, float pBlue)
{
	glClearColor(pRed, pGreen, pBlue, 1.0f);
}

void Camera::SetClearColour(float pRed, float pGreen, float pBlue, float pAlpha)
{
	glClearColor(pRed, pGreen, pBlue, pAlpha);
}

mat4 Camera::WorldToCameraMatrix()
{
	return m_projection * m_view;
}

mat4 Camera::GetTransform() const
{
	return m_localTransform;
}

void Camera::SetTransform(mat4 pValue)
{
	m_localTransform = pValue;
}

void Camera::SetPosition(vec3 pValue)
{
	m_localTransform[3] = glm::vec4(pValue, m_localTransform[3][3]);
}

mat4 Camera::GetView() const
{
	return m_view;
}

void Camera::SetView(mat4 pValue)
{
	m_view = pValue;
}

mat4 Camera::GetProjection() const
{
	return m_projection;
}

void Camera::SetProjection(mat4 pValue)
{
	m_projection = pValue;
}
