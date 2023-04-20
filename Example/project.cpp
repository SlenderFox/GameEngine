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
 * @note Following: https://learnopengl.com/, at https://learnopengl.com/Advanced-OpenGL/Depth-testing
 * @note Disable console through:
 * [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
 * @note Resolutions: 800x600, 1280x720, 1600x900, 1920x1080
 */
int main(int argc, char *args[])
{
	project *app = new project();
	application::setFullscreen(false);
	#ifdef _DEBUG
		application::setDimensions(1030, 650);
		application::setTitle("SRender Example (DEBUG)");
	#else
		application::setDimensions(1600, 900);
		application::setTitle("SRender Example (RELEASE)");
	#endif
	int code = (int)application::run(args[0]);
	delete app;
	return code;
}

// Static

double project::s_camYaw = 90.0;
double project::s_camPitch = 0.0;
const vec3 project::s_cubePositions[project::s_numCubes] = {
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f,  -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f,  -3.5f),
	glm::vec3(-1.7f,  3.0f,  -7.5f),
	glm::vec3( 1.3f, -2.0f,  -2.5f),
	glm::vec3( 1.5f,  2.0f,  -2.5f),
	glm::vec3( 1.5f,  0.2f,  -1.5f),
	glm::vec3(-1.3f,  1.0f,  -1.5f)
};

void project::mouseCallback(double _deltaX, double _deltaY) noexcept
{
	const double sens = 0.05f;
	_deltaX *= sens;
	_deltaY *= sens;
	s_camYaw += _deltaX;
	s_camPitch += _deltaY;
	if (s_camPitch > 89.0f)
		s_camPitch = 89.0f;
	else if (s_camPitch < -89.0f)
		s_camPitch = -89.0f;
	// The forward direction of the camera
	vec3 forward = vec3();
	forward.x = (float)(cos(radians(s_camYaw)) * cos(radians(s_camPitch)));
	forward.y = (float)sin(radians(s_camPitch));
	forward.z = (float)(sin(radians(s_camYaw)) * cos(radians(s_camPitch)));
	forward = normalize(forward);
	graphics::getCamera()->setForward(forward);
}

void project::scrollCallback(double _offsetX, double _offsetY) noexcept
{
	graphics::getCamera()->modifyFovH((float)_offsetY * -3.0f);
}

// Member

project::project()
{
	m_lightRefs = vector<light*>();
	m_cubes = vector<entity*>();

	object_backpack = new entity();
}

project::~project()
{
	for (uint8_t i = 0; i < m_cubes.size(); ++i)
	{
		if (m_cubes[i])
			delete m_cubes[i];
	}

	if (object_backpack)
		delete object_backpack;
}

bool project::startup()
{
	createLights();
	createScene();
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
			m_cubes[i]->getTransform(),
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
	model *model = nullptr;
	shader *shader = nullptr;

	// Place 9 cubes behind
	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		entity *cube = new entity(
			"assets/models/cube/cube.obj",
			"assets/shaders/default",
			model, shader
		);
		cube->translate(s_cubePositions[i]);
		cube->setScale(vec3(0.6f));
		m_cubes.push_back(cube);
	}

	// Create a backpack in the centre
	entity *backpack = new entity(
		"assets/models/backpack/backpack.obj",
		"assets/shaders/default",
		model, shader
	);
	backpack->translate(vec3(0.0f, 0.0f, 0.9f));
	backpack->setScale(vec3(0.6f));
	object_backpack = backpack;
}

