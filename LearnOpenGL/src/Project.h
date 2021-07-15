#pragma once
#include "Application.h"

class Project : public Engine::Application
{
public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update(float pDeltaTime) override;
	void Draw() override;
};
