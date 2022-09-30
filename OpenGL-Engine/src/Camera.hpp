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
		Camera(float pAspectRatio,				const glm::mat4& pTransform);
		Camera(float pAspectRatio, float pFovH, const glm::mat4& pTransform);
		Camera(float pAspectRatio,				const glm::vec3& pFrom, const glm::vec3& pTo,
		 const glm::vec3& pUp = { 0, 1, 0 });
		Camera(float pAspectRatio, float pFovH, const glm::vec3& pFrom, const glm::vec3& pTo,
		 const glm::vec3& pUp = { 0, 1, 0 });
		#pragma endregion

		void LookAt(const glm::vec3& pFrom, const glm::vec3& pTo, const glm::vec3& pUp = { 0, 1, 0 });

		void ModifyFovH(float pValue) noexcept;
		void ModifyFovV(float pValue) noexcept;
		void UpdateFovH() noexcept;
		void UpdateFovV() noexcept;

		#pragma region Setters
		void SetTransform(const glm::mat4& pValue) noexcept override;
		void SetView(const glm::mat4& pValue) noexcept;
		void SetProjection(float pFovV) noexcept;
		constexpr void SetProjection(const glm::mat4& pValue) noexcept { m_projection = pValue; }
		constexpr void SetAspectRatio(float pAspectRatio) noexcept { m_aspectRatio = pAspectRatio; }

		void SetPosition(const glm::vec3& pValue) noexcept override;
		void Translate(const glm::vec3& pValue) noexcept override;
		void SetAxes(const glm::vec3& pRight, const glm::vec3& pUp, const glm::vec3& pForward) noexcept override;

		void SetFovH(float pFovH) noexcept;
		void SetFovV(float pFovV) noexcept;
		#pragma endregion

		#pragma region Getters
		glm::mat4 GetWorldToCameraMatrix() const noexcept;
		constexpr glm::mat4 GetView() const noexcept { return m_view; }
		constexpr glm::mat4 GetProjection() const noexcept { return m_projection; }
		#pragma endregion
	};
}
