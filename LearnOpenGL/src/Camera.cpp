#pragma region
#include "Camera.hpp"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/gtc/matrix_transform.hpp>
#ifdef _DEBUG
 #include <iostream>
#endif

using glm::radians;
using glm::atan;
using glm::tan;
using glm::perspective;
using glm::degrees;
using glm::inverse;
#pragma endregion

namespace Engine
{
	#pragma region Constructors
	Camera::Camera(float pAspectRatio)
	{
		SetTransform(mat4(1.0f));
		SetAspectRatio(pAspectRatio);
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH)
	{
		SetTransform(mat4(1.0f));
		SetAspectRatio(pAspectRatio);
		SetFovH(pFovH);
	}

	Camera::Camera(float pAspectRatio, mat4 pTransform)
	{
		SetTransform(pTransform);
		SetAspectRatio(pAspectRatio);
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH, mat4 pTransform)
	{
		SetTransform(pTransform);
		SetAspectRatio(pAspectRatio);
		SetFovH(pFovH);
	}

	Camera::Camera(float pAspectRatio, vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		SetAspectRatio(pAspectRatio);
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		SetAspectRatio(pAspectRatio);
		SetFovH(pFovH);
	}
	#pragma endregion

	void Camera::LookAt(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		m_view = glm::lookAt(pFrom, pTo, pUp);
		m_transform = inverse(m_view);
	}

	void Camera::ModifyFovH(float pValue)
	{
		m_fovH += pValue;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::ModifyFovV(float pValue)
	{
		m_fovV += pValue;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		UpdateFovH();
	}

	void Camera::UpdateFovH()
	{
		m_fovH = degrees(2 * atan(tan(radians(m_fovV) * 0.5f) * m_aspectRatio));
		SetProjection(m_fovV);
	}

	void Camera::UpdateFovV()
	{
		m_fovV = degrees(2 * atan(tan(radians(m_fovH) * 0.5f) * (1 / m_aspectRatio)));
		SetProjection(m_fovV);
	}

	#pragma region Setters
	void Camera::SetTransform(mat4 pValue)
	{
		m_transform = pValue;
		m_view = inverse(m_transform);
	}

	void Camera::SetView(mat4 pValue)
	{
		m_view = pValue;
		m_transform = inverse(m_view);
	}

	void Camera::SetProjection(mat4 pValue)
	{
		m_projection = pValue;
	}

	void Camera::SetProjection(float pFovV)
	{
		m_projection = perspective(radians(pFovV), m_aspectRatio, 0.1f, 100.0f);
		// #ifdef _DEBUG
		//  std::cout << "Field of view set to: " << m_fovH << "H, " << m_fovV << "V" << std::endl;
		// #endif
	}

	void Camera::SetPosition(vec3 pValue)
	{
		m_transform[3] = vec4(pValue, m_transform[3][3]);
		m_view = inverse(m_transform);
	}

	void Camera::Translate(vec3 pValue)
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
		m_view = inverse(m_transform);
	}

	void Camera::SetRight(vec3 pValue)
	{
		m_transform[0] = vec4(pValue, 0);
		m_view = inverse(m_transform);
	}

	void Camera::SetUp(vec3 pValue)
	{
		m_transform[1] = vec4(pValue, 0);
		m_view = inverse(m_transform);
	}

	void Camera::SetForward(vec3 pValue)
	{
		m_transform[2] = vec4(pValue, 0);
		m_view = inverse(m_transform);
	}
	
	void Camera::SetAspectRatio(float pAspectRatio)
	{
		m_aspectRatio = pAspectRatio;
	}

	void Camera::SetFovH(float pFovH)
	{
		m_fovH = pFovH;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::SetFovV(float pFovV)
	{
		m_fovV = pFovV;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		UpdateFovH();
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
	#pragma endregion
	#pragma region Getters
	mat4 Camera::GetView() const
	{
		return m_view;
	}

	mat4 Camera::GetProjection() const
	{
		return m_projection;
	}

	vec3 Camera::GetRight() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_transform[0];
	}

	vec3 Camera::GetUp() const
	{
		return (vec3)m_transform[1];
	}

	vec3 Camera::GetForward() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_transform[2];
	}
	
	mat4 Camera::GetWorldToCameraMatrix()
	{
		return m_projection * m_view;
	}
	#pragma endregion
}
