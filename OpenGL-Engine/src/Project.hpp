#pragma once
#include "Application.hpp"

class Project : public Engine::Application
{
private:
	Engine::Renderer* rendererRef;

	Engine::Entity ent;

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
