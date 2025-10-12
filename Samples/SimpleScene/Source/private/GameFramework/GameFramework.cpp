// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#define GAME_FRAMEWORK

#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsLogic.h>
#include <ECS/ecsSystems.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <flecs.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponents();
	RegisterSystems();


	m_PatronageSistemEntity = m_World.entity()
		.set(CreateEntity{ .create = false, .canCreate = true })
		.set(ControllerPtr{
			new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini"))
			});


	cubeControl = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })
		.set(Alive{ true });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(TriggerTimer{ .isActive = false, .value = 2.0f })
		.set(Alive{ true })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });
}

void GameFramework::RegisterComponents()
{
	// Exposing these components for the lua system
	ECS_META_COMPONENT(m_World, Position);
	ECS_META_COMPONENT(m_World, Velocity);
	ECS_META_COMPONENT(m_World, Gravity);
	ECS_META_COMPONENT(m_World, BouncePlane);
	ECS_META_COMPONENT(m_World, Bounciness);
	ECS_META_COMPONENT(m_World, ShiverAmount);
	ECS_META_COMPONENT(m_World, FrictionAmount);
	ECS_META_COMPONENT(m_World, Speed);
	ECS_META_COMPONENT(m_World, TriggerTimer);
	ECS_META_COMPONENT(m_World, Alive);
	ECS_META_COMPONENT(m_World, TargetEntity);
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
}

void GameFramework::Update(float dt)
{
	if (m_PatronageSistemEntity.get<CreateEntity>()->create) {

		flecs::entity shootingCube = m_World.entity()
			.set(Position{ Core::g_MainCamera->GetPosition().x, Core::g_MainCamera->GetPosition().y, Core::g_MainCamera->GetPosition().z })
			.set(Velocity{ Core::g_MainCamera->GetViewDir().x * 20.0f, Core::g_MainCamera->GetViewDir().y * 20.0f, Core::g_MainCamera->GetViewDir().z * 20.0f })
			.set(Gravity{ 0.f, -9.8065f, 0.f })
			.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
			.set(Bounciness{ 0.3f })
			.set(FrictionAmount{ 0.4f })
			.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
			.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
			.set(TriggerTimer{ .isActive = false, .value = 5.0f})
			.set(Alive{ true })
			.set(TargetEntity{ cubeControl.id() });

		m_PatronageSistemEntity.set<CreateEntity>(
			{ false, m_PatronageSistemEntity.get<CreateEntity>()->canCreate }
		);
	}
}