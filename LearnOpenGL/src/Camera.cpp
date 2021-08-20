#include "Camera.h"
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
using glm::lookAt;
using glm::inverse;

namespace Engine
{
	Camera::Camera()
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		SetTransform(mat4(1.0f));
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(75.0f);
	}

	Camera::Camera(float pFovH)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		SetTransform(mat4(1.0f));
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(pFovH);
	}

	Camera::Camera(mat4 pTransform)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		SetTransform(pTransform);
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(75.0f);
	}

	Camera::Camera(float pFovH, mat4 pTransform)
	{
		m_localUp = vec3(0.0f, 1.0f, 0.0f);
		SetTransform(pTransform);
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(pFovH);
	}

	Camera::Camera(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(75.0f);
	}

	Camera::Camera(float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
	{
		LookAt(pFrom, pTo, pUp);
		// Defaults to 16:9 aspect ratio
		UpdateAspectRatio(16.0f, 9.0f);
		SetFovH(pFovH);
	}

	void Camera::UpdateAspectRatio(float pWidth, float pHeight)
	{
		m_aspectRatio = pWidth / pHeight;
		m_invAspectRatio = pHeight / pWidth;
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

	void Camera::ModifyFovH(float pValue)
	{
		m_fovH += pValue;
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
		m_fovV = degrees(2 * atan(tan(radians(m_fovH) * 0.5f) * m_invAspectRatio));
		SetProjection(m_fovV);
	}

	void Camera::LookAt(vec3 pFrom, vec3 pTo, vec3 pUp)
	{
		m_localUp = pUp;
		m_view = lookAt(pFrom, pTo, pUp);
	}

	void Camera::LookAt(vec3 pFrom, vec3 pTo)
	{
		m_view = lookAt(pFrom, pTo, m_localUp);
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
		m_view = inverse(m_localTransform);
	}

	mat4 Camera::GetView() const
	{
		return m_view;
	}

	void Camera::SetView(mat4 pValue)
	{
		m_view = pValue;
		m_localTransform = inverse(m_view);
	}

	mat4 Camera::GetProjection() const
	{
		return m_projection;
	}

	void Camera::SetProjection(mat4 pValue)
	{
		m_projection = pValue;
	}

	void Camera::SetProjection(float pFovV)
	{
		m_projection = perspective(radians(pFovV), m_aspectRatio, 0.1f, 100.0f);
#ifdef _DEBUG
		std::cout << "Field of view set to: " << m_fovH << "H, " << m_fovV << "V" << std::endl;
#endif
	}

	vec3 Camera::GetPosition() const
	{
		return (vec3)m_localTransform[3];
	}

	void Camera::SetPosition(vec3 pValue)
	{
		m_localTransform[3] = vec4(pValue, m_localTransform[3][3]);
		m_view = inverse(m_localTransform);
	}

	void Camera::Translate(vec3 pValue)
	{
		m_localTransform[3] = vec4((vec3)m_localTransform[3] + pValue, m_localTransform[3][3]);
		m_view = inverse(m_localTransform);
	}

	vec3 Camera::GetRight() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_localTransform[0];
	}

	void Camera::SetRight(vec3 pValue)
	{
		m_localTransform[0] = vec4(pValue, 0);
		m_view = inverse(m_localTransform);
	}

	vec3 Camera::GetUp() const
	{
		return (vec3)m_localTransform[1];
	}

	void Camera::SetUp(vec3 pValue)
	{
		m_localTransform[1] = vec4(pValue, 0);
		m_view = inverse(m_localTransform);
	}

	vec3 Camera::GetForward() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_localTransform[2];
	}

	void Camera::SetForward(vec3 pValue)
	{
		m_localTransform[2] = vec4(pValue, 0);
		m_view = inverse(m_localTransform);
	}
}
