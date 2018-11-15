#version 330 core
// This is a sample fragment shader.

uniform samplerCube skybox;
uniform vec3 cameraPos;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 colour;
in vec3 nor;
in vec3 position;

// You can output many things. The first vec4 type output determines the color of the fragment

out vec4 colorFinal;

void main() {
	// colorFinal = vec4(colour, 1.0f);

	vec3 I = normalize(position - cameraPos);
	vec3 R = reflect(I, normalize(nor));
	colorFinal = texture(skybox, R);
	//vec4(texture(skybox, R).rgb, 1.0);
}