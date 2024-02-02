#include "project.hpp"
#include "glm/gtc/matrix_transform.hpp"

using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::transpose;
using glm::inverse;
using glm::rotate;
using glm::radians;
using namespace srender;

/** Entry point of the project
 * @note Following: https://learnopengl.com/, at https://learnopengl.com/Advanced-OpenGL/Blending
 * @note Disable console through:
 * [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
 * @note Resolutions: 800x600, 1280x720, 1600x900, 1920x1080
 */
int main(int _argc, char *_args[])
{
	application::setFullscreen(false);
	#ifdef _DEBUG
		application::setDimensions(1030, 650);
		application::setTitle("SRender Example (DEBUG)");
	#else
		application::setDimensions(1600, 900);
		application::setTitle("SRender Example (RELEASE)");
	#endif
	project *app = new project();
	return app->run(_argc, _args);
}

// Static

double project::s_camYaw = 90.0;
double project::s_camPitch = 0.0;
const vec3 project::s_cubePositions[project::s_numCubes] = {
	glm::vec3( 2.0f,  8.5f, -15.0f),
	glm::vec3(-1.5f,  1.3f,  -2.5f),
	glm::vec3(-3.8f,  1.5f, -12.3f),
	glm::vec3( 2.4f,  3.1f,  -3.5f),
	glm::vec3(-1.7f,  6.5f,  -7.5f),
	glm::vec3( 1.3f,  1.5f,  -2.5f),
	glm::vec3( 1.5f,  5.5f,  -2.5f),
	glm::vec3( 1.5f,  3.7f,  -1.5f),
	glm::vec3(-1.3f,  4.5f,  -1.5f)
};

void project::mouseCallback(double _deltaX, double _deltaY) noexcept
{
	const double sens = 0.05f;
	_deltaX *= sens;
	_deltaY *= sens;
	s_camYaw += _deltaX;
	s_camPitch += _deltaY;
	if (s_camPitch > 89.0f)
	{	s_camPitch = 89.0f; }
	else if (s_camPitch < -89.0f)
	{	s_camPitch = -89.0f; }
	// The forward direction of the camera
	vec3 forward = vec3();
	forward.x = (float)(cos(radians(s_camYaw)) * cos(radians(s_camPitch)));
	forward.y = (float)sin(radians(s_camPitch));
	forward.z = (float)(sin(radians(s_camYaw)) * cos(radians(s_camPitch)));
	forward = normalize(forward);
	graphics::getCamera()->setForward(forward);
}

void project::scrollCallback(double _offsetX, double _offsetY) noexcept
{	graphics::getCamera()->modifyFovH((float)_offsetY * -3.0f); }

// Member

project::project()
{}

project::~project()
{
	for (uint8_t i = 0; i < m_cubes.size(); ++i)
	{
		if (m_cubes[i])
		{	delete m_cubes[i]; }
	}

	if (m_backpack)
	{	delete m_backpack; }
}

bool project::startup()
{
	createLights();
	createScene();
	//graphics::setRenderDepthBuffer(true);
	input::addMouseCallback(mouseCallback);
	input::addSrollCallback(scrollCallback);
	return true;
}

void project::shutdown() {}

void project::update()
{
	processInput();

	// Rotates the cubes
	for (uint8_t i = 0; i < s_numCubes; i++)
	{
		float angle = (float)getDeltaTime() * 5.0f * ((i + 1) / (i * 0.2f + 1));
		mat4 rot = rotate(
			m_cubes[i]->getTransform().getTransform(),
			radians(angle),
			glm::normalize(vec3(1.0f, 0.3f, 0.5f))
		);
		m_cubes[i]->setTransform(&rot);
	}
}

void project::fixedUpdate() {}

void project::lateUpdate() {}

void project::createScene()
{
	// Create the ground
	m_ground = new entity();
	model *groundModel = new model();
	groundModel->addShader(new shader());
	vector<mesh::vertex> verts = mesh::generateVertices();
	vector<uint32_t> inds = mesh::generateIndices();
	mesh *square = new mesh(&verts, &inds);
	groundModel->addMesh(square);
	groundModel->sentTint(colour(0.25f, 0.4f, 0.18f));
	m_ground->addComponent(groundModel);
	m_ground->setScale(vec3(50, 1, 50));

	// Create a backpack in the centre
	m_backpack = new entity();
	m_backpack->addComponent(new model(
		"assets/models/backpack/backpack.obj",
		"assets/shaders/default"
	));
	m_backpack->translate(vec3(0.0f, 3.5f, 0.9f));
	m_backpack->setScale(vec3(0.6f));

	// Place 9 cubes behind
	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		entity *cube = new entity();
		cube->addComponent(new model(
			"assets/models/cube/cube.obj",
			"assets/shaders/default"
		));
		cube->translate(s_cubePositions[i]);
		cube->setScale(vec3(0.6f));
		m_cubes.push_back(cube);
	}
}

