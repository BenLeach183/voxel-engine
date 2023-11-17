#include "World/Chunk.h"

// set the chunk coordinates
void Chunk::setPos(int x, int y) {
	m_chunkPos.x = x;
	m_chunkPos.y = y;
};

// returns a block with position within whole chunk
char Chunk::getBlock(int x, int y, int z) {
	// calculate the sub chunk block is in and the height within subchunk
	char subChunk = (char)(y / m_subChunkHeight);
	char localY = y - (subChunk * m_subChunkHeight);

	// return the block in the subchunk
	return getBlockInSubChunk(subChunk, x, localY, z);
}

// returns a block based on its subchunk and position within subchunk
char Chunk::getBlockInSubChunk(char subChunk, char x, char y, char z) {
	return m_solidBlockArray[subChunk][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)];
}

// fills the block vector with block data
void Chunk::generateChunkBlockData() {
	// set the blocks generated flag to true
	m_blocksGenerated = true;

	// calculate the global coordinates of the chunk for example
	// with chunk width of 16
	// chunk coordinate (2, 3) = global (32, 48)
	glm::vec2 chunkWorldPos = glm::vec2(m_chunkPos.x * m_chunkWidth, m_chunkPos.y * m_chunkWidth);

	// set frequency for sampling 2d perlin noise
	float frequency = 0.007f;

	// loop over x and z coords
	for (int x = 0; x < m_chunkWidth; x++)
	{
		for (int z = 0; z < m_chunkWidth; z++)
		{
			// fill the bit array with solid blocks
			populateSolidBlockArray(x, z, frequency, chunkWorldPos);
		}
	}
}

// generate the vertex data, must have existing block data
void Chunk::generateChunkVertexData() {
	// clear the vertex vector
	m_vertexData.clear();
	m_vertexData.shrink_to_fit();

	// clear the index data
	m_indices.clear();
	m_indices.shrink_to_fit();

	// reset the face count
	totalFaces = 0;

	// loop over x y z to fill the vertex and indices data
	for (int y = 0; y < m_chunkHeight; y++)
	{
		// calculate sub chunk block is in and the height of the block within sub chunk
		int subChunk = (int)(y / m_subChunkHeight);
		int localY = y % m_subChunkHeight;

		for (int x = 0; x < m_chunkWidth; x++)
		{
			for (int z = 0; z < m_chunkWidth; z++)
			{
				// get current block
				unsigned char currentBlock = getBlockInSubChunk(subChunk, x, localY, z);

				// if current block is air continue, no need to draw faces
				if (currentBlock == 0) continue;
	
				// else loop over the 6 directions
				for (char i = 0; i < 6; i++) {
					// cast the index to a direction
					Direction dir = static_cast<Direction>(i);

					// check if neighbouring block is solid or air
					if (isNeighbourAir(subChunk, x, localY, z, dir)) {
						// if it's air create a face in that direction
						createFace(currentBlock, dir, subChunk, x, localY, z);
					}
				}
			}
		}
	}
}

// function to check if a block in a direction from the current block is air
bool Chunk::isNeighbourAir(char subChunk, char x, char y, char z, Direction dir) {
	switch (dir) {
	case Direction::Left:
		// if the block is on the edge of the chunk
		if (x == 0) {
			// check if a neighbour exists, if not don't draw the face
			if (m_neighbouringChunks[0] == nullptr) return 0;
			// if neighbour does exist test if block in that chunk is air
			return m_neighbouringChunks[0]->getBlockInSubChunk(subChunk, m_chunkWidth - 1, y, z) == 0;
		}
		// else test for the left block within this chunk and see if it's air
		return getBlockInSubChunk(subChunk, x - 1, y, z) == 0;
	case Direction::Right:
		// test for neighbour chunk if on edge
		if (x == m_chunkWidth-1) {
			if (m_neighbouringChunks[1] == nullptr) return 0;
			return m_neighbouringChunks[1]->getBlockInSubChunk(subChunk, 0, y, z) == 0;
		}
		// else test for block in this chunk
		return getBlockInSubChunk(subChunk, x + 1, y, z) == 0;
	case Direction::Back:
		// test for neighbour chunk if on edge
		if (z == 0) {
			if (m_neighbouringChunks[2] == nullptr) return 0;
			return m_neighbouringChunks[2]->getBlockInSubChunk(subChunk, x, y, m_chunkWidth-1) == 0;
		}
		// else test for block in this chunk
		return getBlockInSubChunk(subChunk, x, y, z-1) == 0;
	case Direction::Front:
		// test for neighbour chunk if on edge
		if (z == m_chunkWidth-1) {
			if (m_neighbouringChunks[3] == nullptr) return 0;
			return m_neighbouringChunks[3]->getBlockInSubChunk(subChunk, x, y, 0) == 0;
		}
		// else test for block in this chunk
		return getBlockInSubChunk(subChunk, x, y, z + 1) == 0;
	case Direction::Down:
		// if block is at the bottom of the world draw a face
		if (subChunk == 0 && y == 0) return 1;
		// if its at the the bottom of a subchunk check the top block in subchunk below
		if (y == 0) return getBlockInSubChunk(subChunk - 1, x, m_subChunkHeight - 1, z) == 0;
		// else check block below current block in same subchunk
		return getBlockInSubChunk(subChunk, x, y - 1, z) == 0;
	case Direction::Up:
		// if block is at top of world draw the face
		if (subChunk == m_subChunkCount-1 && y == m_subChunkHeight-1) return 1;
		// if block is at top of subchunk check block at bottom of subchunk above
		if (y == m_subChunkHeight-1) return getBlockInSubChunk(subChunk + 1, x, 0, z) == 0;
		// else check block above current block in same subchunk
		return getBlockInSubChunk(subChunk, x, y + 1, z) == 0;
	default:
		return false;
	}
}

