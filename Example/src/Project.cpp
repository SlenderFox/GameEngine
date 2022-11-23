#pragma region
#pragma warning(disable:4100)
#include "Project.hpp"
#include "glm/gtc/matrix_transform.hpp"

using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::translate;
using glm::transpose;
using glm::inverse;
using glm::rotate;
using glm::radians;
using namespace Engine;
#pragma endregion

/** Entry point of the project
 * @note Following: https://learnopengl.com/
 * @note Disable console through:
 * [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
 * @note Resolutions:
 * 800, 600
 * 1280, 720
 * 1600, 900
 * 1920, 1080
 * @note Refer to Notes.md to see TODO and notes
 */
int main()
{
	Project* app = new Project();
	#ifdef _DEBUG
		app->SetDimensions(1030, 650);
		app->SetTitle("Application (DEBUG)");
	#else
		app->SetDimensions(1600, 900);
		app->SetTitle("Application (RELEASE)");
	#endif
	app->SetFullscreen(false);
	int code = (int)app->Run();
	delete app;
	return code;
}

// Static

double Project::s_camYaw = 90.0;
double Project::s_camPitch = 0.0;

void Project::MouseCallback(double pDeltaX, double pDeltaY) noexcept
{
	const double sens = 0.05f;
	pDeltaX *= sens;
	pDeltaY *= sens;
	s_camYaw += pDeltaX;
	s_camPitch += pDeltaY;
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
	Renderer::GetCamera()->SetForward(forward);
}

void Project::ScrollCallback(double pOffsetX, double pOffsetY) noexcept
{
	Renderer::GetCamera()->ModifyFovH((float)pOffsetY * -3.0f);
}

// Member

Project::Project()
{
	m_lightRefs = vector<Light*>();
	m_cubes = vector<Entity*>();

	object_backpack = new Entity();
}

Project::~Project()
{
	for (uint8_t i = 0; i < m_cubes.size(); ++i)
	{
		if (m_cubes[i])
			delete m_cubes[i];
	}

	if (object_backpack)
		delete object_backpack;
}

bool Project::Startup()
{
	CreateLights();
	CreateScene();
	Input::AddMouseCallback(MouseCallback);
	Input::AddSrollCallback(ScrollCallback);
	return true;
}

void Project::Shutdown() {}

void Project::Update()
{
	ProcessInput();

	// Rotates the cubes
	for (uint8_t i = 0; i < s_numCubes; i++)
	{
		float angle = (float)GetDeltaTime() * 5.0f * ((i + 1) / (i * 0.2f + 1));
		mat4 rot = rotate(
			m_cubes[i]->GetTransform(),
			radians(angle),
			glm::normalize(vec3(1.0f, 0.3f, 0.5f))
		);
		m_cubes[i]->SetTransform(&rot);
	}
}

void Project::FixedUpdate() {}

void Project::LateUpdate() {}

void Project::CreateScene()
{
	Model* model = nullptr;
	Shader* shader = nullptr;

	// Place 9 cubes behind
	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		Entity* cube = Entity::CreateWithModel(
			"assets/models/cube/cube.obj", "assets/shaders/default", model, shader
		);
		cube->Translate(m_cubePositions[i]);
		cube->SetScale(vec3(0.6f));
		m_cubes.push_back(cube);
	}

	// Create a backpack in the centre
	Entity* backpack = Entity::CreateWithModel(
		"assets/models/backpack/backpack.obj", "assets/shaders/default", model, shader
	);
	backpack->Translate(vec3(0.0f, 0.0f, 0.9f));
	backpack->SetScale(vec3(0.6f));
	object_backpack = backpack;
}