void project::createLights()
{
	// Allows me to toggle at will
	const bool directional = true,
		point = true,
		spot = true;

	// Creates lights
	entity *entityRef = nullptr;
	light *lightRef = nullptr;

	if (directional)
	{
		entityRef = new entity();
		entityRef->addComponent(new light(
			light::type::directional,
			/* White light */
			colour(colour::hsvToRgb(vec3(0, 0.0f, 0.6f)))
		));
		lightRef = entityRef->getComponentLight();
		entityRef->setForward(vec3(0.0f, -1.0f, 0.0f));
		graphics::setClearColour(
			lightRef->getColour() * graphics::getAmbience()
		);
		m_lights.push_back(entityRef);
	}

	if (point)
	{
		entityRef = new entity();
		entityRef->addComponent(new light(
			light::type::point,
			/* Red light */
			colour(colour::hsvToRgb(vec3(0, 0.6f, 0.8f)))
		));
		lightRef = entityRef->getComponentLight();
		entityRef->addComponent(new model(
			"assets/models/cube/cube.obj",
			"assets/shaders/default",
			false
		));
		entityRef->setPosition(vec3(-4.0f, 5.5f, -2.0f));
		entityRef->setScale(vec3(0.1f, 0.1f, 0.1f));
		entityRef->getComponentModel()->sentTint(lightRef->getColour());
		entityRef->getComponentModel()->fullbright(true);
		m_lights.push_back(entityRef);
	}

	if (spot)
	{
		entityRef = new entity();
		entityRef->addComponent(new light(
			light::type::spot,
			/* Green light */
			colour(colour::hsvToRgb(vec3(110, 0.3f, 1.0f)))
		));
		lightRef = entityRef->getComponentLight();
		entityRef->addComponent(new model(
			"assets/models/cube/cube.obj",
			"assets/shaders/default",
			false
		));
		entityRef->getComponentModel()->sentTint(lightRef->getColour());
		entityRef->getComponentModel()->fullbright(true);
		entityRef->setPosition(vec3(2.0f, 6.0f, 6.0f));
		entityRef->setForward(vec3(-0.3f, -0.4f, -1));
		// FIXME needs to be called after setForward
		entityRef->setScale(vec3(0.1f, 0.1f, 0.2f));
		lightRef->setAngle(13.0f);
		lightRef->setBlur(0.23f);
		m_lights.push_back(entityRef);
	}
}

void project::processInput()
{
	// Render triangles normally
	if (input::checkKeyState(input::key::key_f1, input::state::press))
	{	graphics::setRenderMode(graphics::mode::fill); }
	// Render triangles as lines
	if (input::checkKeyState(input::key::key_f2, input::state::press))
	{	graphics::setRenderMode(graphics::mode::line); }
	// Render triangles as dots
	if (input::checkKeyState(input::key::key_f3, input::state::press))
	{	graphics::setRenderMode(graphics::mode::point); }

	// Spotlight cone
	const float coneSpeed = valueModKeys(6.0f) * (float)getDeltaTime();
	if (input::checkKeyState(input::key::key_t, input::state::press))
	{	graphics::modifyAllSpotlights(true, coneSpeed); }
	if (input::checkKeyState(input::key::key_g, input::state::press))
	{	graphics::modifyAllSpotlights(true, -coneSpeed); }
	// Spotlight blur
	const float blurSpeed = valueModKeys(0.5f) * (float)getDeltaTime();
	if (input::checkKeyState(input::key::key_y, input::state::press))
	{	graphics::modifyAllSpotlights(false, -blurSpeed); }
	if (input::checkKeyState(input::key::key_h, input::state::press))
	{	graphics::modifyAllSpotlights(false, blurSpeed); }

	vec3 translation = vec3();
	const float moveSpeed = valueModKeys(4.0f) * (float)getDeltaTime();

	// Forwards
	if (input::checkKeyState(input::key::key_w, input::state::press))
	{	translation += (vec3)graphics::getCamera()->getForward() * moveSpeed; }
	// Backwards
	if (input::checkKeyState(input::key::key_s, input::state::press))
	{	translation -= (vec3)graphics::getCamera()->getForward() * moveSpeed; }
	// Left
	if (input::checkKeyState(input::key::key_a, input::state::press))
	{	translation += (vec3)graphics::getCamera()->getRight() * moveSpeed; }
	// Right
	if (input::checkKeyState(input::key::key_d, input::state::press))
	{	translation -= (vec3)graphics::getCamera()->getRight() * moveSpeed; }
	// Up
	if (input::checkKeyState(input::key::key_space, input::state::press))
	{	translation += (vec3)graphics::getCamera()->getUp() * moveSpeed; }
	// Down
	if (input::checkKeyState(input::key::key_c, input::state::press))
	{	translation -= (vec3)graphics::getCamera()->getUp() * moveSpeed; }

	graphics::getCamera()->translate(translation);
}

float project::valueModKeys(float _value) noexcept
{
	// Reduce
	if (input::checkKeyState(input::key::key_left_control, input::state::press))
	{	_value *= 0.1f; }
	// Increase
	else if (input::checkKeyState(input::key::key_left_shift, input::state::press))
	{	_value *= 3; }
	return _value;
}
