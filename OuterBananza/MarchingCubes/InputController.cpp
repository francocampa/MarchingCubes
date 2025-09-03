#include "InputController.h"
#include <SDL.h>
#include "backends/imgui_impl_sdl2.h"
#include <SDL_opengl.h>
#include <GL/glu.h>

bool InputController::up = false;
bool InputController::right = false;
bool InputController::left = false;
bool InputController::down = false;
bool InputController::space = false;
bool InputController::x = false;
bool InputController::y = false;
bool InputController::z = false;
bool InputController::s = false;
bool InputController::g = false;
bool InputController::r = false;
bool InputController::del = false;
bool InputController::shift = false;
bool InputController::d = false;
bool InputController::close = false;
bool InputController::windowResize = false;

glm::ivec2 InputController::mousePos = { 0,0 };
glm::ivec2 InputController::mouseMove = {0,0};
bool InputController::clickLeft = false;
bool InputController::mouseLeft = false;
bool InputController::clickRight = false;
bool InputController::mouseRight = false;
bool InputController::mouseWheel = false;
float InputController::mouseScroll = 0.0f;
float InputController::clickLeftTimer = 0.0f;
float InputController::clickRightTimer = 0.0f;
float InputController::keyTimer = 0.0f;
float InputController::CLICK_THRESHOLD = 0.3f;


void InputController::procesFrameInputs(float deltaTime)
{
	clickLeftTimer += deltaTime;
	clickRightTimer += deltaTime;
	keyTimer += deltaTime;
	clickLeft = false;
	clickRight = false;
	g = false;
	s = false;
	r = false;
	x = false;
	z = false;
	y = false;
	d = false;
	del = false;
	mouseMove.x = 0;;
	mouseMove.y = 0;;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			close = true;
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				close = true;
				break;
			case SDLK_UP:
				up = false;
				break;
			case SDLK_DOWN:
				down = false;
				break;
			case SDLK_s:
				s = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_LEFT:
				left = false;
				break;
			case SDLK_RIGHT:
				right = false;
				break;
			case SDLK_x:
				x = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_z:
				z = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_d:
				d = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_y:
				y = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_SPACE:
				space = false;
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				shift = false;
				break;
			case SDLK_DELETE:
				del = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_TAB:
				break;
			case SDLK_v:
				break;
			case SDLK_r:
				r = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			case SDLK_g:
				g = keyTimer < CLICK_THRESHOLD;
				keyTimer = 0.0f;
				break;
			}
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				close = true;
				break;
			case SDLK_UP:
				up = true;
				break;
			case SDLK_DOWN:
				down = true;
				break;
			case SDLK_s:
				keyTimer = 0.0f;
				break;
			case SDLK_LEFT:
				left = true;
				break;
			case SDLK_RIGHT:
				right = true;
				break;
			case SDLK_x:
				keyTimer = 0.0f;
				break;
			case SDLK_z:
				keyTimer = 0.0f;
				break;
			case SDLK_y:
				keyTimer = 0.0f;
				break;
			case SDLK_d:
				keyTimer = 0.0f;
				break;
			case SDLK_SPACE:
				space = true;
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				shift = true;
				break;
			case SDLK_r:
				keyTimer = 0.0f;
				break;
			case SDLK_g:
				keyTimer = 0.0f;
				break;
			case SDLK_DELETE:
				keyTimer = 0.0f;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				clickLeftTimer = 0.0f;
				mouseLeft = true;
				break;
			case SDL_BUTTON_RIGHT:
				clickRightTimer = 0.0f;
				mouseRight = true;
				break;
			case SDL_BUTTON_MIDDLE:
				mouseWheel = true;
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				clickLeft = clickLeftTimer < CLICK_THRESHOLD;
				clickLeftTimer = 0.0f;
				mouseLeft = false;
				break;
			case SDL_BUTTON_RIGHT:
				clickRight = clickRightTimer < CLICK_THRESHOLD;
				clickRightTimer = 0.0f;
				mouseRight = false;
				break;
			case SDL_BUTTON_MIDDLE:
				mouseWheel = false;
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			mouseMove = { event.motion.xrel,event.motion.yrel };
			mousePos = { event.motion.x,event.motion.y };
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event != SDL_WINDOWEVENT_RESIZED)
				break;
			windowResize = true;
			break;
		}
	}
}
