#pragma once
#include "Screen.h"
#include "utils.h"
#include "FastNoiseLite.h"
#include <string>
#include "InputController.h"
#include "MCTables.h"
#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

struct Sphere {
	Mesh m;
	glm::vec3 pos;
	glm::vec3 color;
};

class TerrainGenerator : public Screen
{
private:
	float distance = 0.5f;
	int base = 20;
	int height = 10;

	float threshold = 0.3f;
	int seed = 0;
	float frequency = 0.1f;
	FastNoiseLite::NoiseType noiseType = FastNoiseLite::NoiseType_OpenSimplex2;
	FastNoiseLite noise;
	void generateTerrain();
	glm::vec3 getIndexPos(glm::vec3 offset, int i);
	float f(glm::vec3 pos);
	Mesh terrain;

	bool showSpheres = false;
	std::vector<Sphere> visualizationSpheres;

	Camera cam;
	glm::vec3 center;
	bool enabledLight = false;
	void handleCamera(float delta);
	void handleLight();
	void handleUI();

	void setNoiseParams();
public:
	TerrainGenerator();
	void process(float delta);
};

