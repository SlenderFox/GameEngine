#pragma region
#pragma once
#include "Transform.hpp"
#pragma endregion

namespace Engine
{
	class Camera : public Transform
	{
	public:
		#pragma region Constructors
		Camera(float pAspectRatio);
		Camera(float pAspectRatio, float pFovH);
		Camera(float pAspectRatio, mat4 pTransform);
		Camera(float pAspectRatio, float pFovH, mat4 pTransform);
		Camera(float pAspectRatio, vec3 pFrom, vec3 pTo, vec3 pUp);
		Camera(float pAspectRatio, float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp);
		#pragma endregion

		~Camera() {}

		void LookAt(vec3 pFrom, vec3 pTo, vec3 pUp);

		/**
		 * @brief Adds the supplied value to the current fov
		 *
		 * @param pValue The value in radians that is added to the fov
		 */
		void ModifyFovH(float pValue);
		/**
		 * @brief Adds the supplied value to the current fov
		 *
		 * @param pValue The value in radians that is added to the fov
		 */
		void ModifyFovV(float pValue);
		/**
		 * @brief Updates the horizontal fov from the vertical given the new aspect ratio
		 * then creates a projection
		 */
		void UpdateFovH();
		/**
		 * @brief Updates the vertical fov from the horizontal given the inverse aspect ratio
		 * then creates a projection
		 */
		void UpdateFovV();

		#pragma region Setters
		void SetTransform(mat4 pValue) override;
		void SetView(mat4 pValue);
		void SetProjection(mat4 pValue);
		void SetProjection(float pFovV);

		void SetPosition(vec3 pValue) override;
		void Translate(vec3 pValue);

		void SetRight(vec3 pValue);
		void SetUp(vec3 pValue);
		void SetForward(vec3 pValue);

		/**
		 * @brief Sets the aspect ratio
		 *
		 * @param pAspectRatio The aspect ratio
		 */
		void SetAspectRatio(float pAspectRatio);
		/**
		 * @brief Directly sets the horizontal fov and calculates the corresponding vertical fov
		 *
		 * @param pFovH The desired horizontal fov in radians
		 */
		void SetFovH(float pFovH);
		/**
		 * @brief Directly sets the vertical fov and calculates the corresponding horizontal fov
		 *
		 * @param pFovV The desired vertical fov in radians
		 */
		void SetFovV(float pFovV);
		
		/**
		 * @brief Sets what colour the clear function resolves to
		 *
		 * @param pValue The desired colour in RGBA
		 */
		void SetClearColour(vec4 pValue);
		/**
		 * @brief Sets what colour the clear function resolves to. Alpha is set to 1
		 *
		 * @param pValue The desired colour in RGB
		 */
		void SetClearColour(vec3 pValue);
		/**
		 * @brief Sets what colour the clear function resolves to
		 *
		 * @param pValue The desired colour in RGB
		 * @param pA The desired alpha value
		 */
		void SetClearColour(vec3 pValue, float pA);
		/**
		 * @brief Sets what colour the clear function resolves to. Alpha is set to 1
		 *
		 * @param pRed The desired red value
		 * @param pGreen The desired blue value
		 * @param pBlue The desired green value
		 */
		void SetClearColour(float pRed, float pGreen, float pBlue);
		/**
		 * @brief Sets what colour the clear function resolves to
		 *
		 * @param pRed The desired red value
		 * @param pGreen The desired blue value
		 * @param pBlue The desired green value
		 * @param pAlpha The desired alpha value
		 */
		void SetClearColour(float pRed, float pGreen, float pBlue, float pAlpha);
		#pragma endregion
		#pragma region Getters
		mat4 GetView() const;
		mat4 GetProjection() const;

		vec3 GetRight() const;
		vec3 GetUp() const;
		vec3 GetForward() const;

		mat4 GetWorldToCameraMatrix();
		#pragma endregion

	private:
		float m_fovH = 0, m_fovV = 0;	// The field of view of the camera in degrees, horizontal & vertical
		float m_aspectRatio = 0;
		mat4 m_view;
		mat4 m_projection;
	};
}
