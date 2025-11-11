#include <CameraManager.h>
#include <ecsControl.h>
#include <Constants.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<CameraManagerPtr, const ControllerPtr>()
		.each([&](flecs::entity e, CameraManagerPtr& cameraManager, const ControllerPtr& controller)
	{
		Core::InputHandler::MouseMovevement mouseMovement = Core::InputHandler::GetInstance()->GetMouseMovement();

		mouseMovement.dx *= 0.25 * Math::Constants::PI / 180.f;
		mouseMovement.dy *= 0.25 * Math::Constants::PI / 180.f;

		Core::Camera* camera = cameraManager.ptr->GetCamera();

		camera->Rotate(mouseMovement.dx, mouseMovement.dy);

		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera->GetViewDir();
		}
		float speed = 10.0f;
		Math::Vector3f position = camera->GetPosition() + currentMoveDir.Normalized() * speed * world.delta_time();
		camera->SetPosition(position);

		if (controller.ptr->IsPressed("CreateCamera"))
		{
			cameraManager.ptr->CreateCamera();
			ecs_sleepf(1.0);
		}
		else if (controller.ptr->IsPressed("NextCamera"))
		{
			cameraManager.ptr->SwitchNextCamera();
			ecs_sleepf(1.0);
		}
		else if (controller.ptr->IsPressed("PrevCamera"))
		{
			cameraManager.ptr->SwitchPrevCamera();
			ecs_sleepf(1.0);
		}
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.y = jump.value;
			}
		}
	});
}

