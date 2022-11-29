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
	Camera::Camera(float inAspectRatio) noexcept
	{
		mat4 id = mat4(1.0f);
		SetAspectRatio(inAspectRatio);
		SetTransform(&id);
		SetFovH(75.0f);
	}

	Camera::Camera(
		float inAspectRatio,
		float inFovH
	) noexcept
	{
		mat4 id = mat4(1.0f);
		SetAspectRatio(inAspectRatio);
		SetTransform(&id);
		SetFovH(inFovH);
	}

	Camera::Camera(
		float inAspectRatio,
		mat4 inTransform
	) noexcept
	{
		SetAspectRatio(inAspectRatio);
		SetTransform(&inTransform);
		SetFovH(75.0f);
	}

	Camera::Camera(
		float inAspectRatio,
		float inFovH,
		mat4 inTransform
	) noexcept
	{
		SetAspectRatio(inAspectRatio);
		SetTransform(&inTransform);
		SetFovH(inFovH);
	}

	Camera::Camera(
		float inAspectRatio,
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		SetAspectRatio(inAspectRatio);
		LookAt(inFrom, inTo, inUp);
		SetFovH(75.0f);
	}

	Camera::Camera(
		float inAspectRatio,
		float inFovH,
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		SetAspectRatio(inAspectRatio);
		LookAt(inFrom, inTo, inUp);
		SetFovH(inFovH);
	}
	#pragma endregion

	void Camera::LookAt(
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		m_view = glm::lookAt(inFrom, inTo, inUp);
		SetView(&m_view);
	}

	void Camera::ModifyFovH(float inValue) noexcept
	{
		m_fovH += inValue;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::ModifyFovV(float inValue) noexcept
	{
		m_fovV += inValue;
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

	#pragma region Setters
	void Camera::SetTransform(const mat4* inValue) noexcept
	{
		Transform::SetTransform(inValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetView(mat4* inValue) noexcept
	{
		m_view = *inValue;
		mat4 inv = inverse(m_view);
		Transform::SetTransform(&inv);
	}

	void Camera::SetProjection(float inFovV) noexcept
	{
		m_projection = perspective(radians(inFovV), m_aspectRatio, 0.1f, 500.0f);
	}

	constexpr void Camera::SetProjection(glm::mat4* inValue) noexcept
	{ m_projection = *inValue; }

	constexpr void Camera::SetAspectRatio(float inAspectRatio) noexcept
	{ m_aspectRatio = inAspectRatio; }

	void Camera::SetPosition(vec3 inValue) noexcept
	{
		Transform::SetPosition(inValue);
		m_view = inverse(GetTransform());
	}

	void Camera::Translate(vec3 inValue) noexcept
	{
		Transform::Translate(inValue);
		m_view = inverse(GetTransform());
	}

	void Camera::SetAxes(
		vec3 pRight,
		vec3 inUp,
		vec3 pForward
	) noexcept
	{
		Transform::SetAxes(pRight, inUp, pForward);
		m_view = inverse(GetTransform());
	}

	void Camera::SetFovH(float inFovH) noexcept
	{
		m_fovH = inFovH;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		UpdateFovV();
	}

	void Camera::SetFovV(float inFovV) noexcept
	{
		m_fovV = inFovV;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		UpdateFovH();
	}
	#pragma endregion

	mat4 Camera::GetWorldToCameraMatrix() const noexcept
	{ return m_projection * m_view; }

	constexpr glm::mat4 Camera::GetView() const noexcept
	{ return m_view; }

	constexpr glm::mat4 Camera::GetProjection() const noexcept
	{ return m_projection; }
}
