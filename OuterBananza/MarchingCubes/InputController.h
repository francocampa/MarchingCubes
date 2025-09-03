#pragma once
#include <glm/glm.hpp>

class InputController
{
public:
	static void procesFrameInputs(float deltaTime);

	static bool up;
	static bool right;
	static bool down;
	static bool left;
	static bool space;
	static bool z;
	static bool x;
	static bool y;
	static bool s;
	static bool g;
	static bool r;
	static bool d;
	static bool shift;
	static bool del;
	static bool close;
	static float keyTimer;

	static bool windowResize;
	static glm::ivec2 mousePos;
	static glm::ivec2 mouseMove;
	static float CLICK_THRESHOLD;
	static float clickLeftTimer;
	static bool clickLeft;
	static bool mouseLeft;
	static float clickRightTimer;
	static bool clickRight;
	static bool mouseRight;
	static bool mouseWheel;
	static float mouseScroll;
};