void Chunk::createFace(unsigned char blockType, Direction face, char subChunk, char x, char y, char z) {
	// keep track of the offset to add to the indices
	unsigned int indexOffset = totalFaces * 4;
	// the 6 directions can be saved in 3 bits and unpacked in the shader
	char normal = (char)face;
	// gets an index for the texture based on block and the face
	unsigned char texIndex = BlocksList::blocksInfoList[blockType-1].getTextureIndex(face);

	// This is how data is packed into one unsigned int vertex attribute
	// 
	// uv coords     =	2 bits		= 0 offset
	// texture index =	8 bits		= 2 offset
	// normal        =	3 bits		= 10 offset
	// pos x, y, z   =	3 * 5 bits	= 13, 18, 23 offset
	// subchunk      =	4 bits		= 28 offset

	//
	//  normal
	//	0 = -1.0f, 0.0f, 0.0f
	//	1 = 1.0f, 0.0f, 0.0f
	//	2 = 0.0f, 0.0f, -1.0f
	//	3 = 0.0f, 0.0f, 1.0f
	//	4 = 0.0f, 1.0f, 0.0f
	//	5 = 0.0f, -1.0f, 0.0f
	//
	//	vertex offset
	//	0 = -0.5
	//	1 = +0.5
	// 

	// create temp vertice vector to store the 4 vertices of the face
	std::vector<unsigned int> vertices(4);

	// loop over vertices
	for (int i = 0; i < 4; i++) {
		// coordinates for the individual vertex
		int vertexX, vertexY, vertexZ;
		// depending on the face direction set the coordinates of face
		switch (face) {
		case Left:
			vertexX = x;								// This function keeps the axis of the face
			vertexY = y + ((i < 2) ? 1 : 0);			// constant, for example x for the left face
			vertexZ = z + ((i % 2 == 0) ? 1 : 0);		// and will alternate the other two coordinates
			break;										// by using < 2 and % 2, meaning all combinations
		case Right:										// of y and z are covered in this example.
			vertexX = x + 1;
			vertexY = y + ((i < 2) ? 1 : 0);
			vertexZ = z + ((i % 2 == 0) ? 0 : 1);
			break;
		case Back:
			vertexX = x + ((i % 2 == 0) ? 0 : 1);
			vertexY = y + ((i < 2) ? 1 : 0);
			vertexZ = z;
			break;
		case Front:
			vertexX = x + ((i % 2 == 0) ? 1 : 0);
			vertexY = y + ((i < 2) ? 1 : 0);
			vertexZ = z + 1;
			break;
		case Down:
			vertexX = x + ((i % 2 == 0) ? 1 : 0);
			vertexY = y;
			vertexZ = z + ((i < 2) ? 1 : 0);
			break;
		case Up:
			vertexX = x + ((i % 2 == 0) ? 0 : 1);
			vertexY = y + 1;
			vertexZ = z + ((i < 2) ? 1 : 0);
			break;
		default:
			std::cout << "Error creating face vertex" << std::endl;
			break;
		}

		// packs all the data for the vertex into one unsigned int
		vertices[i] = (subChunk << 28) | (vertexX << 23) | (vertexY << 18) | (vertexZ << 13) | (normal << 10) | (texIndex << 2) | i;
	}

	// appends the 4 vertices onto the end of the vertex vector
	m_vertexData.insert(m_vertexData.end(), vertices.begin(), vertices.end());
	// appends the indices, with an added offset
	m_indices.insert(m_indices.end(), { indexOffset + 2, indexOffset + 1, indexOffset + 3, indexOffset + 1, indexOffset + 2, indexOffset + 0});
	// increment the number of faces calculated
	totalFaces++;
}

