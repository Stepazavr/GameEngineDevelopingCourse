#pragma once

#include <flecs.h>

#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN
#endif


ECS_STRUCT(TriggerTimer,
	{
		bool isActive;
		float value;
	});

ECS_STRUCT(Alive,
	{
		bool isAlive;
	});

ECS_STRUCT(CreateEntity,
	{
		bool create;
		bool canCreate;
	});

ECS_STRUCT(TargetEntity,
	{
		uint64_t target;
	});