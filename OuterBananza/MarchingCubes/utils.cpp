#include "utils.h"

void renderChunk(Chunk chunk)
{
	glPushMatrix();
	glTranslatef(chunk.pos.x, chunk.pos.y, chunk.pos.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	const Vertex* vertexDataPtr = chunk.mesh.vertices.data();
	const GLuint* indexData = chunk.mesh.indices.data();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)((char*)vertexDataPtr + offsetof(Vertex, position)));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)((char*)vertexDataPtr + offsetof(Vertex, normal)));
	//glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*)((char*)vertexDataPtr + offsetof(Vertex, color)));

	glDrawElements(GL_TRIANGLES, chunk.mesh.indices.size(), GL_UNSIGNED_INT, indexData);


	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

Mesh createSphereMesh(float radius, int sectorCount, int stackCount) {
	Mesh sphere;
	for (int i = 0; i <= stackCount; ++i) {
		float stackAngle = pi / 2 - i * pi / stackCount;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * 2 * pi / sectorCount;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			glm::vec3 position = { x, y, z };
			glm::vec3 normal = glm::normalize(position);

			Vertex v;
			v.position = position;
			v.normal = normal;
			v.texCoord = { (float)j / sectorCount, (float)i / stackCount };
			sphere.vertices.push_back(v);
		}
	}

	for (int i = 0; i < stackCount; ++i) {
		for (int j = 0; j < sectorCount; ++j) {
			int k1 = i * (sectorCount + 1) + j;
			int k2 = k1 + sectorCount + 1;

			sphere.indices.push_back(k1);
			sphere.indices.push_back(k2);
			sphere.indices.push_back(k1 + 1);

			sphere.indices.push_back(k1 + 1);
			sphere.indices.push_back(k2);
			sphere.indices.push_back(k2 + 1);
		}
	}

	return sphere;
}

void printVec3(glm::vec3 v)
{
	printf("%f,%f,%f\n",v.x,v.y,v.z);
}

void renderMesh(Mesh mesh, glm::vec3 pos, glm::vec3 color, float scale )
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(scale,scale,scale);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glColor4f(color.r, color.g, color.b, 0.4f);

	const Vertex* vertexDataPtr = mesh.vertices.data();
	const GLuint* indexData = mesh.indices.data();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)((char*)vertexDataPtr + offsetof(Vertex, position)));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)((char*)vertexDataPtr + offsetof(Vertex, normal)));

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, indexData);


	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}