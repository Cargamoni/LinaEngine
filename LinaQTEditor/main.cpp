#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Core/Application.hpp"
#include "PackageManager/PAMWindow.hpp"
#include "PackageManager/PAMInputDevice.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Input/InputEngine.hpp"
#include "Levels/DefaultEditorLevel.hpp"
#include "Rendering/ContextWindow.hpp"
#include "Utility/Log.hpp"

namespace LinaEditor
{
	class EditorApplication : public LinaEngine::Application
	{
	public:

		EditorApplication(LinaEngine::Graphics::WindowProperties windowProps = LinaEngine::Graphics::WindowProperties()) : Application(windowProps) {
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
	QGuiApplication app();

	QSurfaceFormat format;
	format.setSamples(16);
	return new LinaEditor::EditorApplication();
}

// Default platform context window.
LinaEngine::Graphics::Window* LinaEngine::CreateContextWindow()
{
	//return new LinaEditor::Graphics::ContextWindow();
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

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
		&app, [url](QObject* obj, const QUrl& objUrl) {
			if (!obj && url == objUrl)
				QCoreApplication::exit(-1);
		}, Qt::QueuedConnection);
	engine.load(url);

	LinaEngine::Log::Init();

	LinaEngine::Graphics::WindowProperties props = LinaEngine::Graphics::WindowProperties();
	LinaEditor::EditorApplication* editorApp = new LinaEditor::EditorApplication(props);
	editorApp->Run();
	delete editorApp;

	return app.exec();
}
