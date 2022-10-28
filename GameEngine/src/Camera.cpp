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
#	pragma region Constructors
	Camera::Camera(float pAspectRatio)
	{
		SetAspectRatio(pAspectRatio);
		SetTransform(mat4(1.0f));
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH)
	{
		SetAspectRatio(pAspectRatio);
		SetTransform(mat4(1.0f));
		SetFovH(pFovH);
	}

	Camera::Camera(float pAspectRatio, const mat4& pTransform)
	{
		SetAspectRatio(pAspectRatio);
		SetTransform(pTransform);
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH, const mat4& pTransform)
	{
		SetAspectRatio(pAspectRatio);
		SetTransform(pTransform);
		SetFovH(pFovH);
	}

	Camera::Camera(float pAspectRatio, const vec3& pFrom, const vec3& pTo, const vec3& pUp)
	{
		SetAspectRatio(pAspectRatio);
		LookAt(pFrom, pTo, pUp);
		SetFovH(75.0f);
	}

	Camera::Camera(float pAspectRatio, float pFovH, const vec3& pFrom, const vec3& pTo, const vec3& pUp)
	{
		SetAspectRatio(pAspectRatio);
		LookAt(pFrom, pTo, pUp);
		SetFovH(pFovH);
	}
#	pragma endregion

	void Camera::LookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp)
	{
		m_view = glm::lookAt(pFrom, pTo, pUp);
		SetView(m_view);
	}

	void Camera::ModifyFovH(float pValue) noexcept
	{
		m_fovH += pValue;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::ModifyFovV(float pValue) noexcept
	{
		m_fovV += pValue;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		UpdateFovH();
	}

	void Camera::UpdateFovH() noexcept
	{
		m_fovH = degrees(2 * atan(tan(radians(m_fovV) * 0.5f) * m_aspectRatio));
		SetProjection(m_fovV);
	}

	void Camera::UpdateFovV() noexcept
	{
		m_fovV = degrees(2 * atan(tan(radians(m_fovH) * 0.5f) * (1 / m_aspectRatio)));
		SetProjection(m_fovV);
	}

#	pragma region Setters
	void Camera::SetTransform(const mat4& pValue) noexcept
	{
		Transform::SetTransform(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetView(const mat4& pValue) noexcept
	{
		m_view = pValue;
		Transform::SetTransform(inverse(m_view));
	}

	void Camera::SetProjection(float pFovV) noexcept
	{
		m_projection = perspective(radians(pFovV), m_aspectRatio, 0.1f, 500.0f);
	}

	void Camera::SetPosition(const vec3& pValue) noexcept
	{
		Transform::SetPosition(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::Translate(const vec3& pValue) noexcept
	{
		Transform::Translate(pValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetAxes(const vec3& pRight, const vec3& pUp, const vec3& pForward) noexcept
	{
		Transform::SetAxes(pRight, pUp, pForward);
		m_view = inverse(GetTransform());
	}

	void Camera::SetFovH(float pFovH) noexcept
	{
		m_fovH = pFovH;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::SetFovV(float pFovV) noexcept
	{
		m_fovV = pFovV;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		UpdateFovH();
	}
#	pragma endregion

	mat4 Camera::GetWorldToCameraMatrix() const noexcept
	{
		return m_projection * m_view;
	}
}