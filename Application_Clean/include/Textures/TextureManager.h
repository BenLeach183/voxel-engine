#pragma once
#include "Textures/stb_image.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>

	/*
		A class for loading textures
		from file paths and creating
		textures with GLFW.

		Returns an id for the texture.
	*/

class TextureManager {
public:
	// loads a 2d texture and returns id for texture
	static unsigned int loadTexture(char const* path, bool smoothInterpolation = true);

	// loads a cubemap texture, takes an array of paths to 6 textures
	static unsigned int loadCubeMapTexture(std::string path[6]);
};