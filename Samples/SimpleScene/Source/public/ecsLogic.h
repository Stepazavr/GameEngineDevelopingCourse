#pragma once


#include <flecs.h>

struct Patronage
{
	int value;
	int restartValue;
};

struct TriggerTimer
{
	bool isActive;
	float value;
	float restartValue;
};

struct Alive
{
	bool isAlive;
};

struct CreateEntity
{
	bool create;
	bool canCreate;
};

struct TargetEntity
{
	flecs::entity target;
};

void RegisterEcsLogicSystems(flecs::world& world);