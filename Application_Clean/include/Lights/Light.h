#pragma once
#include "Shader.h"
#include <string>

	/*
		Base class for all lights
		only stores the colour and
		if light is active.
	*/

class Light {
public:
	// constructor takes colour value
	Light(glm::vec3 colour) : m_colour(colour) {};

	// setters for colour and if light is enabled
	void setColour(glm::vec3 colour) { m_colour = colour; }
	void setEnabled(bool enabled) { m_enabled = enabled; }

	// getters for colour and if light is enabled
	glm::vec3& getColour() { return m_colour; }
	bool getEnabled() { return m_enabled; }

	// toggle if light is enabled
	void toggleEnabled() { m_enabled = !m_enabled; }
protected:
	// lights colour
	glm::vec3 m_colour;

	// light enabled
	bool m_enabled = true;
};