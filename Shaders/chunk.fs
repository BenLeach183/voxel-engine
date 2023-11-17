#version 330 core
uniform sampler2D textureMap;
uniform float specularStrength;
uniform int specularShine;

uniform vec3 viewPos;

in vec3 normal;
in vec3 posInWS;
in vec2 uv;

vec3 viewDir = normalize(viewPos - posInWS);
vec3 normalisedNormal = normalize(normal);	
vec3 albedo;

out vec4 FragColor;

#include "light_structs.glsl"
#include "light_setup.glsl"
#include "light_methods.glsl"
#include "fog.glsl"

void main() {
	albedo = texture(textureMap, uv).rgb;
	vec3 result;
		
	// get directional light
	if(directionalLightEnabled) result = getDirectionalLight();
	
	// loop over point lights
	for(int i = 0; i < numPL; i++) {
		if(pointLights[i].enabled) result += getPointLight(i);
	}
	
	// loop over spot lights
	for(int i = 0; i < numSL; i++) {
		if(spotLights[i].enabled) result += getSpotLight(i);
	}
	
	result = fog(result);
	result = aces(result);
	
	FragColor = vec4(result, 1.0);
}


