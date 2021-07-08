#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	Camera();
	Camera(mat4 pTransform);
	Camera(vec3 pFrom, vec3 pTo, vec3 pUp = { 0, 1, 0 });
	~Camera();

private:
	mat4 m_transform;
};
