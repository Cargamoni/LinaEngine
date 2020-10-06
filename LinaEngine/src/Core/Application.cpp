/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: Lina_Application
Timestamp: 12/29/2018 10:43:46 PM

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

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)


	Application::Application(Graphics::WindowProperties windowProps)
	{
		LINA_CORE_TRACE("[Constructor] -> Application ({0})", typeid(*this).name());
		LINA_CORE_ASSERT(!instance, "Application already exists!");

		// Get engine instances.
		m_appWindow = CreateContextWindow();
		m_inputDevice = CreateInputDevice();
		m_renderEngine = CreateRenderEngine();
		m_inputEngine = CreateInputEngine();
		m_physicsEngine = CreatePhysicsEngine();

		// Create main window.
		bool windowCreationSuccess = m_appWindow->CreateContext(windowProps);
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
		delete m_physicsEngine;
		delete m_inputEngine;
		delete m_renderEngine;
		delete m_inputDevice;
		delete m_appWindow;
		LINA_CORE_TRACE("[Destructor] -> Application ({0})", typeid(*this).name());
	}

	void Application::OnEvent()
	{
		/*EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// Propagate the window resize event to render engine.
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& windowEvent = (WindowResizeEvent&)(e);
			m_RenderEngine->OnWindowResized((float)windowEvent.GetWidth(), (float)windowEvent.GetHeight());
		}

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.isHandled)
				break;
		}*/
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

			// Update layers.
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			double newTime = (double)m_appWindow->GetTime();
			double frameTime = newTime - currentTime;

			// Update current level.
			if (m_ActiveLevelExists)
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
				m_renderEngine->Render();

			// Simple FPS count
			m_FPSCounter++;

			if (currentTime - m_PreviousTime >= 1.0)
			{
				m_PreviousTime = currentTime;
				m_CurrentFPS = m_FPSCounter;
				m_FPSCounter = 0;
			}

			// Update necessary engines that the first run has finished.
			if (m_FirstRun)
			{
				m_renderEngine->PostInitialFrame();
				m_FirstRun = false;
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

	void Application::LoadLevel(LinaEngine::World::Level* level)
	{
		// TODO: Implement unloading the current level & loading a new one later.
		m_CurrentLevel = level;
		m_CurrentLevel->SetEngineReferences(&m_ECS, *m_renderEngine, *m_inputEngine);
		m_CurrentLevel->Install();
		m_CurrentLevel->Initialize();
		m_ActiveLevelExists = true;
	}


	void Application::UnloadLevel(LinaEngine::World::Level* level)
	{
		if (m_CurrentLevel == level)
		{
			m_ActiveLevelExists = false;
			m_CurrentLevel = nullptr;
		}

		level->Uninstall();
	}

	void Application::OnDrawLine(Vector3 from, Vector3 to, Color color, float width)
	{
		m_renderEngine->DrawLine(from, to, color, width);
	}
}

