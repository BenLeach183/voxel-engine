#include "Textures/TextureManager.h"
#include <iostream>

unsigned int TextureManager::loadTexture(char const* path, bool smoothInterpolation)
{
	// create variable to store id of texture
	unsigned int textureID = 0;

	// load texture file and get reference to width height and number of channels
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	// if texture data is read correctly
	if (data)
	{
		// set the format based on number of channels
		GLenum format;
		if (nrComponents == 1)     format = GL_RED;
		else if (nrComponents == 3)format = GL_RGB;
		else if (nrComponents == 4)format = GL_RGBA;

		// create a 2d texture and give it reference to id variable
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 4, GL_RGBA8, width, height);
		glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(textureID);

		// bind the texture to GL_TEXTURE_2D
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set paramaters
		// set the wrapping mode to repeat
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT); // S = x axis 
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT); // T = y axis, R if 3D

		// if smooth interpolation enabled use linear interpolation
		if (smoothInterpolation) {
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		// else use nearest neighbour
		else {
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		// free the data from the stbi_image library
		stbi_image_free(data);
		std::cout << "Loaded texture at path: " << path << " components " << nrComponents << " id " << textureID << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		// free the data from the stbi_image library
		stbi_image_free(data);
	}

	// return the texture id
	return textureID;
}

unsigned int TextureManager::loadCubeMapTexture(std::string path[6]) {
	// create variable to store id of texture
	unsigned int textureID = 0;

	// create a cube map texture and give it reference to id variable
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// set the paramaters, this time in 3d, and clamp to edge instead of repeat
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // S = x axis 
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // T = y axis
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // R = z axis
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// loop through all 6 textures
	for (unsigned int i = 0; i < 6; i++) {
		// load texture file and get reference to width height and number of channels
		int width, height, nrComponents;
		unsigned char* data = stbi_load(path[i].c_str(), &width, &height, &nrComponents, 0);

		// set the format based on number of channels
		GLenum format;
		if (nrComponents == 1)     format = GL_RED;
		else if (nrComponents == 3)format = GL_RGB;
		else if (nrComponents == 4)format = GL_RGBA;

		// if texture data is read correctly
		if (data) {
			// flip the textures as cubemap reads the texture from top down
			stbi_set_flip_vertically_on_load(false);

			// set the texture data
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,		// GL_TEXTURE_CUBE_MAP_POSITIVE_X is the first, 
				0,										// by adding i it loops through all directions
				format,
				width,
				height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data
			);

			// free the data from the stbi_image library
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			// free the data from the stbi_image library
			stbi_image_free(data);
		}
	}

	// return the texture id
	return textureID;
}