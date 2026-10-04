#ifndef ENG_ENGINE_H
#define ENG_ENGINE_H

#include <memory>
#include <chrono>
#include <string>
#include "input/inputHandler.h"
#include "graphics/graphicsAPI.h"

struct GLFWwindow;

namespace Eng
{
	class Application;

	class Engine
	{
	public:
		static Engine& GetInstance();

	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;

	public:
		bool Init(int width = 1280, int height = 720, const std::string& title = "Particle Engine");
		void Run();
		void Destroy();

		void SetApplication(std::unique_ptr<Application> app);
		Application* GetApplication() const { return m_application.get(); }

		InputHandler& GetInputHandler() { return m_inputHandler; }
		GraphicsAPI& GetGraphicsAPI() { return m_graphicsAPI; }
		GLFWwindow* GetWindow() const { return m_window; }

		int GetWindowWidth() const { return m_windowWidth; }
		int GetWindowHeight() const { return m_windowHeight; }

		float GetDeltaTime() const { return m_deltaTime; }
		float GetFPS() const { return m_fps; }

	private:
		void InitImGui();
		void ShutdownImGui();

	private:
		std::unique_ptr<Application> m_application = nullptr;
		GLFWwindow* m_window = nullptr;

		int m_windowWidth = 1280;
		int m_windowHeight = 720;

		InputHandler m_inputHandler;
		GraphicsAPI m_graphicsAPI;

		std::chrono::high_resolution_clock::time_point m_lastTime;
		float m_deltaTime = 0.0f;
		float m_fps = 0.0f;
	};
}

#endif
