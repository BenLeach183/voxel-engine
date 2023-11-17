#pragma once
#include "Lights/PointLight.h"

/*
	Inherits point light class
	Point light has a position
	and attenuation constants

	Spot light has additional
	direction and radii
*/

class SpotLight : public PointLight {
public:
	// default constructor
	SpotLight() = default;

	// constructor with colour position, attenuation constants, direction and radii
	SpotLight(glm::vec3 colour, glm::vec3 position, glm::vec3 constants, glm::vec3 dir, glm::vec2 radii) : 
		PointLight(colour, position, constants), m_direction(dir), m_radii(radii) {};
	~SpotLight() {};

	// updates the uniforms for the light in a given shader
	void setLightUniforms(Shader* shader, unsigned int id);

	// sets the direction and the radii
	void setDirection(glm::vec3 direction) {m_direction = direction; }
	void setRadii(glm::vec2 radii) { m_radii = radii; }

	// gets the direction and the radii
	glm::vec3& getDirection() { return m_direction; }
	glm::vec2& getRadii() { return m_radii; }
protected:
	glm::vec3 m_direction;
	glm::vec2 m_radii;
};