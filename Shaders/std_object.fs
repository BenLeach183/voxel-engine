#version 330 core
// Material Properties
uniform int specularShine;
uniform float staticSpecularStrength;
uniform vec3 staticAlbedo;

// Material Textures
uniform bool isTextured;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// Toggles
uniform bool nMapEnabled;

// Camera position
uniform vec3 viewPos;

in vec3 normal;
in vec3 posInWS;
in vec2 UV;
in mat3 TBN;

vec3 viewDir = normalize(viewPos - posInWS);
vec3 normalisedNormal;
float specularStrength;
vec3 albedo;

#include "light_structs.glsl"
#include "light_setup.glsl"
#include "light_methods.glsl"
#include "fog.glsl"

out vec4 FragColor;

void main() {
	// if a texture is being used swap the albedo and specular strength with texture values
	if(isTextured) {
		albedo = texture(diffuseTexture, UV).rgb;
		specularStrength = texture(specularTexture, UV).r;
		
		if(nMapEnabled) {
			vec3 n = texture(normalTexture, UV).rgb;
			n = n*2.0 - 1.0;
			n = normalize(TBN*n);
			normalisedNormal = n;
		} else {
			normalisedNormal = normalize(normal);
		}

	} else {
		normalisedNormal = normalize(normal);
		specularStrength = staticSpecularStrength;
		albedo = staticAlbedo;
	}
	
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