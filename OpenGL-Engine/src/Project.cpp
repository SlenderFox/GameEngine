#include "Project.hpp"
#include "glm/gtc/matrix_transform.hpp"

bool Project::Startup()
{
	renderer = Engine::Renderer::GetInstance();
	CreateScene();
	return true;
}

void Project::CreateScene()
{
	CreateLights();
	// First creates the shader
	unsigned int ID;
	Engine::Shader* shaderRef = renderer->AddNewShader(ID, "assets/shaders/default");
	// Then loads the model
	renderer->AddNewModel(ID, "assets/models/backpack/backpack.obj", shaderRef);
	// Finally inform the shader
	renderer->LoadLightsIntoShader(shaderRef);
	shaderRef->SetMat4("u_model", mat4(1.0f));
	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
	
	// Repeat
	shaderRef = renderer->AddNewShader(ID, "assets/shaders/default");
	renderer->AddNewModel(ID, "assets/models/cube/cube.obj", shaderRef);
	renderer->LoadLightsIntoShader(shaderRef);
	Engine::Transform trans = Engine::Transform();
	trans.Translate(vec3(0, -2.65f, -1.1f));
	shaderRef->SetMat4("u_model", trans.GetTransform());
	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(trans.GetTransform())));
}

void Project::CreateLights()
{
	// Creates lights
	unsigned int ID;
	renderer->AddNewLight(ID, Engine::LightType::Directional, vec3(0.8f));
	renderer->GetLightAt(ID)->SetDirection(vec3(0, -1, 0));
	renderer->AddNewLight(ID, Engine::LightType::Point);
	renderer->GetLightAt(ID)->SetCamPosition(vec4(-4, 2, -2, 1));
	renderer->AddNewLight(ID, Engine::LightType::Spot);
	renderer->GetLightAt(ID)->SetCamPosition(vec4(4.5f, 3, 4.5f, 1))->SetDirection(vec3(-0.9f, -0.6f, -1))->SetAngle(10.0f)->SetBlur(0.23f);

	// Gives them physical form
	for (unsigned int i = 0; i < renderer->LightCount(); ++i)
	{
		if (renderer->GetLightAt(i)->GetType() == Engine::LightType::Point
		 || renderer->GetLightAt(i)->GetType() == Engine::LightType::Spot)
		{
			unsigned int discard;
			Engine::Shader* shaderRef = renderer->AddNewShader(ID, "assets/shaders/light");
			shaderRef->SetVec3("u_colour", renderer->GetLightAt(i)->GetColour());
			shaderRef->SetMat4("u_model", glm::translate(mat4(1.0f), vec3(renderer->GetLightAt(i)->GetPosition())));
			shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
			renderer->AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef);
		}
	}
}
