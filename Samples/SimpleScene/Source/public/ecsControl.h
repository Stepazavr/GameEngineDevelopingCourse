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
};

struct JumpSpeed
{
	float value;
};

struct CameraManagerPtr
{
	GameEngine::Core::CameraManager* ptr;
};

void RegisterEcsControlSystems(flecs::world& world);

