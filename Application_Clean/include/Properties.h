#pragma once
#include "glm/glm.hpp"
#include <iostream>

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
static glm::vec3 WORLD_UP = glm::vec3(0.0, 1.0, 0.0);

// enum for the 6 directions
static enum Direction { Left, Right, Back, Front, Up, Down };

// if the window size is changed update the properties
static void updateWindowSize(int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}