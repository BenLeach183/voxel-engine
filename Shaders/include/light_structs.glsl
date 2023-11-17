struct pointLight {
	bool enabled;
	vec3 colour;
	vec3 position;
	vec3 constants;
};

struct spotLight {
	bool enabled;
	vec3 colour;
	vec3 position;
	vec3 constants;
	vec3 direction;
	vec2 radii;
};

uniform bool directionalLightEnabled;
uniform vec3 directionalLightDir;
uniform vec3 directionalLightCol;
uniform float directionalAmbient;