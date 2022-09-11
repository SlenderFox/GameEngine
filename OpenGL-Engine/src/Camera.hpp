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

		void ModifyFovH(float pValue);
		void ModifyFovV(float pValue);
		void UpdateFovH();
		void UpdateFovV();

		#pragma region Setters
		void SetTransform(glm::mat4 pValue) override;
		void SetView(glm::mat4 pValue);
		void SetProjection(glm::mat4 pValue);
		void SetProjection(float pFovV);

		void SetPosition(glm::vec3 pValue) override;
		void Translate(glm::vec3 pValue);

		void SetAxes(glm::vec3 pRight, glm::vec3 pUp, glm::vec3 pForward);
		void SetRight(glm::vec3 pRight);
		void SetUp(glm::vec3 pUp);
		void SetForward(glm::vec3 pForward);

		void SetAspectRatio(float pAspectRatio);
		void SetFovH(float pFovH);
		void SetFovV(float pFovV);
		
		void SetClearColour(glm::vec4 pValue);
		void SetClearColour(glm::vec3 pValue);
		void SetClearColour(glm::vec3 pValue, float pAlpha);
		void SetClearColour(float pRed, float pGreen, float pBlue);
		void SetClearColour(float pRed, float pGreen, float pBlue, float pAlpha);
		#pragma endregion
		#pragma region Getters
		glm::vec4 GetPosition() const { return Transform::GetPosition(); }
		glm::vec4 GetRight() const { return Transform::GetRight(); }
		glm::vec4 GetUp() const { return Transform::GetUp(); }
		glm::vec4 GetForward() const { return Transform::GetForward(); }

		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;
		glm::mat4 GetWorldToCameraMatrix();
		#pragma endregion
	};
}
