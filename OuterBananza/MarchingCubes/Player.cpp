
#include "Player.h"

void Player::handleMovement()
{
	glm::vec3 front = glm::normalize(cam.position - center);
	glm::vec3 right = glm::normalize(glm::cross(front, { 0,1,0 }));
	if (InputController::up) {
		cam.position = cam.position - vel * delta * front;
	}
	if (InputController::down) {
		cam.position = cam.position + vel * delta * front;
	}
	if (InputController::right) {
		cam.position = cam.position - vel * delta * right;
	}
	if (InputController::left) {
		cam.position = cam.position + vel * delta * right;
	}
}

void Player::handleCamera()
{
}

void Player::process(float delta)
{
    handleCamera();

    handleMovement();
}

Camera* Player::getCamera()
{
    return &cam;
}

glm::vec3 Player::getCenter()
{
    return center;
}
