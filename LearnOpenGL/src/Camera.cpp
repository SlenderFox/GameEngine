#include "Camera.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/gtc/matrix_transform.hpp>
#ifdef _DEBUG
 #include <iostream>
#endif

namespace Engine
{
	Camera::Camera()
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		m_localTransform = mat4(1.0f);
		SetFovH((16.0f / 9.0f), (9.0f / 16.0f), glm::radians(75.0f));
	}

	Camera::Camera(float pAspect, float pInvAspect)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		m_localTransform = mat4(1.0f);
		SetFovH(pAspect, pInvAspect, glm::radians(75.0f));
	}

	Camera::Camera(float pAspect, float pInvAspect, float pFovH)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		m_localTransform = mat4(1.0f);
		SetFovH(pAspect, pInvAspect, pFovH);
	}

	Camera::Camera(float pAspect, float pInvAspect, mat4 pTransform)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		m_localTransform = pTransform;
		SetFovH(pAspect, pInvAspect, glm::radians(75.0f));
	}

	Camera::Camera(float pAspect, float pInvAspect, float pFovH, mat4 pTransform)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		m_localTransform = pTransform;
		SetFovH(pAspect, pInvAspect, pFovH);
	}

	Camera::Camera(float pAspect, float pInvAspect, vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		SetFovH(pAspect, pInvAspect, glm::radians(75.0f));
	}

	Camera::Camera(float pAspect, float pInvAspect, float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		SetFovH(pAspect, pInvAspect, pFovH);
	}

	void Camera::SetFovH(float pAspect, float pInvAspect, float pFovH)
	{
		m_fovH = pFovH;
		UpdateFovV(pAspect, pInvAspect);
	}

	void Camera::SetFovV(float pAspect, float pInvAspect, float pFovV)
	{
		m_fovV = pFovV;
		UpdateFovH(pAspect, pInvAspect);
	}

	void Camera::UpdateFovH(float pAspect, float pInvAspect)
	{
		m_fovH = 2 * glm::atan(glm::tan(m_fovV * 0.5f) * pAspect);
		m_projection = glm::perspective(m_fovV, pAspect, 0.1f, 100.0f);
#ifdef _DEBUG
		std::cout << "Field of view set to: " << glm::degrees(m_fovH) << "H, " << glm::degrees(m_fovV) << "V" << std::endl;
#endif
	}

	void Camera::UpdateFovV(float pAspect, float pInvAspect)
	{
		m_fovV = 2 * glm::atan(glm::tan(m_fovH * 0.5f) * pInvAspect);
		m_projection = glm::perspective(m_fovV, pAspect, 0.1f, 100.0f);
#ifdef _DEBUG
		std::cout << "Field of view set to: " << glm::degrees(m_fovH) << "H, " << glm::degrees(m_fovV) << "V" << std::endl;
#endif
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
}
