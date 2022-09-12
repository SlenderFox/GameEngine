#pragma region
#include "Project.hpp"
#include "glm/gtc/matrix_transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::translate;
using glm::transpose;
using glm::inverse;
#pragma endregion

Project::Project()
{
	object_lightPoint = new Engine::Entity();
	object_lightSpot = new Engine::Entity();
	object_cube = new Engine::Entity();
	object_backpack = new Engine::Entity();
}

Project::~Project() {}

bool Project::Startup()
{
	rendererRef = Engine::Renderer::GetInstance();
	CreateScene();
	return true;
}

void Project::Shutdown() {}

void Project::Update(double pDeltaTime)
{
	//for (unsigned int j = 0; j < 10; j++)
	//{
	//	mat4 model = mat4(1.0f);
	//	model = glm::translate(model, m_cubePositions[j]);
	//	float angle = (float)pTime * 5.0f * ((j + 1) / (j * 0.2f + 1));
	//	model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
	//	GetShaderAt(0U)->SetMat4("u_model", (mat4)model);
	//	mat3 transposeInverseOfModel = mat3(glm::transpose(glm::inverse(model)));
	//	GetShaderAt(0U)->SetMat3("u_transposeInverseOfModel", (mat3)transposeInverseOfModel);
	//	GetMeshAt(i)->Draw();
	//}
}

void Project::FixedUpdate(double pFixedDeltaTime) {}

void Project::LateUpdate(double pDeltaTime) {}

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
	
	// Repeat
	//shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
	//rendererRef->AddNewModel(ID, "assets/models/cube/cube.obj", shaderRef);
	//rendererRef->LoadLightsIntoShader(shaderRef);
	//Engine::Transform trans = Engine::Transform();
	//trans.Translate(vec3(0, -2.65f, -1.1f));
	//shaderRef->SetMat4("u_model", trans.GetTransform());
	//shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(trans.GetTransform())));
	object_cube->LoadModel("assets/models/cube/cube.obj", "assets/shaders/default");
	object_cube->Translate(vec3(0, -2.65f, -1.1f));
}

void Project::CreateLights()
{
	// Creates lights
	unsigned int ID;
	rendererRef->AddNewLight(ID, Engine::LightType::Directional, Engine::Colour::Silver());
	rendererRef->GetLightAt(ID)->SetDirection(vec3(0, -1, 0));
	rendererRef->AddNewLight(ID, Engine::LightType::Point, Engine::Colour::CreateWithHSV(Engine::hsv(220, 0.6f, 1.0f)));
	rendererRef->GetLightAt(ID)->SetCamPosition(vec4(-4, 2, -2, 1));
	rendererRef->AddNewLight(ID, Engine::LightType::Spot, Engine::Colour::CreateWithHSV(Engine::hsv(97, 0.17f, 1.0f)));
	rendererRef->GetLightAt(ID)->SetCamPosition(vec4(4.5f, 3, 4.5f, 1))->SetDirection(vec3(-0.9f, -0.6f, -1))->SetAngle(10.0f)->SetBlur(0.23f);

	// Gives them physical form
	for (unsigned int i = 0; i < rendererRef->LightCount(); ++i)
	{
		Engine::Light* current = rendererRef->GetLightAt(i);
		if (current->GetType() == Engine::LightType::Point
		 || current->GetType() == Engine::LightType::Spot)
		{
			unsigned int discard;
			Engine::Shader* shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
			shaderRef->SetBool("u_justColour", true);
			shaderRef->SetVec3("u_scale", vec3(0.2f, 0.2f, (current->GetType() == Engine::LightType::Spot) ? 0.4f : 0.2f));
			shaderRef->SetVec3("u_colour", current->GetColour());
			shaderRef->SetMat4("u_model", current->GetTransform());
			shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(current->GetTransform())));
			rendererRef->AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef, false);
		}
	}
}