// This performs the world generation
// TODO - Create world gen class
void Chunk::populateSolidBlockArray(int x, int z, float frequency, glm::vec2 chunkWorldPos) {
	// Set size of vectors and default to air
	for (int i = 0; i < 16; i++) { m_solidBlockArray[i].resize(4096, 0); }

	// calculate the height at coordinate
	int height = (int)(m_noise.perlin2d((float)(chunkWorldPos.x + x + 2000) * frequency, (float)(chunkWorldPos.y + z + 4000) * frequency) * 30.0) + 70;

	// calculate sub chunk that the height is in
	int subChunkOfHeight = (int)(height / m_subChunkHeight);
	int heightInSubChunk = height % m_subChunkHeight;

	// set all the blocks in x z up to height as solid
	// loop over subchunks, fill from y=0 to y=256 as its more efficient
	for (int i = 0; i < m_subChunkCount; i++)
	{
		// if sub chunk is the first (lowest)
		if (i == 0) {
			// set bottom level to block id 1
			m_solidBlockArray[i][(x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 1;
		
			// loop through height in sub chunk
			for (char y = 1; y < m_subChunkHeight; y++)
			{
				// sets the block in array to air or stone, stone id = 2
				m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 2 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
			}
			continue;
		}

		// if sub chunk is below height, set all the blocks in the x z column to solid
		if (i < subChunkOfHeight) {
			for (char y = 0; y < m_subChunkHeight; y++)
			{
				// if the current y value is the top block set it to grass, grass id = 4
				if (y + (i * 16) == height - 1) {
					m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 4 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
					continue;
				}
				// if block is 2 below height set it to dirt, dirt id = 3
				if (y + (i * 16) >= (height - 3)) {
					m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 3 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
					continue;
				}
				// else set the block to stone, stone id = 2
				m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 2 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
			}
			continue;
		}
		// if it is sub chunk height set solid blocks up to height as solid
		if (i == subChunkOfHeight) {
			for (char y = 0; y < m_subChunkHeight; y++)
			{
				if (y < heightInSubChunk) {
					// if the current y value is the top block set it to grass, grass id = 4
					if (y + (i * 16) == height-1) {
						m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 4 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
						continue;
					}

					// if block is 2 below height set it to dirt, dirt id = 3
					if (y + (i * 16) >= (height - 3)) {
						m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 3 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
						continue;
					}

					// else set the block to stone, stone id = 2
					m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 2 * worldGen3DBlockExists(chunkWorldPos, i, x, y, z);
				}
				else {
					// if y is above the top block set block to air
					m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 0;
				}
			}
			continue;
		}

		// else sub chunk is above height so set all blocks to air
		for (char y = 0; y < m_subChunkHeight; y++)
		{
			// air id = 0
			m_solidBlockArray[i][y + (x * m_chunkWidth) + (z * m_chunkWidth * m_chunkWidth)] = 0;
		}
	}
}

// Runs 3d perlin noise and returns if a block should or shouldnt exist
bool Chunk::worldGen3DBlockExists(glm::vec2 chunkWorldPos, int subChunk, char x, char y, char z) {
	// takes the local xyz and converts to global
	return (m_noise.perlin3d((float)(chunkWorldPos.x + x + 2000) * 0.05f, (float)(subChunk * m_subChunkHeight + y) * 0.05f, (float)(chunkWorldPos.y + z + 4000) * 0.05f) < 0.5f);
}

// set the pointer array to neighbour chunks
void Chunk::setNeighbour(Chunk* chunk, Direction dir) {
	unsigned char dir_id = static_cast<unsigned char>(dir);

	// if direction isn't left, right, back or front return nothing
	if (dir_id > 3 || dir_id < 0) return;

	// else set the chunk pointer
	m_neighbouringChunks[dir_id] = chunk;
}

// return reference to neighbour in a direction
Chunk* Chunk::getNeighbour(Direction dir) {
	unsigned char dir_id = static_cast<unsigned char>(dir);

	// if direction isn't left, right, back or front return nullptr
	if (dir_id > 3 || dir_id < 0) return nullptr;

	// else return chunk pointer
	return m_neighbouringChunks[dir_id];
}

// update the vbo and ebo
void Chunk::updateBuffers() {
	// delete existing buffers
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

	// generate new buffers
	glCreateBuffers(1, &m_VBO);
	glCreateBuffers(1, &m_EBO);

	// allocate storage for new buffers
	// load the vertex data into the vbo for the chunk
	glNamedBufferStorage(m_VBO, sizeof(unsigned int) * m_vertexData.size(), m_vertexData.data(), GL_DYNAMIC_STORAGE_BIT);

	// load the index data into the ebo for the chunk
	glNamedBufferStorage(m_EBO, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_DYNAMIC_STORAGE_BIT);

	// set chunk to visibile
	m_chunkVisible = true;
}

// take the players chunk coordinates and calculate the squared distance to this chunk
void Chunk::setDistanceFromPlayer(int playerChunkX, int playerChunkY) {
	m_distanceSquared = ((m_chunkPos.x - playerChunkX) * (m_chunkPos.x - playerChunkX)) + ((m_chunkPos.y - playerChunkY) * (m_chunkPos.y - playerChunkY));
}