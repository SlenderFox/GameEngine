#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Engine
{
	class Camera
	{
	public:
		// Default assumes the aspect ratio is 16:9
		Camera();
		Camera(float pAspect, float pInvAspect);
		Camera(float pAspect, float pInvAspect, float pFovH);
		Camera(float pAspect, float pInvAspect, mat4 pTransform);
		Camera(float pAspect, float pInvAspect, float pFovH, mat4 pTransform);
		Camera(float pAspect, float pInvAspect, vec3 pFrom, vec3 pTo, vec3 pUp);
		Camera(float pAspect, float pInvAspect, float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp);
		~Camera() {}

		/*@brief Directly sets the horizontal fov and calculates the corresponding vertical fov
		* @param pAspect The aspect ratio of the window
		* @param pInvAspect The inverse aspect ratio of the window
		* @param pFovH The desired horizontal fov in radians
		*/
		void SetFovH(float pAspect, float pInvAspect, float pFovH);
		/*@brief Directly sets the vertical fov and calculates the corresponding horizontal fov
		* @param pAspect The aspect ratio of the window
		* @param pInvAspect The inverse aspect ratio of the window
		* @param pFovH The desired vertical fov in radians
		*/
		void SetFovV(float pAspect, float pInvAspect, float pFovV);
		/*@brief Updates the horizontal fov from the vertical given a new aspect ratio
		* then creates a projection
		* @param pAspect The aspect ratio of the window
		* @param pInvAspect The inverse aspect ratio of the window
		*/
		void UpdateFovH(float pAspect, float pInvAspect);
		/*@brief Updates the vertical fov from the horizontal given a new inverse aspect ratio
		* then creates a projection
		* @param pAspect The aspect ratio of the window
		* @param pInvAspect The inverse aspect ratio of the window
		*/
		void UpdateFovV(float pAspect, float pInvAspect);

		void LookAt(vec3 pFrom, vec3 pTo, vec3 pUp);
		void LookAt(vec3 pFrom, vec3 pTo);

		/*@brief Sets what colour the clear function resolves to
		* @param pValue The desired colour in RGBA
		*/
		void SetClearColour(vec4 pValue);
		/*@brief Sets what colour the clear function resolves to
		* @param pValue The desired colour in RGB, alpha is set to 1
		*/
		void SetClearColour(vec3 pValue);
		/*@brief Sets what colour the clear function resolves to
		* @param pValue The desired colour in RGB
		* @param pA The desired alpha value
		*/
		void SetClearColour(vec3 pValue, float pA);
		/*@brief Sets what colour the clear function resolves to
		* @param pValue The desired red value
		* @param pValue The desired blue value
		* @param pValue The desired green value
		*/
		void SetClearColour(float pRed, float pGreen, float pBlue);
		/*@brief Sets what colour the clear function resolves to
		* @param pValue The desired red value
		* @param pValue The desired blue value
		* @param pValue The desired green value
		* @param pValue The desired alpha value
		*/
		void SetClearColour(float pRed, float pGreen, float pBlue, float pAlpha);

		mat4 WorldToCameraMatrix();

		mat4 GetTransform() const;
		void SetTransform(mat4 pValue);
		void SetPosition(vec3 pValue);
		mat4 GetView() const;
		void SetView(mat4 pValue);
		mat4 GetProjection() const;
		void SetProjection(mat4 pValue);

	private:
		float m_fovH, m_fovV;	// The field of view of the camera in radians, horizontal & vertical
		vec3 m_localUp;
		union
		{
			mat4 m_localTransform;
			mat4 m_view;
		};
		mat4 m_projection;
	};
}
