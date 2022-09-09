#pragma once
#include "Application.hpp"

class Project : public Engine::Application
{
private:
	Engine::Renderer* rendererRef;

	Engine::Entity* object_lightPoint;
	Engine::Entity* object_lightSpot;
	Engine::Entity* object_backpack;
	Engine::Entity* object_cube;

	void CreateScene();
	void CreateLights();

public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update(double pDeltaTime) override;
	void FixedUpdate(double pFixedDeltaTime) override;
	void LateUpdate(double pDeltaTime) override;
};
