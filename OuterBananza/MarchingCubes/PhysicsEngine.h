#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "utils.h"

class Player;

using namespace reactphysics3d;

class PhysicsEngine
{
public:
	PhysicsEngine();
	float timer = 0.0f;
	void processPhysics(float delta);

	void addTerrain(Mesh m);
	void setPlayer(Player* player);
private:
	const float fps = 60;
	const float timeStep = 1.0f / fps;

	PhysicsCommon physicsCommon;
	PhysicsWorld::WorldSettings settings;
	PhysicsWorld* world;

	Player* player;
	RigidBody* playerBody;
	std::vector<RigidBody*> terrainBodies;

	const glm::vec3 gravity = {0,-9.81f,0};
	const float unitsPerMeter = 100.0f;

};
