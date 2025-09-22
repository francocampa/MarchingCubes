#include <glm/glm.hpp>
#include <vector>;
#include "Screen.h";
#include <GL/glew.h>
#include <GL/glu.h>

#pragma once
const double pi = 3.14159265358979323846;

struct Camera {
	glm::vec3 position;
	float theta;
	float phi;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

struct Chunk {
	Mesh mesh;
	glm::vec3 pos = { 0,0,0 };
};

struct Ray {
	glm::vec3 from;
	glm::vec3 dir;
};

struct RayHit {
	glm::vec3 point;
	glm::vec3 normal;
	float t;
	bool hit;
};

struct AABB {
	glm::vec3 min;
	glm::vec3 max;

	bool isInside(glm::vec3 point) {
		return (min.x < point.x && max.x > point.x) &&
			(min.y < point.y && max.y > point.y) &&
			(min.z < point.z && max.z > point.z);
	}

	bool rayHits(Ray r) {
		if (isInside(r.from))
			return true;

		glm::vec3 tVec0 = (min - r.from) / r.dir;
		glm::vec3 tVec1 = (max - r.from) / r.dir;

		glm::vec3 tMins = { std::min(tVec0.x,tVec1.x),std::min(tVec0.y,tVec1.y),std::min(tVec0.z,tVec1.z) };
		glm::vec3 tMaxs = { std::max(tVec0.x,tVec1.x),std::max(tVec0.y,tVec1.y),std::max(tVec0.z,tVec1.z) };

		float tEnter = std::max(std::max(tMins.x, tMins.y), tMins.z);
		float tExit = std::min(std::min(tMaxs.x, tMaxs.y), tMaxs.z);

		return tExit >= std::max(tEnter, 0.0f);
	}
};



void renderChunk(Chunk chunk);

void renderMesh(Mesh mesh, glm::vec3 pos, glm::vec3 color, float scale = 1);

Mesh createCubeMesh(float size);
Mesh createSphereMesh(float radius, int sectorCount, int stackCount);

void printVec3(glm::vec3 v);

extern Screen* screen;

extern glm::mat4 model;

extern glm::mat4 view;

extern glm::mat4 projection;

typedef unsigned int uint32;
#define inMine(a) ((uint32&) a)
bool checkPointInTriangle(const glm::vec3& point,
	const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc);

bool getLowestRoot(float a, float b, float c, float maxR,
	float* root);