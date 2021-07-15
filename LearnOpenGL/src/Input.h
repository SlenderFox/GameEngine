#pragma once

struct GLFWwindow;

namespace Engine
{
	// Singleton class used for handling inputs
	class Input
	{
	public:
		static Input* GetInstance() { return m_instance; }

		void ProcessInput(GLFWwindow* pWindow);

	protected:
		// Just giving the Application class access to the Input singleton
		friend class Application;

		// Singleton pointer
		static Input* m_instance;

		// Only want the Application class to be able to create / destroy
		static void Create() { m_instance = new Input(); }
		static void Destroy() { delete m_instance; }

	private:
		Input() {}
		~Input() {}
	};
}
