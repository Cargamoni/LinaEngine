#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Lina.hpp>
#include "PackageManager/PAMWindow.hpp"
#include "PackageManager/PAMInputDevice.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Input/InputEngine.hpp"
#include "Levels/DefaultEditorLevel.hpp"

namespace LinaEditor
{
	class EditorApplication : public LinaEngine::Application
	{
	public:

		EditorApplication() {
			LINA_CLIENT_TRACE("[Constructor] -> Editor Application ({0})", typeid(*this).name());

			// Create layers
			// m_guiLayer = new LinaEditor::GUILayer();

			// Setup layers
			//m_guiLayer->Setup(GetAppWindow(), GetRenderEngine(), GetPhysicsEngine(), this, GetECSREgistry(), m_StartupLevel, m_scenePanelSize);

			// Load startup level.
			LoadLevel(&m_StartupLevel);

			// Push layer into the engine. ** WHILE LOOP INSIDE ** 
			//GetRenderEngine().PushLayer(m_guiLayer);

			// Set display size to match scene panel.
			//PushOverlay(new LinaEngine::Layer_IMGUI());

		}
		~EditorApplication() {
			LINA_CLIENT_TRACE("[Destructor] -> Editor Application ({0})", typeid(*this).name());

		}


	private:


		Vector2 m_scenePanelSize = Vector2(800, 600);
		DefaultEditorLevel m_StartupLevel;

	};

}


LinaEngine::Application* LinaEngine::CreateApplication()
{
	return new LinaEditor::EditorApplication();
}

// Default platform context window.
LinaEngine::Graphics::Window* LinaEngine::CreateContextWindow()
{
	return new ContextWindow();
}

// Default platform input device.
LinaEngine::Input::InputDevice* LinaEngine::CreateInputDevice()
{
	return new InputDevice();
}

// Default engine
LinaEngine::Graphics::RenderEngine* LinaEngine::CreateRenderEngine()
{
	return new LinaEngine::Graphics::RenderEngine();
}

// Default engine
LinaEngine::Physics::PhysicsEngine* LinaEngine::CreatePhysicsEngine()
{
	return new LinaEngine::Physics::PhysicsEngine();
}

// Default engine
LinaEngine::Input::InputEngine* LinaEngine::CreateInputEngine()
{
	return new LinaEngine::Input::InputEngine();
}
