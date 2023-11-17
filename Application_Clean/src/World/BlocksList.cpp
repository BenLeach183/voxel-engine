#include "World/BlocksList.h"

// array of block info (sets textures for each face of a block type)
BlockTexture BlocksList::blocksInfoList[] = {
	BlockTexture(0, 0, 0, 0, 0, 0),	// Bedrock
	BlockTexture(1, 1, 1, 1, 1, 1),	// Stone
	BlockTexture(2, 2, 2, 2, 2, 2),	// Dirt
	BlockTexture(3, 3, 3, 3, 4, 2)	// Grass
};