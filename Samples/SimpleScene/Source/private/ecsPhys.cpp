#include <ecsPhys.h>
#include <flecs.h>

namespace
{
	inline float rand_flt(float from, float to)
	{
		return from + (float(rand()) / RAND_MAX) * (to - from);
	}
}

using Vector3f = GameEngine::Math::Vector3f;

void RegisterEcsPhysSystems(flecs::world& world)
{

	world.system<Velocity, const Gravity, BouncePlane*, Position*>()
		.each([&](flecs::entity e, Velocity& vel, const Gravity& grav, BouncePlane* plane, 
				  Position* pos) {

		if (plane && pos)
		{
			constexpr float planeEpsilon = 0.1f;
			if (pos->value * Vector3f{ plane->value.x, plane->value.y, plane->value.z } < plane->value.w + planeEpsilon)
			{
				return;
			}
		}
		vel.value = vel.value + Vector3f{ grav.value } * world.delta_time();
	});


	world.system<Velocity, Position, const BouncePlane, const Bounciness>()
		.each([&](Velocity& vel, Position& pos, const BouncePlane& plane, const Bounciness& bounciness)
	{
		Vector3f normal = Vector3f{ plane.value.x, plane.value.y, plane.value.z };
		float dotPos = pos.value * normal;
		float dotVel = vel.value * normal;
		if (dotPos < plane.value.w)
		{
			pos.value = pos.value - normal * (dotPos - plane.value.w);

			vel.value = vel.value - normal * (1.f + bounciness.value) * dotVel;
		}
	});


	world.system<Velocity, const FrictionAmount>()
		.each([&](flecs::entity e, Velocity& vel, const FrictionAmount& friction)
	{
		vel.value = vel.value - vel.value * friction.value * world.delta_time();
	});


	world.system<Position, const Velocity>()
		.each([&](flecs::entity e, Position& pos, const Velocity& vel)
	{
		pos.value = pos.value + Vector3f{ vel.value } * world.delta_time();
	});


	world.system<Position, const ShiverAmount>()
		.each([&](flecs::entity e, Position& pos, const ShiverAmount& shiver)
	{
		pos.value = pos.value + 
			Vector3f{ rand_flt(-shiver.value, shiver.value), 
					  rand_flt(-shiver.value, shiver.value),
					  rand_flt(-shiver.value, shiver.value) };
	});
}
