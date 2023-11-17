#pragma once
#include "Properties.h"

	/*
		Contains the textures for
		each face of a block.
	*/

class BlockTexture {
public:
	// constructor takes a texture index for each face
	BlockTexture(unsigned int leftTextureIndex, unsigned int rightTextureIndex, unsigned int backTextureIndex,
		unsigned int frontTextureIndex, unsigned int upTextureIndex, unsigned int downTextureIndex) : 
		m_leftTextureIndex(leftTextureIndex), m_rightTextureIndex(leftTextureIndex), m_backTextureIndex(leftTextureIndex),
		m_frontTextureIndex(leftTextureIndex), m_upTextureIndex(upTextureIndex), m_downTextureIndex(downTextureIndex) {}

	// gets the texture for a certain face
	unsigned int getTextureIndex(Direction face);
protected:
	// stores the id of the texture
	unsigned int m_leftTextureIndex;
	unsigned int m_rightTextureIndex;
	unsigned int m_backTextureIndex;
	unsigned int m_frontTextureIndex;
	unsigned int m_upTextureIndex;
	unsigned int m_downTextureIndex;
};