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

#include "Core/EditorApplication.hpp"
#include "Core/Application.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Panels/ECSPanel.hpp"
#include "Core/SplashScreen.hpp"
#include "Core/GUILayer.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/FreeLookComponent.hpp"
#include "ECS/Systems/FreeLookSystem.hpp"

using namespace LinaEngine::ECS;

namespace LinaEditor
{
#define EDITOR_CAMERA_NAME "Editor Camera"

	LinaEngine::Action::ActionDispatcher EditorApplication::s_editorDispatcher;

	EditorApplication::EditorApplication()
	{
		s_editorDispatcher.Initialize(LinaEngine::Action::ActionType::EditorActionsStartIndex, LinaEngine::Action::ActionType::EditorActionsEndIndex);

	}

	EditorApplication::~EditorApplication()
	{
		LINA_CLIENT_TRACE("[Destructor] -> Editor Application ({0})", typeid(*this).name());
	}

	void EditorApplication::Setup()
	{
		LINA_CLIENT_TRACE("[Constructor] -> Editor Application ({0})", typeid(*this).name());

		LinaEngine::Graphics::WindowProperties splashProps;
		splashProps.m_width = 720;
		splashProps.m_height = 450;
		splashProps.m_decorated = false;
		splashProps.m_resizable = false;

		SplashScreen* splash = new SplashScreen();
		splash->Setup(splashProps);
		splash->Draw(); // We should carry this over to a separate thread later on when things are more complex and requires data shown to the user while loading.

		// Remove splash.
		delete splash;

		LinaEngine::Application::GetRenderEngine().PushLayer(m_guiLayer);

		LinaEngine::Application::GetEngineDispatcher().SubscribeAction<LinaEngine::World::Level*>("##linaeditor_level_init", LinaEngine::Action::ActionType::LevelInitialized,
			std::bind(&EditorApplication::LevelInstalled, this, std::placeholders::_1));

		m_freeLookSystem.Construct(LinaEngine::Application::GetECSRegistry(), LinaEngine::Application::GetInputEngine());
		m_freeLookSystem.SystemActivation(true);
		LinaEngine::Application::GetApp().AddToMainPipeline(m_freeLookSystem);


	}

	void EditorApplication::Refresh()
	{
		m_guiLayer.Refresh();
	}

	void EditorApplication::LevelInstalled(LinaEngine::World::Level* level)
	{
		ECSRegistry& ecs = LinaEngine::Application::GetECSRegistry();

		auto singleView = ecs.view<LinaEngine::ECS::ECSEntityData>();

		bool editorCameraExists = false;

		for (auto entity : singleView)
		{
			if (singleView.get<LinaEngine::ECS::ECSEntityData>(entity).m_name.compare(EDITOR_CAMERA_NAME) == 0)
			{
				editorCameraExists = true;
				break;
			}
		}

		if(!editorCameraExists)
		{
			ECSEntity editorCamera = ecs.CreateEntity(EDITOR_CAMERA_NAME);
			TransformComponent cameraTransform;
			CameraComponent cameraComponent;
			FreeLookComponent freeLookComponent;
			ecs.emplace<TransformComponent>(editorCamera, cameraTransform);
			ecs.emplace<CameraComponent>(editorCamera, cameraComponent);
			ecs.emplace<FreeLookComponent>(editorCamera, freeLookComponent);
			Refresh();
		}

	}


}
