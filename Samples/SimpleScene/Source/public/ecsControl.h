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

