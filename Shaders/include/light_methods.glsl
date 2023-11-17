vec3 getAmbient(vec3 a_albedo, vec3 a_lightColour, float a_ambientStrength) {
	return a_albedo * a_lightColour * a_ambientStrength;
}

vec3 getDiffuse(vec3 d_albedo, vec3 d_lightColour, vec3 d_normal, vec3 d_negLightDirection) {
	float diffuseFactor = dot(d_normal, d_negLightDirection);
	diffuseFactor = max(diffuseFactor, 0);
	
	return d_albedo * diffuseFactor * d_lightColour;
}

vec3 getSpecular(vec3 s_normal, vec3 s_negLightDirection, vec3 s_viewDir, float s_specularStrength, int s_specularShine) {
	vec3 H = normalize(s_negLightDirection + s_viewDir);
	float specLevel = dot(s_normal, H);
	specLevel = max(specLevel, 0.0);
	specLevel = pow(specLevel, s_specularShine);
	
	return specLevel * s_specularStrength * vec3(1.0, 1.0, 1.0);
}

float getAttenuation(vec3 attn_constants, float attn_distance) {
	return 1.0/(attn_constants.x + (attn_constants.y*attn_distance) + (attn_constants.z*(attn_distance*attn_distance)));
}

vec3 getDirectionalLight() {
	vec3 normalisedNegLight = normalize(-directionalLightDir);

	// ambient lighting
	vec3 ambient = getAmbient(albedo, directionalLightCol, directionalAmbient);
	
	// diffuse lighting
	vec3 diffuse = getDiffuse(albedo, directionalLightCol, normalisedNormal, normalisedNegLight);
	
	// specular reflections
	vec3 specular = getSpecular(normalisedNormal, normalisedNegLight, viewDir, specularStrength, specularShine);
	
	return ambient + diffuse + specular;
}

vec3 getPointLight(int index) {
	// attenuation
	float distance = length(pointLights[index].position - posInWS);
	float attn = getAttenuation(pointLights[index].constants, distance);
	
	// find the negative light direction
	vec3 lightDir = normalize(pointLights[index].position - posInWS);
	
	// diffuse
	vec3 diffuse = getDiffuse(albedo, pointLights[index].colour, normalisedNormal, lightDir);
	
	// specular reflections
	vec3 specular = getSpecular(normalisedNormal, lightDir, viewDir, specularStrength, specularShine);
	
	// attentuation
	diffuse = diffuse*attn;
	specular = specular*attn;
	
	return diffuse + specular;
}

vec3 getSpotLight(int index) {
	// find the negative light direction
	vec3 lightDir = normalize(spotLights[index].position - posInWS);
	
	// attn
	float distance = length(spotLights[index].position - posInWS);
	float attn = getAttenuation(spotLights[index].constants, distance);
	
	// diffuse
	vec3 diffuse = getDiffuse(albedo, spotLights[index].colour, normalisedNormal, lightDir);
	
	// specular reflections
	vec3 specular = getSpecular(normalisedNormal, lightDir, viewDir, specularStrength, specularShine);
	
	// apply attenuation
	diffuse = diffuse * attn;
	specular = specular * attn;
	
	// calculate intensity
	float theta = dot(-lightDir, normalize(spotLights[index].direction));
	float denom = (spotLights[index].radii.x - spotLights[index].radii.y);
	float intensity = (theta - spotLights[index].radii.y) / denom;
	intensity = clamp(intensity, 0.0, 1.0);
	
	diffuse = diffuse * intensity;
	specular = specular * intensity;
	
	return diffuse + specular;
}

// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
vec3 aces(vec3 x) {
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((x * (a * x + b)) / (x* (c * x + d) + e), 0.0, 1.0);
}