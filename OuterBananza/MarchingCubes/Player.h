#pragma once
#include "utils.h"
#include <string>
#include "InputController.h"
#include <SDL.h>

class Player
{
private:
	glm::vec3 pos;

	Camera cam;
	glm::vec3 center;

	void handleMovement();
	void handleCamera();
public:
	void process(float delta);

	Camera* getCamera();
	glm::vec3 getCenter();
};
