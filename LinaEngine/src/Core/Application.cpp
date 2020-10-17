/* 
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "LinaPch.hpp"
#include "Core/Application.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Input/InputEngine.hpp"
#include "Rendering/Window.hpp"
#include "Core/Layer.hpp"
#include "World/Level.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/LightComponent.hpp"
#include "ECS/Components/MeshRendererComponent.hpp"
#include "ECS/Components/FreeLookComponent.hpp"
#include <entt/meta/meta.hpp>
#include <entt/meta/factory.hpp>
#include <GLFW/glfw3.h>


namespace LinaEngine
{

	Application::Application()
	{
		// Make sure log event is delegated to the application.
		Log::s_onLog = std::bind(&Application::OnLog, this, std::placeholders::_1);

		LINA_CORE_TRACE("[Constructor] -> Application ({0})", typeid(*this).name());
		LINA_CORE_ASSERT(!instance, "Application already exists!");
	}

	void Application::Initialize(Graphics::WindowProperties& props)
	{
		// Get engine instances.
		m_appWindow = CreateContextWindow();
		m_inputDevice = CreateInputDevice();
		m_renderEngine = CreateRenderEngine();
		m_inputEngine = CreateInputEngine();
		m_physicsEngine = CreatePhysicsEngine();

		// Create main window.
		bool windowCreationSuccess = m_appWindow->CreateContext(props);
		if (!windowCreationSuccess)
		{
			LINA_CORE_ERR("Window Creation Failed!");
			return;
		}

		// Set callbacks.
		m_KeyCallback = std::bind(&Application::KeyCallback, this, std::placeholders::_1, std::placeholders::_2);
		m_MouseCallback = std::bind(&Application::MouseCallback, this, std::placeholders::_1, std::placeholders::_2);
		m_WindowResizeCallback = std::bind(&Application::OnWindowResize, this, std::placeholders::_1);
		m_WindowClosedCallback = std::bind(&Application::OnWindowClose, this);
		m_drawLineCallback = std::bind(&Application::OnDrawLine, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		m_postSceneDrawCallback = std::bind(&Application::OnPostSceneDraw, this);


		// Set event callback for main window.
		m_appWindow->SetKeyCallback(m_KeyCallback);
		m_appWindow->SetMouseCallback(m_MouseCallback);
		m_appWindow->SetWindowResizeCallback(m_WindowResizeCallback);
		m_appWindow->SetWindowClosedCallback(m_WindowClosedCallback);
		m_renderEngine->SetPostSceneDrawCallback(m_postSceneDrawCallback);
		m_renderEngine->SetViewportDisplay(Vector2::Zero, m_appWindow->GetSize());

		// Initialize engines.
		m_inputEngine->Initialize(m_appWindow->GetNativeWindow(), m_inputDevice);
		m_physicsEngine->Initialize(m_ECS, m_drawLineCallback);
		m_renderEngine->Initialize(m_ECS, *m_appWindow);

		// Set running flag.
		m_Running = true;
	}


	Application::~Application()
	{
		if (m_physicsEngine)
			delete m_physicsEngine;

		if (m_inputEngine)
			delete m_inputEngine;

		if (m_renderEngine)
			delete m_renderEngine;

		if (m_inputDevice)
			delete m_inputDevice;

		if (m_appWindow)
			delete m_appWindow;

		LINA_CORE_TRACE("[Destructor] -> Application ({0})", typeid(*this).name());
	}

	void Application::OnLog(Log::LogDump dump)
	{
		// Dump to cout
		std::cout << dump.m_message << std::endl;

		// Dispatch the action to any listeners.
		DispatchAction<Log::LogDump>(Action::ActionType::MessageLogged,dump);
	}

	void Application::Run()
	{
		double t = 0.0;
		double dt = 0.01;
		double currentTime = (double)m_appWindow->GetTime();
		double accumulator = 0.0;

		while (m_Running)
		{
			// Update input engine.
			m_inputEngine->Tick();

			double newTime = (double)m_appWindow->GetTime();
			double frameTime = newTime - currentTime;

			// Update layers.
			for (Layer* layer : m_LayerStack)
				layer->OnTick(frameTime);

			// Update current level.
			if (m_activeLevelExists)
				m_CurrentLevel->Tick(frameTime);

			if (frameTime > 0.25)
				frameTime = 0.25;

			currentTime = newTime;
			accumulator += frameTime;

			while (accumulator >= dt)
			{
				// Update physics engine.
				m_physicsEngine->Tick(dt);
				t += dt;
				accumulator -= dt;
			}

			// Update render engine.
			if (m_canRender)
			{
				// render level.
				if (m_activeLevelExists)
					m_renderEngine->Render();

				// Update gui layers & swap buffers
				m_renderEngine->TickAndSwap(frameTime);
			}


			// Simple FPS count
			m_FPSCounter++;

			if (currentTime - m_PreviousTime >= 1.0)
			{
				m_PreviousTime = currentTime;
				m_CurrentFPS = m_FPSCounter;
				m_FPSCounter = 0;
			}

			// Update necessary engines that the first run has finished.
			if (m_firstRun)
			{
				m_firstRun = false;
			}

		}
	}

	bool Application::OnWindowClose()
	{
		m_Running = false;
		return true;
	}

	void Application::OnWindowResize(Vector2 size)
	{
		if (size.x == 0.0f || size.y == 0.0f)
			m_canRender = false;
		else
			m_canRender = true;

		m_renderEngine->SetViewportDisplay(Vector2::Zero, size);
	}

	void Application::OnPostSceneDraw()
	{
		m_physicsEngine->OnPostSceneDraw();
	}

	void Application::KeyCallback(int key, int action)
	{
		m_inputEngine->DispatchKeyAction(static_cast<LinaEngine::Input::InputCode::Key>(key), action);
	}

	void Application::MouseCallback(int button, int action)
	{
		m_inputEngine->DispatchMouseAction(static_cast<LinaEngine::Input::InputCode::Mouse>(button), action);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::InstallLevel(LinaEngine::World::Level* level)
	{
		level->SetEngineReferences(&m_ECS, *m_renderEngine, *m_inputEngine);
		return level->Install();
	}

	void Application::InitializeLevel(LinaEngine::World::Level* level)
	{
		m_CurrentLevel = level;
		m_CurrentLevel->Initialize();
		m_activeLevelExists = true;
	}


	void Application::UnloadLevel(LinaEngine::World::Level* level)
	{
		if (m_CurrentLevel == level)
		{
			m_activeLevelExists = false;
			m_CurrentLevel = nullptr;
		}

		level->Uninstall();
	}

	void Application::OnDrawLine(Vector3 from, Vector3 to, Color color, float width)
	{
		m_renderEngine->DrawLine(from, to, color, width);
	}
}

