#pragma once
#include "Transform.hpp"

namespace Engine
{
	class Camera: public Transform
	{
	private:
		// The field of view of the camera in degrees, horizontal & vertical
		float m_fovH = 0, m_fovV = 0;
		float m_aspectRatio = 0;
		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		#pragma region Constructors
		Camera(float inAspectRatio) noexcept;
		Camera(
			float inAspectRatio,
			float inFovH
		) noexcept;
		Camera(
			float inAspectRatio,
			glm::mat4 inTransform
		) noexcept;
		Camera(
			float inAspectRatio,
			float inFovH,
			glm::mat4 inTransform
		) noexcept;
		Camera(
			float inAspectRatio,
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;
		Camera(
			float inAspectRatio,
			float inFovH,
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;
		#pragma endregion

		void LookAt(
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;

		void ModifyFovH(float inValue) noexcept;
		void ModifyFovV(float inValue) noexcept;
		void UpdateFovH() noexcept;
		void UpdateFovV() noexcept;

		#pragma region Setters
		void SetTransform(const glm::mat4* inValue) noexcept override;
		void SetView(glm::mat4* inValue) noexcept;
		void SetProjection(float inFovV) noexcept;

		constexpr void SetProjection(glm::mat4* inValue) noexcept;
		constexpr void SetAspectRatio(float inAspectRatio) noexcept;

		void SetPosition(glm::vec3 inValue) noexcept override;
		void Translate(glm::vec3 inValue) noexcept override;
		void SetAxes(
			glm::vec3 inRight,
			glm::vec3 inUp,
			glm::vec3 inForward
		) noexcept override;

		void SetFovH(float inFovH) noexcept;
		void SetFovV(float inFovV) noexcept;
		#pragma endregion

		#pragma region Getters
		_NODISCARD glm::mat4 GetWorldToCameraMatrix() const noexcept;

		_NODISCARD constexpr
		glm::mat4 GetView() const noexcept;

		_NODISCARD constexpr
		glm::mat4 GetProjection() const noexcept;
		#pragma endregion
	};
}
