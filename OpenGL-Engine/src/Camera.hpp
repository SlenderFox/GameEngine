#pragma region
#pragma once
#include "Transform.hpp"
#pragma endregion

namespace Engine
{
	class Camera : public Transform
	{
	private:
		float m_fovH = 0, m_fovV = 0;	// The field of view of the camera in degrees, horizontal & vertical
		float m_aspectRatio = 0;
		mat4 m_view;
		mat4 m_projection;

	public:
		#pragma region Constructors
		Camera(float pAspectRatio);
		Camera(float pAspectRatio, float pFovH);
		Camera(float pAspectRatio, mat4 pTransform);
		Camera(float pAspectRatio, float pFovH, mat4 pTransform);
		Camera(float pAspectRatio, vec3 pFrom, vec3 pTo, vec3 pUp);
		Camera(float pAspectRatio, float pFovH, vec3 pFrom, vec3 pTo, vec3 pUp);
		#pragma endregion

		void LookAt(vec3 pFrom, vec3 pTo, vec3 pUp);

		void ModifyFovH(float pValue);
		void ModifyFovV(float pValue);
		void UpdateFovH();
		void UpdateFovV();

		#pragma region Setters
		void SetTransform(mat4 pValue) override;
		void SetView(mat4 pValue);
		void SetProjection(mat4 pValue);
		void SetProjection(float pFovV);

		void SetPosition(vec3 pValue) override;
		void Translate(vec3 pValue);

		void SetAxes(vec3 pRight, vec3 pUp, vec3 pForward);
		void SetRight(vec3 pRight);
		void SetUp(vec3 pUp);
		void SetForward(vec3 pForward);

		void SetAspectRatio(float pAspectRatio);
		void SetFovH(float pFovH);
		void SetFovV(float pFovV);
		
		void SetClearColour(vec4 pValue);
		void SetClearColour(vec3 pValue);
		void SetClearColour(vec3 pValue, float pAlpha);
		void SetClearColour(float pRed, float pGreen, float pBlue);
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
	};
}
