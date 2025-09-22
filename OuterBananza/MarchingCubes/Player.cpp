
#include "Player.h"
#include "TerrainGenerator.h"

void Player::handleMovement(float delta)
{
	float unitVel = 3.0f;
	glm::vec3 deltaVel = { 0,0,0 };
	glm::vec3 front = glm::vec3(0,0,1); //glm::normalize(cam.position - center);
	glm::vec3 right = glm::vec3(1, 0, 0); // glm::normalize(glm::cross(front, { 0,1,0 }));
	glm::vec3 up = glm::vec3(0,1,0);// glm::normalize(glm::cross(right, front));
	if (InputController::up) {
		deltaVel = - front;
	}
	if (InputController::down) {
		deltaVel =  front;
	}
	if (InputController::left) {
		deltaVel = - right;
	}
	if (InputController::right) {
		deltaVel =   right;
	}
	if (InputController::space) {
		//TODO: jump
	}

	if (deltaVel == glm::vec3(0, 0, 0))
		deltaVel = -vel;

	vel = glm::clamp(vel + deltaVel, -maxVel,maxVel);

	playerBody->setLinearVelocity(Vector3(vel.x, playerBody->getLinearVelocity().y, vel.z));
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
	model = createSphereMesh(radius,10,10);
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

Camera Player::getCamera()
{
    return cam;
}

glm::vec3 Player::getCenter()
{
    return center;
}

glm::vec3 Player::getPos()
{
	return pos;
}

float Player::getRadius()
{
	return radius;
}

void Player::setBody(RigidBody* body)
{
	this->playerBody = body;
}

glm::vec3 Player::getVel()
{
	return vel;
}

void Player::moveToPos(glm::vec3 pos) {
	this->pos = pos;
}


void Player::useCamera()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	gluLookAt(cam.position.x, cam.position.y, cam.position.z, center.x, center.y, center.z, 0, 1, 0);
}

void Player::setTerrain(TerrainGenerator* tg)
{
	terrain = tg;
}