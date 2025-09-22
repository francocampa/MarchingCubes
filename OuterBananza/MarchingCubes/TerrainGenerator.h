#pragma once
#include "Screen.h"
#include "utils.h"
#include "FastNoiseLite.h"
#include <string>
#include "InputController.h"
#include "MCTables.h"
#include <SDL.h>
#include "unordered_map"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <thread>
#include "Shader.h";
#include "Player.h";

class PhysicsEngine;

struct Sphere {
	Mesh m;
	glm::vec3 pos;
	glm::vec3 color;
};
struct Vec3Hash {
	std::size_t operator()(const glm::ivec3& v) const noexcept {
		std::size_t h1 = std::hash<int>{}(v.x);
		std::size_t h2 = std::hash<int>{}(v.y);
		std::size_t h3 = std::hash<int>{}(v.z);
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct EdgeKey {
	glm::ivec3 a, b;
	bool operator==(const EdgeKey& other) const {
		return a == other.a && b == other.b;
	}
};

struct EdgeKeyHash {
	size_t operator()(const EdgeKey& k) const {
		size_t h1 = std::hash<int>()(k.a.x) ^ (std::hash<int>()(k.a.y) << 1) ^ (std::hash<int>()(k.a.z) << 2);
		size_t h2 = std::hash<int>()(k.b.x) ^ (std::hash<int>()(k.b.y) << 1) ^ (std::hash<int>()(k.b.z) << 2);
		return h1 ^ (h2 << 1);
	}
};

class TerrainGenerator : public Screen
{
private:
	float distance = 2.0f;
	int base = 20;
	int height = 10;

	float threshold = -1.0f;
	int seed = 0;
	float noiseWeight = 10;
	float frequency = 0.1f;
	FastNoiseLite::NoiseType noiseType = FastNoiseLite::NoiseType_ValueCubic;
	FastNoiseLite noise;
	void generateTerrain();
	void generateChunk(glm::vec3 offset, Mesh&terrain);
	glm::vec3 getIndexPos(glm::vec3 offset, int i);
	float f(glm::vec3 pos);
	Chunk chunks[27];
	Mesh terrain;

	bool showSpheres = false;
	std::vector<Sphere> visualizationSpheres;
	bool showChunks = false;

	Player player;
	glm::vec3 centerChunk = {0,0,0};
	std::unordered_map<glm::ivec3, float, Vec3Hash> terrainModifications;
	std::vector<glm::vec3> getPointsInRadius(glm::vec3 pos, float radius);

	bool usePlayerCam = false;
	Camera cam= { {-5,5,-5},0,0}; //Por alguna raz[on rompe la luz las coordenadas polares
	glm::vec3 center;
	bool enabledLight = false;
	void handleCamera(float delta);
	void handleLight();
	void handlePlayer(float delta);
	void handleUI();
	void handleVisualizations();

	glm::vec3 getChunkForPos(glm::vec3 pos);
	void setNoiseParams();

	Shader terrainShader = Shader("shaders/terrain_color.glsl", "shaders/terrain_color_fragment.glsl");

	PhysicsEngine* physics;
public:
	TerrainGenerator();
	void process(float delta);
	Mesh* getTerrainMesh();
};

