#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	Camera();
	Camera(float pFovh);
	Camera(mat4 pTransform);
	Camera(vec3 pFrom, vec3 pTo, vec3 pUp);
	~Camera() {}

	void LookAt(vec3 pFrom, vec3 pTo, vec3 pUp);
	void LookAt(vec3 pFrom, vec3 pTo);

	void SetClearColour(vec4 pValue);
	void SetClearColour(vec3 pValue);
	void SetClearColour(vec3 pValue, float pA);
	void SetClearColour(float pRed, float pGreen, float pBlue);
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
	float m_fovh;
	vec3 m_localUp;
	union
	{
		mat4 m_localTransform;
		mat4 m_view;
	};
	mat4 m_projection;
};
