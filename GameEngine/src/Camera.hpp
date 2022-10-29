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
#		pragma region Constructors
		Camera(float const& pAspectRatio);
		Camera(
			float const& pAspectRatio,
			float const& pFovH);
		Camera(
			float const& pAspectRatio,
			glm::mat4 const& pTransform);
		Camera(
			float const& pAspectRatio,
			float const& pFovH,
			glm::mat4 const& pTransform);
		Camera(
			float const& pAspectRatio,
			glm::vec3 const& pFrom,
			glm::vec3 const& pTo,
			glm::vec3 const& pUp = { 0, 1, 0 });
		Camera(
			float const& pAspectRatio,
			float const& pFovH,
			glm::vec3 const& pFrom,
			glm::vec3 const& pTo,
			glm::vec3 const& pUp = { 0, 1, 0 });
#		pragma endregion

		void LookAt(
			glm::vec3 const& pFrom,
			glm::vec3 const& pTo,
			glm::vec3 const& pUp = { 0, 1, 0 });

		void ModifyFovH(float const& pValue) noexcept;
		void ModifyFovV(float const& pValue) noexcept;
		void UpdateFovH() noexcept;
		void UpdateFovV() noexcept;

#		pragma region Setters
		void SetTransform(glm::mat4 const& pValue) noexcept override;
		void SetView(glm::mat4 const& pValue) noexcept;
		void SetProjection(float const& pFovV) noexcept;
		constexpr void SetProjection(glm::mat4 const& pValue) noexcept { m_projection = pValue; }
		constexpr void SetAspectRatio(float const& pAspectRatio) noexcept { m_aspectRatio = pAspectRatio; }

		void SetPosition(glm::vec3 const& pValue) noexcept override;
		void Translate(glm::vec3 const& pValue) noexcept override;
		void SetAxes(
			glm::vec3 const& pRight,
			glm::vec3 const& pUp,
			glm::vec3 const& pForward) noexcept override;

		void SetFovH(float const& pFovH) noexcept;
		void SetFovV(float const& pFovV) noexcept;
#		pragma endregion

#		pragma region Getters
		glm::mat4 GetWorldToCameraMatrix() const noexcept;
		constexpr glm::mat4 GetView() const noexcept { return m_view; }
		constexpr glm::mat4 GetProjection() const noexcept { return m_projection; }
#		pragma endregion
	};
}