void project::createLights()
{
	// Allows me to toggle at will
	const bool directional = true,
		point = true,
		spot = true;

	// Creates lights
	uint8_t ID;
	light *light;
	if (directional)
	{
		light = graphics::addNewLight(
			ID,
			light::type::Directional,
			colour(colour::hsvToRgb({0, 0.0f, 0.6f}))
		);
		light->setDirection(vec3(0, -1, 0));
		graphics::setClearColour(
			graphics::getLightAt(ID)->getColour() * graphics::getAmbience()
		);
	}
	if (point)
	{
		light = graphics::addNewLight(
			ID,
			light::type::Point,
			colour(colour::hsvToRgb({220, 0.6f, 1.0f}))
		);
		light->setPosition(vec4(-4, 2, -2, 1));
	}
	if (spot)
	{
		light = graphics::addNewLight(
			ID,
			light::type::Spot,
			colour(colour::hsvToRgb({97, 0.17f, 1.0f}))
		);
		light->setPosition(vec4(2.0f, 2.5f, 6.0f, 1));
		light->setDirection(vec3(-0.3f, -0.4f, -1));
		light->setAngle(13.0f);
		light->setBlur(0.23f);
	}

	// Don't bother if there are no lights
	if (graphics::lightCount() == 0)
	{	return; }

	model *model = nullptr;
	shader *shader = nullptr;

	// Gives them physical form
	for (uint8_t i = 0; i < graphics::lightCount(); ++i)
	{
		light = graphics::getLightAt(i);

		if (light->getType() == light::type::Point
		 || light->getType() == light::type::Spot)
		{
			light->loadModel(
				"assets/models/cube/cube.obj",
				"assets/shaders/default",
				model, shader, false
			);
			light->setScale(vec3(0.2f, 0.2f, (light->getType() == light::type::Spot) ? 0.4f : 0.2f));
			light->sentTint(light->getColour());
			light->renderOnlyColour(true);
			m_lightRefs.push_back(light);
		}
	}
}

void project::processInput() noexcept
{
	// Render triangles normally
	if (input::checkKeyState(input::key::Key_F1, input::state::Press))
		graphics::setRenderMode(graphics::mode::fill);
	// Render triangles as lines
	if (input::checkKeyState(input::key::Key_F2, input::state::Press))
		graphics::setRenderMode(graphics::mode::line);
	// Render triangles as dots
	if (input::checkKeyState(input::key::Key_F3, input::state::Press))
		graphics::setRenderMode(graphics::mode::point);

	// Spotlight cone
	if (input::checkKeyState(input::key::Key_T, input::state::Press))
		graphics::modifyAllSpotlights(true, 0.05f);
	if (input::checkKeyState(input::key::Key_G, input::state::Press))
		graphics::modifyAllSpotlights(true, -0.05f);
	// Spotlight blur
	if (input::checkKeyState(input::key::Key_Y, input::state::Press))
		graphics::modifyAllSpotlights(false, -0.005f);
	if (input::checkKeyState(input::key::Key_H, input::state::Press))
		graphics::modifyAllSpotlights(false, 0.005f);

	vec3 translation = vec3();
	float moveSpeed = 4;

	// SlowDown
	if (input::checkKeyState(input::key::Key_LeftControl, input::state::Press))
		moveSpeed *= 0.1f;
	// SpeedUp
	else if (input::checkKeyState(input::key::Key_LeftShift, input::state::Press))
		moveSpeed *= 3;

	// Forwards
	if (input::checkKeyState(input::key::Key_W, input::state::Press))
		translation += moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getForward();
	// Backwards
	if (input::checkKeyState(input::key::Key_S, input::state::Press))
		translation -= moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getForward();
	// Left
	if (input::checkKeyState(input::key::Key_A, input::state::Press))
		translation += moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getRight();
	// Right
	if (input::checkKeyState(input::key::Key_D, input::state::Press))
		translation -= moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getRight();
	// Up
	if (input::checkKeyState(input::key::Key_Space, input::state::Press))
		translation += moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getUp();
	// Down
	if (input::checkKeyState(input::key::Key_C, input::state::Press))
		translation -= moveSpeed * (float)getDeltaTime() * (vec3)graphics::getCamera()->getUp();

	graphics::getCamera()->translate(translation);
}
