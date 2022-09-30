#pragma region
#include "Project.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

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
	m_rendererRef = Engine::Renderer::GetInstance();
	CreateLights();
	CreateScene();
	return true;
}

void Project::Shutdown() {}

void Project::Update()
{
	// Rotates the cubes
	for (uint8_t i = 0; i < s_numCubes; i++)
	{
		float angle = (float)GetDeltaTime() * 5.0f * ((i + 1) / (i * 0.2f + 1));
		m_cubes[i]->SetTransform(rotate(m_cubes[i]->GetTransform(), radians(angle), glm::normalize(vec3(1.0f, 0.3f, 0.5f))));
	}
}

void Project::FixedUpdate() {}

void Project::LateUpdate() {}

void Project::CreateScene()
{
	Engine::Root* root = Engine::Root::GetRoot();
	Engine::Model* model = nullptr;
	Engine::Shader* shader = nullptr;
	// Place 9 cubes behind
	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		Engine::Entity* cube = Engine::Entity::CreateWithModel("assets/models/cube/cube.obj", "assets/shaders/default", model, shader);
		cube->SetParent(root);
		cube->Translate(m_cubePositions[i]);
		cube->SetScale(vec3(0.6f));
		m_cubes.push_back(cube);
	}

	// Create a backpack in the centre
	Engine::Entity* backpack = Engine::Entity::CreateWithModel("assets/models/backpack/backpack.obj", "assets/shaders/default", model, shader);
	backpack->SetParent(root);
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
	light = m_rendererRef->AddNewLight(ID, Engine::LightType::Directional, Engine::Colour::Silver());
	light->SetDirection(vec3(0, -1, 0));
	Engine::Renderer::SetClearColour(m_rendererRef->GetLightAt(ID)->GetColour() * Engine::Renderer::s_ambience);
	// Point
	light = m_rendererRef->AddNewLight(ID, Engine::LightType::Point, Engine::Colour::CreateWithHSV(Engine::hsv(220, 0.6f, 1.0f)));
	light->SetPosition(vec4(-4, 2, -2, 1));
	// Spot
	light = m_rendererRef->AddNewLight(ID, Engine::LightType::Spot, Engine::Colour::CreateWithHSV(Engine::hsv(97, 0.17f, 1.0f)));
	light->SetPosition(vec4(2.0f, 2.5f, 6.0f, 1));
	light->SetDirection(vec3(-0.3f, -0.4f, -1));
	light->SetAngle(13.0f);
	light->SetBlur(0.23f);

	Engine::Root* root = Engine::Root::GetRoot();
	Engine::Model* model = nullptr;
	Engine::Shader* shader = nullptr;

	// Gives them physical form
	for (uint8_t i = 0; i < m_rendererRef->LightCount(); ++i)
	{
		Engine::Light* current = m_rendererRef->GetLightAt(i);

		if (current->GetType() == Engine::LightType::Point
		 || current->GetType() == Engine::LightType::Spot)
		{
			current->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default", model, shader, false);
			current->SetParent(root);
			current->SetTransform(current->GetTransform());
			current->SetScale(vec3(0.2f, 0.2f, (current->GetType() == Engine::LightType::Spot) ? 0.4f : 0.2f));
			current->SetColourInShader(current->GetColour());
			current->RenderOnlyColour(true);
			m_lightRefs.push_back(current);
		}
	}
}
