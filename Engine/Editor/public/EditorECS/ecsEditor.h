#pragma once

#include <flecs.h>

namespace GameEngine::Core
{
	class CameraManager;
}

namespace GameEngine::EntitySystem::EditorECS
{
	struct CameraManagerPtr
	{
		Core::CameraManager* ptr;
	};

	struct Position
	{
		float x;
		float y;
		float z;
	};

	struct Speed
	{
		float value;
	};

	void RegisterEditorEcsControlSystems(flecs::world& world);
}
