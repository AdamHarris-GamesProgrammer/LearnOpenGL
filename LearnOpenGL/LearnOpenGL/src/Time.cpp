#include "Time.h"

float Time::deltaTime;
float Time::lastFrame;

void Time::Tick()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}
