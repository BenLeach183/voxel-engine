#include "World/BlockTexture.h"

// function to return the value of a texture for a face
unsigned int BlockTexture::getTextureIndex(Direction face) {
	switch (face) {
	case Left:
		return m_leftTextureIndex;
		break;
	case Right:
		return m_rightTextureIndex;
		break;
	case Front:
		return m_frontTextureIndex;
		break;
	case Back:
		return m_backTextureIndex;
		break;
	case Up:
		return m_upTextureIndex;
		break;
	case Down:
		return m_downTextureIndex;
		break;
	}
}