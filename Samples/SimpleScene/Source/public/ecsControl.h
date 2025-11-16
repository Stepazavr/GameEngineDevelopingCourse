#pragma once

#include <flecs.h>

namespace GameEngine::Core
{
	class CameraManager;
	class Controller;
}

struct ControllerPtr
{
	GameEngine::Core::Controller* ptr;
};

struct ButtonManager
{
	bool wasPressedCreateCameraButton;
	bool wasPressedNextCameraButton;
	bool wasPressedPrevCameraButton;
	bool wasPressedSaveCameraButton;
	bool wasPressedLoadCameraButton;
};

struct JumpSpeed
{
	float value;
};

struct CameraManagerPtr
{
	GameEngine::Core::CameraManager* ptr;
};

struct SavedCameraPtr
{
	GameEngine::Core::Camera::Ptr ptr;
};

void RegisterEcsControlSystems(flecs::world& world);

