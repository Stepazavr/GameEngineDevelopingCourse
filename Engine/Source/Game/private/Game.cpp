#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <string>
#include <windows.h>

#include "INIReader.h"

namespace {

	int getKeyCode(const std::string& key) {
		if (key.size() == 1 && std::isalpha(key[0])) {
			return std::toupper(key[0]);
		}
		if (key == "SPACE") return VK_SPACE;
		if (key == "ESC")   return VK_ESCAPE;

		return 0;
	}

	void LoadConfig()
	{
		INIReader reader("../../../../../Configs/config.ini");

		if (reader.ParseError() < 0) {
			return;
		}
		std::string keyStr = reader.Get("controls", "key_button", "A");

		if (getKeyCode(keyStr)) {
			GameEngine::Core::g_MainWindowsApplication->SetKeyButton(getKeyCode(keyStr));
		}
	}
}

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 3; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		LoadConfig();
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Update(dt);
			
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderThread->OnEndFrame();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			Math::Vector3f pos = m_Objects[i]->GetPosition();

			// Showcase
			if (i == 0)
			{
				pos.x += 0.5f * dt;
			}
			else if (i == 1)
			{
				pos.y -= 0.5f * dt;
			}
			else if (i == 2)
			{
				pos.x += 0.5f * dt;
				pos.y -= 0.5f * dt;
			}
			m_Objects[i]->SetPosition(pos, m_renderThread->GetMainFrame());
		}
	}
}