#include "World/Noise.h"

Noise::Noise() {
	// double the permutation array for overflow
	for (int i = 0; i < 512; i++)
	{
		this->m_doublePermutation[i] = this->m_permutation[i % 256];
	}
}

float Noise::perlin2d(float x, float y) {
	// find the unit grid the point lies in
	int unitX = (int)x % 255;
	int unitY = (int)y % 255;

	// find the coordinates of the point within the grid
	float localX = x - unitX;
	float localY = y - unitY;

	// find the vector from each corner of the unit grid to the point
	glm::vec2 topRight = glm::vec2(localX - 1.0, localY - 1.0);
	glm::vec2 topLeft = glm::vec2(localX, localY - 1.0);
	glm::vec2 bottomRight = glm::vec2(localX - 1.0, localY);
	glm::vec2 bottomLeft = glm::vec2(localX, localY);

	// select pseudo random value for each corner in grid
	int tr, tl, br, bl;
	tr = m_doublePermutation[m_doublePermutation[unitX + 1] + unitY + 1];
	tl = m_doublePermutation[m_doublePermutation[unitX] + unitY + 1];
	br = m_doublePermutation[m_doublePermutation[unitX + 1] + unitY]; 
	bl = m_doublePermutation[m_doublePermutation[unitX] + unitY];

	// dot product the distance vector with the gradient vector
	float dotTopRight = glm::dot(topRight, grad2d(tr));
	float dotTopLeft = glm::dot(topLeft, grad2d(tl));
	float dotBottomRight = glm::dot(bottomRight, grad2d(br));
	float dotBottomLeft = glm::dot(bottomLeft, grad2d(bl));

	// fade the local values before lerp to smooth the transition
	float u = fade(localX);
	float v = fade(localY);

	// interpolate each value to find the value for the local coordinate
	return lerp(u, lerp(v, dotBottomLeft, dotTopLeft), lerp(v, dotBottomRight, dotTopRight));
}

float Noise::perlin3d(float x, float y, float z) {
	// find the unit grid the point lies in
	unsigned char unitX = (int)x % 255;
	unsigned char unitY = (int)y % 255;
	unsigned char unitZ = (int)z % 255;

	// find the coordinates of the point within the grid
	float localX = x - unitX;
	float localY = y - unitY;
	float localZ = z - unitZ;

	// find the vector from each corner of the unit grid to the point
	glm::vec3 topRightBack = glm::vec3(localX - 1.0, localY - 1.0, localZ);
	glm::vec3 topLeftBack = glm::vec3(localX, localY - 1.0, localZ);
	glm::vec3 bottomRightBack = glm::vec3(localX - 1.0, localY, localZ);
	glm::vec3 bottomLeftBack = glm::vec3(localX, localY, localZ);
	glm::vec3 topRightFront = glm::vec3(localX - 1.0, localY - 1.0, localZ - 1.0);
	glm::vec3 topLeftFront = glm::vec3(localX, localY - 1.0, localZ - 1.0);
	glm::vec3 bottomRightFront = glm::vec3(localX - 1.0, localY, localZ - 1.0);
	glm::vec3 bottomLeftFront = glm::vec3(localX, localY, localZ - 1.0);

	// select pseudo random value for each corner in grid
	unsigned char trb, tlb, brb, blb, trf, tlf, brf, blf;
	trb = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX + 1] + unitY + 1] + unitZ];
	tlb = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX] + unitY + 1] + unitZ];
	brb = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX + 1] + unitY] + unitZ];
	blb = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX] + unitY] + unitZ];
	trf = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX + 1] + unitY + 1] + unitZ + 1];
	tlf = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX] + unitY + 1] + unitZ + 1];
	brf = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX + 1] + unitY] + unitZ + 1];
	blf = m_doublePermutation[m_doublePermutation[m_doublePermutation[unitX] + unitY] + unitZ + 1];

	// dot product the distance vector with the gradient vector
	float dotTopRightBack = glm::dot(topRightBack, grad3d(trb));
	float dotTopLeftBack = glm::dot(topLeftBack, grad3d(tlb));
	float dotBottomRightBack = glm::dot(bottomRightBack, grad3d(brb));
	float dotBottomLeftBack = glm::dot(bottomLeftBack, grad3d(blb));
	float dotTopRightFront = glm::dot(topRightFront, grad3d(trf));
	float dotTopLeftFront = glm::dot(topLeftFront, grad3d(tlf));
	float dotBottomRightFront = glm::dot(bottomRightFront, grad3d(brf));
	float dotBottomLeftFront = glm::dot(bottomLeftFront, grad3d(blf));

	// fade the local values before lerp to smooth the transition
	float u = fade(localX);
	float v = fade(localY);
	float w = fade(localZ);

	// interpolate each value to find the value for the local coordinate
	return lerp(w, lerp(u, lerp(v, dotBottomLeftBack, dotTopLeftBack), lerp(v, dotBottomRightBack, dotTopRightBack)), lerp(u, lerp(v, dotBottomLeftFront, dotTopLeftFront), lerp(v, dotBottomRightFront, dotTopRightFront)));
}

// fade function to smooth the transition over grid borders
float Noise::fade(float t) {
	return ((6 * t - 15) * t + 10) * t * t * t;
}

// map the hashed corner value with a gradient vector for the corner
glm::vec2 Noise::grad2d(int hash) {
	switch (hash % 4) {
		case 0: return  glm::vec2(1.0, 1.0);
		case 1: return  glm::vec2(-1.0, 1.0);
		case 2: return  glm::vec2(-1.0, -1.0);
		case 3: return  glm::vec2(1.0, -1.0);
		default: return  glm::vec2(1.0, 1.0);
	}
}

// map the hashed corner value with a gradient vector for the corner
glm::vec3 Noise::grad3d(int hash) {
	switch (hash % 8) {
	case 0: return  glm::vec3(1.0, 1.0, -1.0);
	case 1: return  glm::vec3(-1.0, 1.0, -1.0);
	case 2: return  glm::vec3(-1.0, -1.0, -1.0);
	case 3: return  glm::vec3(1.0, -1.0, -1.0);
	case 4: return  glm::vec3(1.0, 1.0, 1.0);
	case 5: return  glm::vec3(-1.0, 1.0, 1.0);
	case 6: return  glm::vec3(-1.0, -1.0, 1.0);
	case 7: return  glm::vec3(1.0, -1.0, 1.0);
	default: return  glm::vec3(1.0, 1.0, -1.0);
	}
}

// linear interpolation
float Noise::lerp(float t, float a, float b) {
	return a + t * (b - a);
}