#pragma once
#include "Screen.h"
#include "utils.h"
#include "FastNoiseLite.h"
#include <string>
#include "InputController.h"
#include "MCTables.h"
#include "Player.h"
#include <SDL.h>
#include "unordered_map"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

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

class TerrainGenerator : public Screen
{
private:
	float distance = 0.5f;
	int base = 20;
	int height = 10;

	float threshold = -1.0f;
	int seed = 0;
	float noiseWeight = 1;
	float frequency = 0.1f;
	FastNoiseLite::NoiseType noiseType = FastNoiseLite::NoiseType_OpenSimplex2;
	FastNoiseLite noise;
	void generateTerrain();
	void generateChunk(glm::vec3 offset, Mesh&terrain);
	glm::vec3 getIndexPos(glm::vec3 offset, int i);
	float f(glm::vec3 pos);
	Chunk chunks[9];
	Mesh terrain;

	bool showSpheres = false;
	std::vector<Sphere> visualizationSpheres;
	bool showChunks = false;

	Player player;
	glm::vec3 centerChunk = {0,0,0};
	std::unordered_map<glm::ivec3, float,Vec3Hash> terrainModifications;
	std::vector<glm::vec3> getPointsInRadius(glm::vec3 pos, float radius);

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
public:
	TerrainGenerator();
	void process(float delta);
};

