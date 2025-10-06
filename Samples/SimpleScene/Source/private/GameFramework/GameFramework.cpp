#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsLogic.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>

#include <Input/InputHandler.h>
#include <Input/Buttons.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsPhysSystems(m_World);
	RegisterEcsLogicSystems(m_World);

	m_PatronageSistemEntity = m_World.entity()
		.set(Patronage{ 6, 6 })
		.set(TriggerTimer{ .isActive = false, .value = 4.0f, .restartValue = 4.0f })
		.set(CreateEntity{ .create = false, .canCreate = true })
		.set(ControllerPtr{ 
			new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) 
			});

	m_Disappearing—ubeEntity = m_World.entity()
		.set(Position{ Math::Vector3f(-2.f, 0.f, 0.f) })
		.set(Velocity{ Math::Vector3f::Zero() })
		.set(FrictionAmount{ 0.9f })
		.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
		.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
		.set(Bounciness{ 0.3f })
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(RenderObjectPtr{ new Render::RenderObject() })
		.set(Alive{ true })
		.set(TargetEntity{ m_PatronageSistemEntity });

	flecs::entity camera = m_World.entity()
		.set(Position{ Math::Vector3f(0.0f, 12.0f, -10.0f) })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ 
			new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) 
		});
}

void GameFramework::Update(float dt)
{
	if (m_PatronageSistemEntity.get<CreateEntity>()->create) {

		flecs::entity shootingCube = m_World.entity()
			.set(Position{ Core::g_MainCamera->GetPosition() })
			.set(Velocity{ Core::g_MainCamera->GetViewDir() * 20.0f })
			.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
			.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
			.set(Bounciness{ 0.5f })
			.set(FrictionAmount{ 0.5f })
			.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
			.set(RenderObjectPtr{ new Render::RenderObject() })
			.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })
			.set(TriggerTimer{ .isActive = false, .value = 5.0f, .restartValue = 0.0f })
			.set(Alive{ true })
			.set(TargetEntity{ m_Disappearing—ubeEntity });

		m_PatronageSistemEntity.set<CreateEntity>(
			{ false, m_PatronageSistemEntity.get<CreateEntity>()->canCreate}
		);
	}
}