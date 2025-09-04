
#include "Player.h"

void Player::handleMovement(float delta)
{
	float vel = 10.0f;

	glm::vec3 front = glm::vec3(0,0,1); //glm::normalize(cam.position - center);
	glm::vec3 right = glm::vec3(1, 0, 0); // glm::normalize(glm::cross(front, { 0,1,0 }));
	glm::vec3 up = glm::vec3(0,1,0);// glm::normalize(glm::cross(right, front));
	if (InputController::up) {
		pos = pos - vel * delta * front;
	}
	if (InputController::down) {
		pos = pos + vel * delta * front;
	}
	if (InputController::left) {
		pos = pos - vel * delta * right;
	}
	if (InputController::right) {
		pos = pos + vel * delta * right;
	}
	if (InputController::space) {
		pos = pos + vel * delta * up;
	}
	if (InputController::shift) {
		pos = pos - vel * delta * up;
	}
}

void Player::handleCamera(float delta)
{
	float sensitivity = 0.5f;
	float theta = cam.theta + sensitivity * delta * InputController::mouseMove.x;
	float phi = cam.phi + sensitivity * sin(delta * InputController::mouseMove.y);

	float epsilon = 0.5f;
	if (phi < epsilon) phi = epsilon;
	if (phi > M_PI - epsilon) phi = M_PI - epsilon;

	cam.theta = theta;
	cam.phi = phi;
}

Player::Player()
{
	model = createSphereMesh(0.5,10,10);
}

void Player::process(float delta)
{
    handleCamera(delta);
    handleMovement(delta);
	draw();
}

void Player::draw()
{
	renderMesh(model,pos,glm::vec3(0.7f,0.7f,0.7f),1);
}

Camera* Player::getCamera()
{
    return &cam;
}

glm::vec3 Player::getCenter()
{
    return center;
}

glm::vec3 Player::getPos()
{
	return pos;
}

void Player::useCamera()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	gluLookAt(cam.position.x, cam.position.y, cam.position.z, center.x, center.y, center.z, 0, 1, 0);
}
