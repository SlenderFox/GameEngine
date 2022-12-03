#pragma region
#include "camera.hpp"
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

namespace engine
{
	#pragma region Constructors
	camera::camera(float inAspectRatio) noexcept
	{
		mat4 id = mat4(1.0f);
		setAspectRatio(inAspectRatio);
		setTransform(&id);
		setFovH(75.0f);
	}

	camera::camera(
		float inAspectRatio,
		float inFovH
	) noexcept
	{
		mat4 id = mat4(1.0f);
		setAspectRatio(inAspectRatio);
		setTransform(&id);
		setFovH(inFovH);
	}

	camera::camera(
		float inAspectRatio,
		mat4 inTransform
	) noexcept
	{
		setAspectRatio(inAspectRatio);
		setTransform(&inTransform);
		setFovH(75.0f);
	}

	camera::camera(
		float inAspectRatio,
		float inFovH,
		mat4 inTransform
	) noexcept
	{
		setAspectRatio(inAspectRatio);
		setTransform(&inTransform);
		setFovH(inFovH);
	}

	camera::camera(
		float inAspectRatio,
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		setAspectRatio(inAspectRatio);
		lookAt(inFrom, inTo, inUp);
		setFovH(75.0f);
	}

	camera::camera(
		float inAspectRatio,
		float inFovH,
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		setAspectRatio(inAspectRatio);
		lookAt(inFrom, inTo, inUp);
		setFovH(inFovH);
	}
	#pragma endregion

	void camera::lookAt(
		vec3 inFrom,
		vec3 inTo,
		vec3 inUp
	) noexcept
	{
		m_view = glm::lookAt(inFrom, inTo, inUp);
		setView(&m_view);
	}

	void camera::modifyFovH(float inValue) noexcept
	{
		m_fovH += inValue;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		updateFovV();
	}

	void camera::modifyFovV(float inValue) noexcept
	{
		m_fovV += inValue;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		updateFovH();
	}

	void camera::updateFovH() noexcept
	{
		m_fovH = degrees(2 * atan(tan(radians(m_fovV) * 0.5f) * m_aspectRatio));
		setProjection(m_fovV);
	}

	void camera::updateFovV() noexcept
	{
		m_fovV = degrees(2 * atan(tan(radians(m_fovH) * 0.5f) * (1 / m_aspectRatio)));
		setProjection(m_fovV);
	}

	#pragma region Setters
	void camera::setTransform(const mat4 *inValue) noexcept
	{
		transform::setTransform(inValue);
		m_view = inverse(getTransform());
	}

	void camera::setView(mat4 *inValue) noexcept
	{
		m_view = *inValue;
		mat4 inv = inverse(m_view);
		transform::setTransform(&inv);
	}

	void camera::setProjection(float inFovV) noexcept
	{
		m_projection = perspective(radians(inFovV), m_aspectRatio, 0.1f, 500.0f);
	}

	constexpr void camera::setProjection(glm::mat4 *inValue) noexcept
	{ m_projection = *inValue; }

	constexpr void camera::setAspectRatio(float inAspectRatio) noexcept
	{ m_aspectRatio = inAspectRatio; }

	void camera::setPosition(vec3 inValue) noexcept
	{
		transform::setPosition(inValue);
		m_view = inverse(getTransform());
	}

	void camera::translate(vec3 inValue) noexcept
	{
		transform::translate(inValue);
		m_view = inverse(getTransform());
	}

	void camera::setAxes(
		vec3 pRight,
		vec3 inUp,
		vec3 pForward
	) noexcept
	{
		transform::setAxes(pRight, inUp, pForward);
		m_view = inverse(getTransform());
	}

	void camera::setFovH(float inFovH) noexcept
	{
		m_fovH = inFovH;
		if (m_fovH > 120.0f)
			m_fovH = 120.0f;
		else if (m_fovH < 1.0f)
			m_fovH = 1.0f;
		updateFovV();
	}

	void camera::setFovV(float inFovV) noexcept
	{
		m_fovV = inFovV;
		if (m_fovV > 120.0f)
			m_fovV = 120.0f;
		else if (m_fovV < 1.0f)
			m_fovV = 1.0f;
		updateFovH();
	}
	#pragma endregion

	mat4 camera::getWorldToCameraMatrix() const noexcept
	{ return m_projection * m_view; }

	constexpr glm::mat4 camera::getView() const noexcept
	{ return m_view; }

	constexpr glm::mat4 camera::getProjection() const noexcept
	{ return m_projection; }
}
