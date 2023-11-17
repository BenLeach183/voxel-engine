#pragma once
#include "Lights/Light.h"

/*
	Inherits light base class
	Point light has a position
	and attenuation constants
*/

class PointLight : public Light {
	public:
		// default constructor
		PointLight() = default;

		// constructor with colour position and attenuation constants
		PointLight(glm::vec3 colour, glm::vec3 position, glm::vec3 constants) : Light(colour), m_position(position), m_constants(constants) {};
		~PointLight() {};

		// updates the uniforms for the light in a given shader
		void setLightUniforms(Shader* shader, unsigned int id);

		// sets the position and the attenuation constants
		void setConstants(glm::vec3 constants) { m_constants = constants;  }
		void setPosition(glm::vec3 position) { m_position = position; }

		// gets the position and the attenuation constants
		glm::vec3& getPosition() { return m_position;  }
		glm::vec3& getConstants() { return m_constants;  }
	protected:
		glm::vec3 m_constants;
		glm::vec3 m_position;
};