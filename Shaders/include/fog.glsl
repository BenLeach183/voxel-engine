vec3 fogColor = vec3(0.8f);
float fogDensity = 0.06f;

vec3 fog(vec3 f_result) {
	float distance = length(viewPos.xz-posInWS.xz);
	float disRatio = 4.0f * distance / 20.0f;
	
	float fogFactor = exp(-disRatio * fogDensity * disRatio * fogDensity);
	
	return mix(fogColor, f_result, fogFactor);
}