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

struct CameraPtr
{
	GameEngine::Core::Camera::Ptr ptr = nullptr;
};


struct FixedCameraPtr
{
	float dx;
	float dy;
	float dz;
	GameEngine::Core::Camera::Ptr ptr = nullptr;
};

struct SavedCameraPtr
{
	GameEngine::Core::Camera::WeakPtr ptr;
};

void RegisterEcsControlSystems(flecs::world& world);

