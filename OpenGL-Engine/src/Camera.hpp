#pragma once
#include "Transform.hpp"

namespace Engine
{
	class Camera: public Transform
	{
	private:
		float m_fovH = 0, m_fovV = 0;	// The field of view of the camera in degrees, horizontal & vertical
		float m_aspectRatio = 0;
		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		#pragma region Constructors
		Camera(float pAspectRatio);
		Camera(float pAspectRatio, float pFovH);
		Camera(float pAspectRatio, glm::mat4 pTransform);
		Camera(float pAspectRatio, float pFovH, glm::mat4 pTransform);
		Camera(float pAspectRatio, glm::vec3 pFrom, glm::vec3 pTo, glm::vec3 pUp);
		Camera(float pAspectRatio, float pFovH, glm::vec3 pFrom, glm::vec3 pTo, glm::vec3 pUp);
		#pragma endregion

		void LookAt(glm::vec3 pFrom, glm::vec3 pTo, glm::vec3 pUp);

		void ModifyFovH(float pValue) noexcept;
		void ModifyFovV(float pValue) noexcept;
		void UpdateFovH() noexcept;
		void UpdateFovV() noexcept;

		#pragma region Setters
		void SetTransform(glm::mat4 pValue) noexcept override;
		void SetView(glm::mat4 pValue) noexcept;
		void SetProjection(float pFovV) noexcept;

		void SetPosition(glm::vec3 pValue) noexcept override;
		void Translate(glm::vec3 pValue) noexcept override;
		void SetAxes(glm::vec3 pRight, glm::vec3 pUp, glm::vec3 pForward) noexcept override;

		void SetFovH(float pFovH) noexcept;
		void SetFovV(float pFovV) noexcept;
		#pragma endregion

		glm::mat4 GetWorldToCameraMatrix() const noexcept;

		#pragma region Constexpr
		constexpr void SetProjection(glm::mat4 pValue) noexcept { m_projection = pValue; }
		constexpr void SetAspectRatio(float pAspectRatio) noexcept { m_aspectRatio = pAspectRatio; }
		constexpr glm::mat4 GetView() const noexcept { return m_view; }
		constexpr glm::mat4 GetProjection() const noexcept { return m_projection; }
		#pragma endregion
	};
}
