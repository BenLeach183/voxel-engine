#include "Lights/SpotLight.h"

// update the light uniforms in a shader
void SpotLight::setLightUniforms(Shader* shader, unsigned int id) {
	// bind shader
	shader->use();

	// set uniforms for array element of light struct
	shader->setBool("spotLights[" + std::to_string(id) + "].enabled", m_enabled);
	shader->setVec3("spotLights[" + std::to_string(id) + "].colour", m_colour);
	shader->setVec3("spotLights[" + std::to_string(id) + "].position", m_position);
	shader->setVec3("spotLights[" + std::to_string(id) + "].constants", m_constants);
	shader->setVec3("spotLights[" + std::to_string(id) + "].direction", m_direction);
	shader->setVec2("spotLights[" + std::to_string(id) + "].radii", m_radii);
}