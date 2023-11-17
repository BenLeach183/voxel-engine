#include "Lights/PointLight.h"

// update the light uniforms in a shader
void PointLight::setLightUniforms(Shader* shader, unsigned int id) {
	// bind shader
	shader->use();

	// set uniforms for array element of light struct
	shader->setBool("pointLights[" + std::to_string(id) + "].enabled", m_enabled);
	shader->setVec3("pointLights[" + std::to_string(id) + "].colour", m_colour);
	shader->setVec3("pointLights[" + std::to_string(id) + "].position", m_position);
	shader->setVec3("pointLights[" + std::to_string(id) + "].constants", m_constants);
}