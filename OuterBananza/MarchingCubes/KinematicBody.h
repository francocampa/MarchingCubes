#pragma once
#include <glm/glm.hpp>

class KinematicBody {
public:
	virtual void moveToPos(glm::vec3 pos) {}
	virtual glm::vec3 getVel() { return { 0,0,0 }; }
	virtual glm::vec3 getPos() { return { 0,0,0 }; }
	virtual glm::vec3 getERadius() { return { 1,1,1 }; }
};