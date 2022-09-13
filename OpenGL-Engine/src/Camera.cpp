#pragma region
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
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
		SetView(m_view);
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
		Transform::SetTransform(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetView(mat4 pValue)
	{
		m_view = pValue;
		Transform::SetTransform(inverse(m_view));
	}

	void Camera::SetProjection(mat4 pValue)
	{
		m_projection = pValue;
	}

	void Camera::SetProjection(float pFovV)
	{
		m_projection = perspective(radians(pFovV), m_aspectRatio, 0.1f, 500.0f);
	}

	void Camera::SetPosition(vec3 pValue)
	{
		Transform::SetPosition(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::Translate(vec3 pValue)
	{
		Transform::Translate(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetAxes(vec3 pRight, vec3 pUp, vec3 pForward)
	{
		Transform::SetAxes(pRight, pUp, pForward);
		m_view = inverse(GetTransform());
	}

	void Camera::SetRight(vec3 pRight)
	{
		vec3 forward = cross(pRight, vec3(0, 1, 0));
		vec3 up = cross(pRight, forward);
		SetAxes(pRight, up, forward);
	}

	void Camera::SetUp(vec3 pUp)
	{
		vec3 right = cross(pUp, vec3(0, 0, 1));
		vec3 forward = cross(pUp, right);
		SetAxes(right, pUp, forward);
	}

	void Camera::SetForward(vec3 pForward)
	{
		vec3 right = cross(vec3(0, 1, 0), pForward);
		vec3 up = cross(pForward, right);
		SetAxes(right, up, pForward);
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

	mat4 Camera::GetWorldToCameraMatrix()
	{
		return m_projection * m_view;
	}
	#pragma endregion
}
