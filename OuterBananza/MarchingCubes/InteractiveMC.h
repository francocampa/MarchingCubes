#pragma once
#include <string>
#include "Screen.h"
#include "InputController.h"
#include "utils.h"
#include "MCTables.h"
#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

class InteractiveMC: public Screen
{
private:
	Mesh balls[8];
	float values[8] = { 10,10,0,0,0,10,0,0 };
	glm::vec3 pos;
	Camera cam = { {-10,10,-10}, pi / 4, pi / 1.5f };
	glm::vec3 center;
	float separation = 4; 
	float threshold = 5;
	void handleCamera(float delta);
	float f(int index);
	glm::vec3 getIndexPos(int i);

	bool enabledLight = false;
	void handleLight();
	void handleUI();
public:
	void process(float delta);


};

