#pragma region
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
#pragma endregion

Project::Project()
{
	m_lightRefs = vector<Engine::Light*>();
	m_cubes = vector<Engine::Entity*>();

	object_backpack = new Engine::Entity();
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
		m_cubes[i]->SetTransform(rotate(
			m_cubes[i]->GetTransform(), 
			radians(angle), 
			glm::normalize(vec3(1.0f, 0.3f, 0.5f))
			)
		);
	}
}

void Project::FixedUpdate() {}

void Project::LateUpdate() {}

void Project::CreateScene()
{
	Engine::Model* model = nullptr;
	Engine::Shader* shader = nullptr;
	
	// Place 9 cubes behind
	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		Engine::Entity* cube = Engine::Entity::CreateWithModel(
			"assets/models/cube/cube.obj", "assets/shaders/default", model, shader
		);
		cube->Translate(m_cubePositions[i]);
		cube->SetScale(vec3(0.6f));
		m_cubes.push_back(cube);
	}

	// Create a backpack in the centre
	Engine::Entity* backpack = Engine::Entity::CreateWithModel(
		"assets/models/backpack/backpack.obj", "assets/shaders/default", model, shader
	);
	backpack->Translate(vec3(0.0f, 0.0f, 0.9f));
	backpack->SetScale(vec3(0.6f));
	object_backpack = backpack;
}

void Project::CreateLights()
{
	// Creates lights
	uint8_t ID;
	Engine::Light* light;
	// Directional
	light = Engine::Renderer::AddNewLight(
		ID, Engine::LightType::Directional, Engine::Colour::Silver()
	);
	light->SetDirection(vec3(0, -1, 0));
	Engine::Renderer::SetClearColour(
		Engine::Renderer::GetLightAt(ID)->GetColour() * Engine::Renderer::s_ambience
	);
	// Point
	light = Engine::Renderer::AddNewLight(
		ID, Engine::LightType::Point, Engine::Colour::CreateWithHSV(
			Engine::hsv(220, 0.6f, 1.0f)
		)
	);
	light->SetPosition(vec4(-4, 2, -2, 1));
	// Spot
	light = Engine::Renderer::AddNewLight(
		ID, Engine::LightType::Spot, Engine::Colour::CreateWithHSV(
			Engine::hsv(97, 0.17f, 1.0f)
		)
	);
	light->SetPosition(vec4(2.0f, 2.5f, 6.0f, 1));
	light->SetDirection(vec3(-0.3f, -0.4f, -1));
	light->SetAngle(13.0f);
	light->SetBlur(0.23f);

	Engine::Model* model = nullptr;
	Engine::Shader* shader = nullptr;

	// Gives them physical form
	for (uint8_t i = 0; i < Engine::Renderer::LightCount(); ++i)
	{
		Engine::Light* current = Engine::Renderer::GetLightAt(i);

		if (current->GetType() == Engine::LightType::Point
		 || current->GetType() == Engine::LightType::Spot)
		{
			current->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default", model, shader, false);
			current->SetTransform(current->GetTransform());	// Required to update shader
			current->SetScale(vec3(0.2f, 0.2f, (current->GetType() == Engine::LightType::Spot) ? 0.4f : 0.2f));
			current->SetColourInShader(current->GetColour());	// Required to update shader
			current->RenderOnlyColour(true);
			m_lightRefs.push_back(current);
		}
	}
}

void Project::ProcessInput() noexcept
{
	// Render triangles normally
	if (Engine::Input::GetKey(Engine::Input::Key::Key_F1, Engine::Input::State::Press))
		Engine::Renderer::SetRenderMode(Engine::Renderer::Mode::Fill);
	// Render triangles as lines
	if (Engine::Input::GetKey(Engine::Input::Key::Key_F2, Engine::Input::State::Press))
		Engine::Renderer::SetRenderMode(Engine::Renderer::Mode::Line);
	// Render triangles as dots
	if (Engine::Input::GetKey(Engine::Input::Key::Key_F3, Engine::Input::State::Press))
		Engine::Renderer::SetRenderMode(Engine::Renderer::Mode::Point);

	// Spotlight cone
	if (Engine::Input::GetKey(Engine::Input::Key::Key_T, Engine::Input::State::Press))
		Engine::Renderer::ModifyAllSpotlightAngles(0.05f);
	if (Engine::Input::GetKey(Engine::Input::Key::Key_G, Engine::Input::State::Press))
		Engine::Renderer::ModifyAllSpotlightAngles(-0.05f);
	// Spotlight blur
	if (Engine::Input::GetKey(Engine::Input::Key::Key_Y, Engine::Input::State::Press))
		Engine::Renderer::ModifyAllSpotlightBlurs(-0.005f);
	if (Engine::Input::GetKey(Engine::Input::Key::Key_H, Engine::Input::State::Press))
		Engine::Renderer::ModifyAllSpotlightBlurs(0.005f);

	vec3 translation = vec3();
	float moveSpeed = 4;

	// SlowDown
	if (Engine::Input::GetKey(Engine::Input::Key::Key_LeftControl, Engine::Input::State::Press))
		moveSpeed *= 0.1f;
	// SpeedUp
	else if (Engine::Input::GetKey(Engine::Input::Key::Key_LeftShift, Engine::Input::State::Press))
		moveSpeed *= 3;

	// Forwards
	if (Engine::Input::GetKey(Engine::Input::Key::Key_W, Engine::Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetForward();
	// Backwards
	if (Engine::Input::GetKey(Engine::Input::Key::Key_S, Engine::Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetForward();
	// Left
	if (Engine::Input::GetKey(Engine::Input::Key::Key_A, Engine::Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetRight();
	// Right
	if (Engine::Input::GetKey(Engine::Input::Key::Key_D, Engine::Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetRight();
	// Up
	if (Engine::Input::GetKey(Engine::Input::Key::Key_Space, Engine::Input::State::Press))
		translation += moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetUp();
	// Down
	if (Engine::Input::GetKey(Engine::Input::Key::Key_C, Engine::Input::State::Press))
		translation -= moveSpeed * (float)GetDeltaTime() * (vec3)Engine::Renderer::GetCamera()->GetUp();

	Engine::Renderer::GetCamera()->Translate(translation);
}