void Project::CreateLights()
{
	// Allows me to toggle at will
	const bool directional = true,
		point = true,
		spot = true;

	// Creates lights
	uint8_t ID;
	Light* light;
	if (directional)
	{
		light = Renderer::AddNewLight(
			ID,
			LightType::Directional,
			Colour(Colour::HSVtoRGB({0, 0.0f, 0.6f}))
		);
		light->SetDirection(vec3(0, -1, 0));
		Renderer::SetClearColour(
			Renderer::GetLightAt(ID)->GetColour() * Renderer::s_ambience
		);
	}
	if (point)
	{
		light = Renderer::AddNewLight(
			ID,
			LightType::Point,
			Colour(Colour::HSVtoRGB({220, 0.6f, 1.0f}))
		);
		light->SetPosition(vec4(-4, 2, -2, 1));
	}
	if (spot)
	{
		light = Renderer::AddNewLight(
			ID,
			LightType::Spot,
			Colour(Colour::HSVtoRGB({97, 0.17f, 1.0f}))
		);
		light->SetPosition(vec4(2.0f, 2.5f, 6.0f, 1));
		light->SetDirection(vec3(-0.3f, -0.4f, -1));
		light->SetAngle(13.0f);
		light->SetBlur(0.23f);
	}

	// Don't bother if there are no lights
	if (Renderer::LightCount() == 0) return;

	Model* model = nullptr;
	Shader* shader = nullptr;

	// Gives them physical form
	for (uint8_t i = 0; i < Renderer::LightCount(); ++i)
	{
		light = Renderer::GetLightAt(i);

		if (light->GetType() == LightType::Point
		 || light->GetType() == LightType::Spot)
		{
			light->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default", model, shader, false);
			light->SetScale(vec3(0.2f, 0.2f, (light->GetType() == LightType::Spot) ? 0.4f : 0.2f));
			light->SentTint(light->GetColour());
			light->RenderOnlyColour(true);
			m_lightRefs.push_back(light);
		}
	}
}

void Project::ProcessInput() noexcept
{
	// Render triangles normally
	if (Input::CheckKeyState(Input::Key::Key_F1, Input::State::Press))
		Renderer::SetRenderMode(Renderer::Mode::Fill);
	// Render triangles as lines
	if (Input::CheckKeyState(Input::Key::Key_F2, Input::State::Press))
		Renderer::SetRenderMode(Renderer::Mode::Line);
	// Render triangles as dots
	if (Input::CheckKeyState(Input::Key::Key_F3, Input::State::Press))
		Renderer::SetRenderMode(Renderer::Mode::Point);

	// Spotlight cone
	if (Input::CheckKeyState(Input::Key::Key_T, Input::State::Press))
		Renderer::ModifyAllSpotlights(true, 0.05f);
	if (Input::CheckKeyState(Input::Key::Key_G, Input::State::Press))
		Renderer::ModifyAllSpotlights(true, -0.05f);
	// Spotlight blur
	if (Input::CheckKeyState(Input::Key::Key_Y, Input::State::Press))
		Renderer::ModifyAllSpotlights(false, -0.005f);
	if (Input::CheckKeyState(Input::Key::Key_H, Input::State::Press))
		Renderer::ModifyAllSpotlights(false, 0.005f);

	vec3 translation = vec3();
	float moveSpeed = 4;

	// SlowDown
	if (Input::CheckKeyState(Input::Key::Key_LeftControl, Input::State::Press))
		moveSpeed *= 0.1f;
	// SpeedUp
	else if (Input::CheckKeyState(Input::Key::Key_LeftShift, Input::State::Press))
		moveSpeed *= 3;

	// Forwards
	if (Input::CheckKeyState(Input::Key::Key_W, Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetForward();
	// Backwards
	if (Input::CheckKeyState(Input::Key::Key_S, Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetForward();
	// Left
	if (Input::CheckKeyState(Input::Key::Key_A, Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetRight();
	// Right
	if (Input::CheckKeyState(Input::Key::Key_D, Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetRight();
	// Up
	if (Input::CheckKeyState(Input::Key::Key_Space, Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetUp();
	// Down
	if (Input::CheckKeyState(Input::Key::Key_C, Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Renderer::GetCamera()->GetUp();

	Renderer::GetCamera()->Translate(translation);
}
