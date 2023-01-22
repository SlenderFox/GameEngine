#pragma once
#include "transform.hpp"

namespace srender
{
	class camera: public transform
	{
	private:
		// The field of view of the camera in degrees, horizontal & vertical
		float m_fovH = 0, m_fovV = 0;
		float m_aspectRatio = 0;
		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		#pragma region Constructors
		camera(float inAspectRatio) noexcept;
		camera(
			float inAspectRatio,
			float inFovH
		) noexcept;
		camera(
			float inAspectRatio,
			glm::mat4 inTransform
		) noexcept;
		camera(
			float inAspectRatio,
			float inFovH,
			glm::mat4 inTransform
		) noexcept;
		camera(
			float inAspectRatio,
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;
		camera(
			float inAspectRatio,
			float inFovH,
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;
		#pragma endregion

		void lookAt(
			glm::vec3 inFrom,
			glm::vec3 inTo,
			glm::vec3 inUp = { 0, 1, 0 }
		) noexcept;

		void modifyFovH(float inValue) noexcept;
		void modifyFovV(float inValue) noexcept;
		void updateFovH() noexcept;
		void updateFovV() noexcept;

		#pragma region Setters
		void setTransform(const glm::mat4 *inValue) noexcept override;
		void setView(glm::mat4 *inValue) noexcept;
		void setProjection(float inFovV) noexcept;

		constexpr void setProjection(glm::mat4 *inValue) noexcept;
		constexpr void setAspectRatio(float inAspectRatio) noexcept;

		void setPosition(glm::vec3 inValue) noexcept override;
		void translate(glm::vec3 inValue) noexcept override;
		void setAxes(
			glm::vec3 inRight,
			glm::vec3 inUp,
			glm::vec3 inForward
		) noexcept override;

		void setFovH(float inFovH) noexcept;
		void setFovV(float inFovV) noexcept;
		#pragma endregion

		#pragma region Getters
		_NODISCARD glm::mat4 getWorldToCameraMatrix() const noexcept;

		_NODISCARD constexpr
		glm::mat4 getView() const noexcept;

		_NODISCARD constexpr
		glm::mat4 getProjection() const noexcept;
		#pragma endregion
	};
}