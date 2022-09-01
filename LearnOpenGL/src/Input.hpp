#pragma once

struct GLFWwindow;

namespace Engine
{
	// Singleton class used for handling inputs
	class Input
	{
		friend class Application;	// Allowing application access and control
	public:
		static Input *GetInstance()
		{
			static Input *sm_instance = new Input();
			return sm_instance;
		}

		//void ProcessInput(GLFWwindow *pWindow);

	private:
		#pragma region Constructors
		Input() = default;
		~Input() {};
		// Delete copy/move so extra instances can't be created/moved.
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(Input&&) = delete;
		#pragma endregion
	};
}
