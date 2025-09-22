#pragma once
#include "utils.h"
#include <string>
#include <reactphysics3d/reactphysics3d.h>
#include "InputController.h"
#include <SDL.h>

class TerrainGenerator;

using namespace reactphysics3d;

class Player
{
private:
	TerrainGenerator* terrain;

	Mesh model;

	float radius = 0.5;
	glm::vec3 pos = {7,2,7};

	glm::vec3 vel = { 0,0,0 };
	glm::vec3 maxVel = { 5,10,5 };

	Camera cam;
	glm::vec3 center;

	RigidBody* playerBody;

	void handleMovement(float delta);
	void handleCamera(float delta);
public:
	Player();
	void process(float delta);
	void draw();

	Camera getCamera();
	glm::vec3 getCenter();
	void useCamera();

	void setTerrain(TerrainGenerator* tg);

	void moveToPos(glm::vec3 pos);
	glm::vec3 getVel();
	glm::vec3 getPos();
	float getRadius();
	void setBody(RigidBody* body);
};
