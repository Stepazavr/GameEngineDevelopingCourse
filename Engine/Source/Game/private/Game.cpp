#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>
#include <MathHelper.h>

#include <cstdlib>

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

		m_GameTimer.Reset();
		srand(static_cast<unsigned int>(m_GameTimer.GetBeginTime()));

		// How many objects do we want to create
		Math::Vector3f startPos = Math::Vector3f(-10.0f, 0.0f, -10.0f);
		Math::Vector3f startVelObj = Math::Vector3f::Zero();
		float margin = 5.0f;
		Math::Vector3f startPosObj = startPos;

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				startPosObj = startPos + Math::Vector3f(margin * i, 0.0f, margin * j);

				int randomType = rand() % 3;
				ObjectType type = ObjectType::Unknown;
				switch (randomType) {
					case 0: 
						type = ObjectType::Jumping; 
						startVelObj = Math::Vector3f(0.0f, 10.0f, 0.0f);
						break;
					case 1: 
						type = ObjectType::Moving;
						startVelObj = Math::Vector3f(0.0f, 0.0f, 10.0f);
						break;
					case 2: type = ObjectType::Ñontrolled;
						startVelObj = Math::Vector3f::Zero();
						break;
				}
				m_Objects.push_back(new GameObject(type, startPosObj, startVelObj));
				Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
				m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
			}
		}

		Core::g_InputHandler->RegisterCallback("GoForwardCam", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBackCam", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRightCam", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeftCam", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoRightObject", [&]() { MoveObjects(Math::Vector3f(1.0f, 0.0f, 0.0f)); });
		Core::g_InputHandler->RegisterCallback("GoLeftObject", [&]() { MoveObjects(Math::Vector3f(-1.0f, 0.0f, 0.0f)); });
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

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (GameObject* gameObject : m_Objects) {

			Math::Vector3f pos = gameObject->GetPosition();
			Math::Vector3f vel = gameObject->GetVelosity();


			// Showcase
			switch (gameObject->GetType()) {
			case ObjectType::Ñontrolled:
			{
				float speed = 10.0f;
				pos = pos + gameObject->GetMoveDir().Normalized() * speed * dt;
				gameObject->SetMoveDir(Math::Vector3f::Zero());
				break;
			}
			case ObjectType::Jumping:
			{
				float g = 10.0f;
				vel.y -= g * dt;
				pos.y += vel.y * dt;
				if (pos.y <= 0.0f) {
					pos.y = gameObject->GetStartPosition().y;
					vel.y = gameObject->GetStartVelosity().y;
				}
				break;
			}
			case ObjectType::Moving:
			{
				float omega2 = 1.0f;
				float acceleration = -omega2 * (pos.z - gameObject->GetStartPosition().z);
				vel.z += acceleration * dt;
				pos.z += vel.z * dt;
				break;
			}
			}
			gameObject->SetPosition(pos, m_renderThread->GetMainFrame());
			gameObject->SetVelosity(vel);
		}
	}

	void Game::MoveObjects(Math::Vector3f dir) {
		for (GameObject* gameObject : m_Objects) {
			if (gameObject->GetType() == ObjectType::Ñontrolled) {
				gameObject->Move(dir);
			}
		}
	}
}