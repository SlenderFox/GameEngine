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
	ent = Engine::Entity();
}

bool Project::Startup()
{
	rendererRef = Engine::Renderer::GetInstance();
	CreateScene();
	return true;
}

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

void Project::CreateScene()
{
	CreateLights();
	// First creates the shader
	unsigned int ID;
	Engine::Shader* shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
	// Then loads the model
	rendererRef->AddNewModel(ID, "assets/models/backpack/backpack.obj", shaderRef);
	// Finally inform the shader
	rendererRef->LoadLightsIntoShader(shaderRef);
	shaderRef->SetMat4("u_model", mat4(1.0f));
	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(mat4(1.0f))));
	
	// Repeat
	shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/default");
	rendererRef->AddNewModel(ID, "assets/models/cube/cube.obj", shaderRef);
	rendererRef->LoadLightsIntoShader(shaderRef);
	Engine::Transform trans = Engine::Transform();
	trans.Translate(vec3(0, -2.65f, -1.1f));
	shaderRef->SetMat4("u_model", trans.GetTransform());
	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(trans.GetTransform())));
}

void Project::CreateLights()
{
	// Creates lights
	unsigned int ID;
	rendererRef->AddNewLight(ID, Engine::LightType::Directional, vec3(0.8f));
	rendererRef->GetLightAt(ID)->SetDirection(vec3(0, -1, 0));
	rendererRef->AddNewLight(ID, Engine::LightType::Point);
	rendererRef->GetLightAt(ID)->SetCamPosition(vec4(-4, 2, -2, 1));
	rendererRef->AddNewLight(ID, Engine::LightType::Spot);
	rendererRef->GetLightAt(ID)->SetCamPosition(vec4(4.5f, 3, 4.5f, 1))->SetDirection(vec3(-0.9f, -0.6f, -1))->SetAngle(10.0f)->SetBlur(0.23f);

	// Gives them physical form
	for (unsigned int i = 0; i < rendererRef->LightCount(); ++i)
	{
		if (rendererRef->GetLightAt(i)->GetType() == Engine::LightType::Point
		 || rendererRef->GetLightAt(i)->GetType() == Engine::LightType::Spot)
		{
			unsigned int discard;
			Engine::Shader* shaderRef = rendererRef->AddNewShader(ID, "assets/shaders/light");
			shaderRef->SetVec3("u_colour", rendererRef->GetLightAt(i)->GetColour());
			shaderRef->SetMat4("u_model", translate(mat4(1.0f), vec3(rendererRef->GetLightAt(i)->GetPosition())));
			shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)transpose(inverse(mat4(1.0f))));
			rendererRef->AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef);
		}
	}
}
