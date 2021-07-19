#pragma once
#include "Application.h"

class Project : public Engine::Application
{
public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update(double pDeltaTime) override;
	void FixedUpdate(double pDeltaTime) override;
	//void Draw() override;
};
