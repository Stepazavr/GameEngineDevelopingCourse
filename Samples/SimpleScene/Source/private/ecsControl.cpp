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


static void ProcessButtonPress(const ControllerPtr& controller,
                               const char* actionName,
                               const std::function<void()>& action,
                               bool& wasPressedFlag)
{
    if (controller.ptr->IsPressed(actionName))
    {
        if (!wasPressedFlag)
        {
            action();
            wasPressedFlag = true;
        }
    }
    else
    {
        wasPressedFlag = false;
    }
}

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<CameraManagerPtr, SavedCameraPtr, ButtonManager, const ControllerPtr>()
		.each([&](flecs::entity e, CameraManagerPtr& cameraManager, SavedCameraPtr& savedCameraPtr, ButtonManager& buttonManager, const ControllerPtr& controller)
	{
		Core::InputHandler::MouseMovevement mouseMovement = Core::InputHandler::GetInstance()->GetMouseMovement();

		mouseMovement.dx *= 0.25 * Math::Constants::PI / 180.f;
		mouseMovement.dy *= 0.25 * Math::Constants::PI / 180.f;

		Core::Camera::Ptr camera = cameraManager.ptr->GetActiveCamera();

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

		ProcessButtonPress(controller, "CreateCamera",
						   [&]() { cameraManager.ptr->CreateCamera(); },
						   buttonManager.wasPressedCreateCameraButton
						   );
		ProcessButtonPress(controller, "NextCamera",
						   [&]() { cameraManager.ptr->SwitchNextCamera(); },
						   buttonManager.wasPressedNextCameraButton
						   );
		ProcessButtonPress(controller, "PrevCamera",
						   [&]() { cameraManager.ptr->SwitchPrevCamera(); },
						   buttonManager.wasPressedPrevCameraButton
						   );
		ProcessButtonPress(controller, "SaveCamera",
						   [&]() { savedCameraPtr.ptr = cameraManager.ptr->GetActiveCamera(); },
						   buttonManager.wasPressedSaveCameraButton
						   );
		ProcessButtonPress(controller, "LoadCamera",
						   [&]() { cameraManager.ptr->SetActiveCamera(savedCameraPtr.ptr); },
						   buttonManager.wasPressedLoadCameraButton
						   );
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

