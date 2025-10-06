#include <ecsPhys.h>
#include <ecsLogic.h>
#include <flecs.h>

using Vector3f = GameEngine::Math::Vector3f;

void RegisterEcsLogicSystems(flecs::world& world) {
	world.system<const Alive, Position>()
		.each([&](flecs::entity e, const Alive& alive, Position& pos) {

		if (!alive.isAlive) {
			pos.value = Vector3f{ 1000.0f, 0.0f, 1000.0f };
		}
	});

	world.system<TriggerTimer>()
		.each([&](TriggerTimer& timer) {

		if (timer.isActive) {
			timer.value -= world.delta_time();
			if (timer.value <= 0.0f) {
				timer.isActive = false;
			}
		}
	});

	world.system<const BouncePlane, Position, TriggerTimer, Alive>()
		.each([&](flecs::entity e, const BouncePlane& plane, Position& pos,
			TriggerTimer& timer, Alive& alive) {

		constexpr float planeEpsilon = 0.1f;
		if (pos.value * Vector3f{ plane.value.x, plane.value.y, plane.value.z } < plane.value.w + planeEpsilon) {
			timer.isActive = true;
		}

		if (timer.value <= 0.0f) {
			alive.isAlive = false;
			e.remove<TriggerTimer>();
		}
	});

	world.system<Patronage, TriggerTimer>()
		.each([&](Patronage& patronage, TriggerTimer& timer) {

		if (timer.value <= 0.0f) {
			timer.value = timer.restartValue;
			patronage.value = patronage.restartValue;
		}
		if (!patronage.value) {
			timer.isActive = true;
		}
		else {
			timer.isActive = false;
			timer.value = timer.restartValue;
		}
	});

	world.system<Position, TargetEntity>()
		.each([&](Position& pos, TargetEntity& targetEntity) {

		if (!targetEntity.target.is_valid()) return;

		const Alive* dCubeAlive = targetEntity.target.get<Alive>();
		if (!dCubeAlive) return;

		if (!(dCubeAlive->isAlive)) return;

		Vector3f dCubePos = targetEntity.target.get<Position>()->value;
		auto distance = [](Vector3f& pos1, Vector3f& pos2) {
			return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
				+ (pos1.y - pos2.y) * (pos1.y - pos2.y)
				+ (pos1.z - pos2.z) * (pos1.z - pos2.z));
			};
		constexpr float epsilon = 1.5f;
		if (distance(pos.value, dCubePos) < epsilon) {
			targetEntity.target.set<Alive>({ false });

			targetEntity.target.get_mut<TargetEntity>()->target.get_mut<Patronage>()->value += 10;
		}
	});

}