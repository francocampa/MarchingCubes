#pragma once
#include "utils.h"
#include <string>
#include "InputController.h"
#include <SDL.h>

class Player
{
private:
	Mesh model;

	glm::vec3 pos = {5,2,5};

	Camera cam;
	glm::vec3 center;

	void handleMovement(float delta);
	void handleCamera(float delta);
public:
	Player();
	void process(float delta);
	void draw();

	Camera* getCamera();
	glm::vec3 getCenter();
	glm::vec3 getPos();
	void useCamera();
};
