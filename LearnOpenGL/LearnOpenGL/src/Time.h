#pragma once

#include <GLFW/glfw3.h>

class Time {
public:
	static float deltaTime;

	static void Tick();

private:
	static float lastFrame;
};
