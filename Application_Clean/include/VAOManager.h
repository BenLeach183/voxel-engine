#pragma once
#include <vector>
#include <glad/glad.h>

#include "Shader.h"
#include "World/Chunk.h"
#include "Objects/StandardObject.h"

	/*
		Stores the VAO's for the different mesh types
		Also contains the functions to draw the
		objects.
	*/

class VAOManager {
public:
	void initializeChunkArray(std::vector<Chunk>* chunkArray);
	void initializeCubeArray(std::vector<StandardObject*>* stdObjectsArray);

	// functions to create the VAO's
	void makeChunkVAO();
	void makeStdVAO();
	void makeCubeMapVAO();

	// functions to draw the mesh's
	void drawChunks(Shader* shader, unsigned int texture);
	void drawCubeMap(Shader* shader, unsigned int texture);
	// std objects contain their own texture
	void drawStdObjects(Shader* shader);

	// toggles normal maps on and off
	void toggleNormalMaps();
private:
	// reference to the chunk vector and render width in the chunk manager
	std::vector<Chunk>* m_chunks;
	unsigned char* m_chunkRenderWidth;

	// reference to the std objects in the scene
	std::vector<StandardObject*>* m_standardObjects;

	// vaos for chunks, std objects and the sky cubemap
	unsigned int m_chunkVAO, m_stdVAO, m_cubeMapVAO;

	// vbos and ebos for standard objects
	// TODO move VBO and EBO into the standard object class
	std::vector<unsigned int> m_stdVBOs, m_stdEBOs;

	// VBO and EBO for the cube map
	// TODO create cubemap class
	// TODO move into cubemap class
	unsigned int m_cubeMapVBO, m_cubeMapEBO;

	// stores whether normal maps are enabled
	bool m_normalMapsEnabled = true;
};