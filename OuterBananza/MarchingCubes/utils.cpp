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

Mesh createCubeMesh(float size) {
	Mesh cube;
	float h = size / 2.0f;

	glm::vec3 positions[] = {
		{-h, -h, -h}, { h, -h, -h}, { h,  h, -h}, {-h,  h, -h}, // back
		{-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h}  // front
	};

	int faceIndices[][6] = {
		{0, 1, 2, 2, 3, 0}, // back
		{4, 5, 6, 6, 7, 4}, // front
		{0, 4, 7, 7, 3, 0}, // left
		{1, 5, 6, 6, 2, 1}, // right
		{3, 2, 6, 6, 7, 3}, // top
		{0, 1, 5, 5, 4, 0}  // bottom
	};

	glm::vec3 normals[] = {
		{ 0,  0, -1},  // back
		{ 0,  0,  1},  // front
		{-1,  0,  0},  // left
		{ 1,  0,  0},  // right
		{ 0,  1,  0},  // top
		{ 0, -1,  0}   // bottom
	};

	for (int f = 0; f < 6; ++f) {
		for (int i = 0; i < 6; ++i) {
			Vertex v;
			v.position = positions[faceIndices[f][i]];
			v.normal = normals[f];
			v.texCoord = { 0.0f, 0.0f };
			cube.vertices.push_back(v);
			cube.indices.push_back(static_cast<unsigned int>(cube.indices.size()));
		}
	}

	return cube;
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