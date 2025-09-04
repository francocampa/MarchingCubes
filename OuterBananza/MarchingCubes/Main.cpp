#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "utils.h"
#include "Screen.h"
#include "TerrainGenerator.h"
#include "InteractiveMC.h"
#include "InputController.h"

void setupImGui(SDL_Window* window, SDL_GLContext context);
void setupLighting();

glm::ivec2 windowSize = { 1024,768 };
Camera camera = { {-10,10,-10}, pi / 4, pi / 1.5f };
Screen* screen = nullptr;

bool close = false;
int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error de inicializaci[on de SDL");
		exit(1);
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //	THE GOD LINE

	SDL_Window* win = SDL_CreateWindow("MarchingCubes",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowSize.x, windowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GLContext context = SDL_GL_CreateContext(win);

	if (glewInit() != GLEW_OK)
	{
		printf("Error de inicializaci[on de glew");
		return -1;
	}

	//glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);

	glClearColor(0.1, 0, 0.3f, 1.0f);
	gluPerspective(45, windowSize.x / (float)windowSize.y, 0.1, 100);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	setupImGui(win, context);
	setupLighting();

	Uint64 currentTick = SDL_GetPerformanceCounter();
	Uint64 lastTick = currentTick;
	//screen = new InteractiveMC();
	screen = new TerrainGenerator();
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		currentTick = SDL_GetPerformanceCounter();
		float delta = (currentTick - lastTick) / (float)SDL_GetPerformanceFrequency();
		lastTick = currentTick;

		InputController::procesFrameInputs(delta);
		if (screen != nullptr)
			screen->process(delta);

		SDL_GL_SwapWindow(win);
	} while (!InputController::close);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

void setupImGui(SDL_Window* window, SDL_GLContext context) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void setupLighting() {
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);
}