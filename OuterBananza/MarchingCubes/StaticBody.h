#pragma once

#include "utils.h"

class StaticBody
{
public:
	StaticBody(Mesh& mesh) {
		this->mesh = mesh;
		for (int i = 0; i < mesh.vertices.size(); i++)
		{
			minAABB = glm::min(minAABB, mesh.vertices[i].position);
			maxAABB = glm::max(maxAABB, mesh.vertices[i].position);
		}
	}

	Mesh mesh;
private:
	glm::vec3 minAABB = glm::vec3(1000,1000,1000);
	glm::vec3 maxAABB = -glm::vec3(1000, 1000, 1000);
};
