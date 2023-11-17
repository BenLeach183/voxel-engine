#pragma once
#include "Lights/Light.h"

	/*
		Inherits light base class
		Directional light has a direction
		and an ambient factor
	*/

class DirectionalLight : public Light {
public:
	// default constructor
	DirectionalLight() = default;

	// constructor with colour direction and ambient
	DirectionalLight(glm::vec3 colour, glm::vec3 direction, float ambient = 0.5) : Light(colour), m_direction(direction), m_ambient(ambient) {};
	~DirectionalLight() {};

	// updates the uniforms for the light in a given shader
	void setLightUniforms(Shader* shader);

	// sets the direction and the ambient factor
	void setDirection(glm::vec3 direction) { m_direction = direction; }
	void setAmbient(float ambient) { m_ambient = ambient; }

	// get the direction and the ambient factor
	glm::vec3& getDirection() { return m_direction; }
	float& getAmbient() { return m_ambient; }
protected:
	glm::vec3 m_direction;
	float m_ambient;
};