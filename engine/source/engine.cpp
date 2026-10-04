#include "engine.h"
#include "application.h"
#include "render/renderer2D.h"
#include "graphics/textureManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>

namespace Eng
{
	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		(void)window;
		Engine::GetInstance().GetInputHandler().SetMousePosition(glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));
	}

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		(void)window; (void)scancode; (void)mods;
		if (action == GLFW_PRESS)
		{
			Engine::GetInstance().GetInputHandler().SetKeyPressed(key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			Engine::GetInstance().GetInputHandler().SetKeyPressed(key, false);
		}
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		(void)window; (void)mods;
		if (action == GLFW_PRESS)
		{
			Engine::GetInstance().GetInputHandler().SetMouseButtonPressed(button, true);
		}
		else if (action == GLFW_RELEASE)
		{
			Engine::GetInstance().GetInputHandler().SetMouseButtonPressed(button, false);
		}
	}

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		(void)window;
		Engine::GetInstance().GetInputHandler().SetMouseScroll(glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset)));
	}

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		(void)window;
		glViewport(0, 0, width, height);
	}

	Engine& Engine::GetInstance()
	{
		static Engine instance;
		return instance;
	}

	bool Engine::Init(int width, int height, const std::string& title)
	{
		m_windowWidth = width;
		m_windowHeight = height;

		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW!\n";
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!m_window)
		{
			std::cerr << "Failed to create GLFW window!\n";
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		glfwSetKeyCallback(m_window, KeyCallback);
		glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
		glfwSetCursorPosCallback(m_window, CursorPositionCallback);
		glfwSetScrollCallback(m_window, ScrollCallback);
		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD!\n";
			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		m_graphicsAPI.Init();
		m_graphicsAPI.SetWindowSize(width, height);

		Renderer2D::Init();

		InitImGui();

		if (m_application)
		{
			return m_application->OnInit();
		}

		return true;
	}

	void Engine::Run()
	{
		if (!m_application)
		{
			std::cerr << "No application assigned to Engine!\n";
			return;
		}

		m_lastTime = std::chrono::high_resolution_clock::now();
		float fpsTimer = 0.0f;
		int frameCount = 0;

		while (!glfwWindowShouldClose(m_window) && !m_application->ShouldClose())
		{
			glfwPollEvents();

			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = currentTime - m_lastTime;
			m_deltaTime = duration.count();
			m_lastTime = currentTime;

			if (m_deltaTime > 0.1f) m_deltaTime = 0.1f;

			fpsTimer += m_deltaTime;
			frameCount++;
			if (fpsTimer >= 0.5f)
			{
				m_fps = static_cast<float>(frameCount) / fpsTimer;
				fpsTimer = 0.0f;
				frameCount = 0;
			}

			glfwGetFramebufferSize(m_window, &m_windowWidth, &m_windowHeight);
			m_graphicsAPI.SetWindowSize(m_windowWidth, m_windowHeight);
			glViewport(0, 0, m_windowWidth, m_windowHeight);

			m_application->OnUpdate(m_deltaTime);

			m_application->OnRender();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

			m_application->OnImGui();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);

			m_inputHandler.EndFrame();
		}
	}

	void Engine::Destroy()
	{
		if (m_application)
		{
			m_application->OnDestroy();
			m_application.reset();
		}

		Renderer2D::Shutdown();
		TextureManager::Get().Clear();

		ShutdownImGui();

		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}

		glfwTerminate();
	}

	void Engine::SetApplication(std::unique_ptr<Application> app)
	{
		m_application = std::move(app);
	}

	void Engine::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Engine::ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
