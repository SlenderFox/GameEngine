#pragma once
#include "Application.hpp"

class Project : public Engine::Application
{
public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update(double pDeltaTime) override;
	void FixedUpdate(double pFixedDeltaTime) override;
	//void Draw() override;
};
