#include <glm/glm.hpp>
#include <vector>;
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
	glm::vec2 texCoord;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

struct Chunk {
	Mesh mesh;
	glm::vec3 pos = { 0,0,0 };
};

void renderChunk(Chunk chunk);

void renderMesh(Mesh mesh, glm::vec3 pos, glm::vec3 color, float scale = 1);

Mesh createCubeMesh(float size);
Mesh createSphereMesh(float radius, int sectorCount, int stackCount);

void printVec3(glm::vec3 v);
