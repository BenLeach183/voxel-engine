#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "World/Noise.h"
#include "World/BlocksList.h"
#include "Properties.h"

/*
	Stores chunk data and calculates 
	the mesh data for the chunk.
*/

class Chunk {
public:
	// ----------------------------SETTERS------------------------------------
	// set the position of the chunk (won't render until updateThreadToRenderProperties() is called)
	void setPos(int x, int y);

	// calculates the squared distance from the chunk players in
	void setDistanceFromPlayer(int playerChunkX, int playerChunkY);

	// sets properties for chunk generation
	void setVisible(bool visible) { m_chunkVisible = visible; }
	void setAssignedPos(bool assignedPos) { m_assignedPos = assignedPos; }
	void setBlocksGenerated(bool blocksGenerated) { m_blocksGenerated = blocksGenerated; }

	// set reference to neighbour chunk in a direction
	void setNeighbour(Chunk* chunk, Direction dir);

	// -----------------------------------------------------------------------

	// ----------------------------GETTERS------------------------------------
	// get position of chunk, used for calculations on thread
	int getPosX() { return m_chunkPos.x; }
	int getPosY() { return m_chunkPos.y; }
	// get render position, used for main thread
	int getRenderPosX() { return m_renderChunkPos.x; }
	int getRenderPosY() { return m_renderChunkPos.y; }
	
	// get the distance of chunk from the chunk player is in
	unsigned int getDistanceFromPlayer() const { return m_distanceSquared; }

	// get properties for chunk generation
	bool getVisible() { return m_chunkVisible; }
	bool getAssignedPos() { return m_assignedPos; }
	bool getBlocksGenerated() { return m_blocksGenerated; }

	// get reference to neighbouring chunk in a direction
	Chunk* getNeighbour(Direction dir);

	// get block in chunk
	char getBlock(int x, int y, int z);
	// get block in subchunk
	char getBlockInSubChunk(char subChunk, char x, char y, char z);

	// get the vertex and index data
	std::vector<unsigned int>* getVertexData() { return &m_vertexData; }
	std::vector<unsigned int>* getIndices() { return &m_indices; }
	unsigned int getVBO() { return m_VBO; }
	unsigned int getEBO() { return m_EBO; }

	// -----------------------------------------------------------------------

	// copy the calculation properties to render properties
	void updateThreadToRenderProperties() { m_renderChunkPos = m_chunkPos; }

	// generate the blocks in chunk
	void generateChunkBlockData();
	// generate the vertex & index data
	void generateChunkVertexData();

	// update VBO and EBO
	void updateBuffers();
protected:
	// chunk position
	glm::vec2 m_chunkPos;
	glm::vec2 m_renderChunkPos;

	// squared distance to players chunk
	unsigned int m_distanceSquared;

	// properties for chunk generation
	bool m_chunkVisible = false;
	bool m_assignedPos = false;
	bool m_blocksGenerated = false;

	// vertex, index and buffers
	std::vector<unsigned int> m_vertexData, m_indices;
	unsigned int m_VBO, m_EBO;

	// chunk + subchunk size
	unsigned char m_chunkWidth = 16;
	unsigned int m_chunkHeight = 256;
	unsigned char m_subChunkHeight = 16;
	unsigned char m_subChunkCount = (unsigned char)(m_chunkHeight / m_subChunkHeight);

	// noise class for perlin noise
	Noise m_noise;

	// array of blocks in chunk
	// in order to access the data quicker it is stored in 16x16x16 sub chunks, there are 16 sub chunks per chunk
	std::vector<unsigned char> m_solidBlockArray[16];

	Chunk* m_neighbouringChunks[4] = { nullptr, nullptr, nullptr, nullptr }; // LEFT, RIGHT, BACK, FRONT

	// enum for directions Left, Right, Back, Front, Up, Down
	Direction direction;

	// keeps track of the total number of faces calculated so far
	int totalFaces = 0;

	// tests if a block in a chunk is air (for face culling at chunk borders)
	bool isNeighbourAir(char subChunk, char x, char y, char z, Direction dir);

	// creates a face at a block position in a direction
	void createFace(unsigned char blockType, Direction face, char subChunk, char x, char y, char z);

	// runs the world generation code to fill block array
	void populateSolidBlockArray(int x, int z, float frequency, glm::vec2 chunkWorldPos);

	// run 3d perlin noise world gen to see if a block is air or not
	// TODO make world gen class
	bool worldGen3DBlockExists(glm::vec2 chunkWorldPos, int subChunk, char x, char y, char z);
};
