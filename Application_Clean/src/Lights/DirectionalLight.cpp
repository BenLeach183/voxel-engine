#include "Lights/DirectionalLight.h"

// update the light uniforms in a shader
void DirectionalLight::setLightUniforms(Shader* shader) {
	// bind shader
	shader->use();

	// set uniforms for directional light
	shader->setBool("directionalLightEnabled", m_enabled);
	shader->setVec3("directionalLightCol", m_colour);
	shader->setVec3("directionalLightDir", m_direction);
	shader->setFloat("directionalAmbient", m_ambient);
}