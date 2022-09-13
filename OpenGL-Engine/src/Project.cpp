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
	object_lightPoint = new Engine::Entity();
	object_lightSpot = new Engine::Entity();
	object_backpack = new Engine::Entity();
	//object_cube = new Engine::Entity();

	m_cubes = vector<Engine::Entity*>();
}

Project::~Project() {}

bool Project::Startup()
{
	m_rendererRef = Engine::Renderer::GetInstance();
	CreateScene();
	return true;
}

void Project::Shutdown() {}

void Project::Update()
{
	for (unsigned int i = 0; i < s_numCubes; i++)
	{
		float angle = (float)GetDeltaTime() * 5.0f * ((i + 1) / (i * 0.2f + 1));
		m_cubes[i]->SetTransform(rotate(m_cubes[i]->GetTransform(), radians(angle), glm::normalize(vec3(1.0f, 0.3f, 0.5f))));
	}
}

void Project::FixedUpdate() {}

void Project::LateUpdate() {}

void Project::CreateScene()
{
	CreateLights();
	//// First creates the shader
	//unsigned int ID;
	//Engine::Shader* shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
	//// Then loads the model
	//rendererRef->AddNewModel(ID, "assets/models/backpack/backpack.obj", shaderRef);
	//// Finally inform the shader
	//rendererRef->LoadLightsIntoShader(shaderRef);
	//shaderRef->SetMat4("u_model", mat4(1.0f));
	//shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(mat4(1.0f))));
	object_backpack->LoadModel("assets/models/backpack/backpack.obj", "assets/shaders/default");
	object_backpack->Translate(vec3(0.0f, 0.0f, 0.5f));
	object_backpack->Scale(vec3(0.5f));
	
	// Repeat
	//shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
	//rendererRef->AddNewModel(ID, "assets/models/cube/cube.obj", shaderRef);
	//rendererRef->LoadLightsIntoShader(shaderRef);
	//Engine::Transform trans = Engine::Transform();
	//trans.Translate(vec3(0, -2.65f, -1.1f));
	//shaderRef->SetMat4("u_model", trans.GetTransform());
	//shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(trans.GetTransform())));
	//object_cube->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default");
	//object_cube->Translate(vec3(0, -2.65f, -1.1f));

	for (uint8_t i = 0; i < s_numCubes; ++i)
	{
		m_cubes.push_back(new Engine::Entity());
		m_cubes[i]->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default");
		m_cubes[i]->Translate(m_cubePositions[i]);
		m_cubes[i]->Scale(vec3(0.6f));
	}
}

void Project::CreateLights()
{
	// Creates lights
	unsigned int ID;
	m_rendererRef->AddNewLight(ID, Engine::LightType::Directional, Engine::Colour::Silver());
	m_rendererRef->GetLightAt(ID)->SetDirection(vec3(0, -1, 0));
	m_rendererRef->AddNewLight(ID, Engine::LightType::Point, Engine::Colour::CreateWithHSV(Engine::hsv(220, 0.6f, 1.0f)));
	m_rendererRef->GetLightAt(ID)->SetCamPosition(vec4(-4, 2, -2, 1));
	m_rendererRef->AddNewLight(ID, Engine::LightType::Spot, Engine::Colour::CreateWithHSV(Engine::hsv(97, 0.17f, 1.0f)));
	m_rendererRef->GetLightAt(ID)->SetCamPosition(vec4(2.0f, 2.5f, 6.0f, 1))->SetDirection(vec3(-0.3f, -0.4f, -1))->SetAngle(13.0f)->SetBlur(0.23f);

	// Gives them physical form
	for (unsigned int i = 0; i < m_rendererRef->LightCount(); ++i)
	{
		Engine::Light* current = m_rendererRef->GetLightAt(i);
		if (current->GetType() == Engine::LightType::Point
		 || current->GetType() == Engine::LightType::Spot)
		{
			unsigned int discard;
			Engine::Shader* shaderRef = m_rendererRef->AddNewShader(ID, "assets/shaders/default");
			shaderRef->SetBool("u_justColour", true);
			shaderRef->SetVec3("u_scale", vec3(0.2f, 0.2f, (current->GetType() == Engine::LightType::Spot) ? 0.4f : 0.2f));
			shaderRef->SetVec3("u_colour", current->GetColour());
			shaderRef->SetMat4("u_model", current->GetTransform());
			shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(current->GetTransform())));
			m_rendererRef->AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef, false);
		}
	}
}
