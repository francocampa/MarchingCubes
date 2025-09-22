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

bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10,e10);
	float b = glm::dot(e10,e20);
	float c = glm::dot(e20,e20);
	float ac_bb = (a * c) - (b * b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp,e10);
	float e = glm::dot(vp,e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	//return false;
	return ((inMine(z) & ~(inMine(x) | inMine(y))) & 0x80000000);

}
bool getLowestRoot(float a, float b, float c, float maxR, float* root)
{
	// Check if a solution exists
	float determinant = b * b - 4.0f * a * c;
	// If determinant is negative it means no solutions.
	if (determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	// Sort so x1 <= x2
	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
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


Screen* screen = nullptr;


glm::mat4 model = glm::mat4(1.0f);

glm::mat4 view = glm::mat4(1.0f);

glm::mat4 projection = glm::mat4(1.0f);
