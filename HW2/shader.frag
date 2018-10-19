#version 330 core
// This is a sample fragment shader.

uniform mat3 material;
uniform float shininess;
uniform bool isPhong;
uniform vec3 camPos;
uniform mat4 model;

uniform vec3 potCol;
uniform vec3 potPos;

uniform vec3 spotCol;
uniform vec3 spotPos;
uniform vec3 spotConeDir;
uniform float spotExp;
uniform float spotCutoff;

uniform vec3 dirCol;
uniform vec3 dirPos;

uniform bool potLightOn;
uniform bool spotLightOn;
uniform bool dirLightOn;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 vert;
in vec3 norm;
in vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment

out vec4 colorFinal;

vec3 CalcPotLight();
vec3 CalcSpotLight();
vec3 CalcDirLight();
vec3 viewDir = normalize(camPos - vert);

void main() {
	vec3 colorTemp = vec3(0.1745f, 0.01175f, 0.01175f);
    if(isPhong){
		if(potLightOn)	colorTemp += CalcPotLight();
		if(spotLightOn)	colorTemp += CalcSpotLight();
		if(dirLightOn)	colorTemp += CalcDirLight();
		colorFinal = vec4(colorTemp, 1.0f);
	}else colorFinal = vec4(color, 1.0f);
}

vec3 CalcPotLight(){
	vec3 dir			= potPos - vert;
    vec3 lightDir		= normalize(dir);
    float diff			= max(dot(norm, lightDir), 0.0f);
    vec3 reflectDir		= reflect(-lightDir, norm);
    float spec			= pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    float distance		= length(dir);
    float attenuation	= 1.0f / (1.0f * distance);    

    vec3 ambient  = potCol * material[0];
    vec3 diffuse  = potCol * diff * material[1] * attenuation;
    vec3 specular = potCol * spec * material[2] * attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight() {
    vec3 dir			= spotPos - vert;
    vec3 lightDir		= normalize(dir);
    float diff			= max(dot(norm, lightDir), 0.0f);
    vec3 reflectDir		= reflect(-lightDir, norm);
    float spec			= pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    float distance		= length(dir);
    float attenuation	= 1.0f / (1.0f * (distance * distance));
    
    float theta = dot(lightDir, normalize(-spotConeDir));
	float intensity = 0.0f; // pow(theta, spotExp);

    vec3 ambient  = spotCol * material[0];
    vec3 diffuse  = spotCol * diff * material[1] * attenuation * intensity;
    vec3 specular = spotCol * spec * material[2] * attenuation * intensity;

	if(theta > spotCutoff){
		vec3 n = normalize(norm);
        intensity = max(dot(n, lightDir), 0.0f);

        if (intensity > 0.0) {
            vec3 eye = normalize(camPos);
            vec3 h = normalize(lightDir + eye);
            float intSpec = max(dot(h, n), 0.0);
            vec3 spec = specular * pow(intSpec, shininess);
        }
		return max(intensity * diffuse + spec, ambient);
	}

   	return max(intensity * diffuse + specular, ambient);
}

vec3 CalcDirLight() {
	vec3 dir		= dirPos - vert;
    vec3 lightDir	= normalize(dir);
	float diff		= max(dot(norm, lightDir), 0.0f);
	vec3 reflectDir	= 2 * (dot(norm, lightDir) * norm) - lightDir;
	float spec		= pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient	= dirCol * material[0];
	vec3 diffuse	= dirCol * diff * material[1];
	vec3 specular	= dirCol * spec * material[2];

	return ambient + diffuse + specular;
}