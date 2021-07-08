#include "Camera.h"

Camera::Camera()
{
	m_transform = mat4(1.0f);
}

Camera::Camera(mat4 pTransform)
{
	m_transform = pTransform;
}

Camera::Camera(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 })
{
	m_transform = glm::lookAt(pFrom, pTo, pUp);
}

Camera::~Camera()
{
}
