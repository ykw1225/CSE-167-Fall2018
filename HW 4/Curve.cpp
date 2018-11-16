#include "Window.h"
#include "Curve.h"

int i = 0;

Curve::Curve(vec3 c1, vec3 c2, vec3 c3, vec3 c4) {
	toWorld = mat4(1.0f);

	controlPoints.push_back(c1);
	controlPoints.push_back(c2);
	controlPoints.push_back(c3);
	controlPoints.push_back(c4);

	vertPts.push_back(c1);
	vertPts.push_back(c2);
	vertPts.push_back(c3);

	m[0] = c1;
	m[1] = c2;
	m[2] = c3;
	m[3] = c4;

	vec3 q0 = m[0];
	vec3 q1 = m[3];

	findVert(q0, q1);

	setupPipeline();
	setupPipelinePt();
}

void Curve::draw(GLuint shaderProgram){
	glUseProgram(shaderProgram);

	auto model = toWorld;
	auto view = Window::V;
	auto projection = Window::P;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
		1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
		1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
		1, GL_FALSE, &view[0][0]);

	glPointSize(5);
	glBindVertexArray(VAO);

	glLineWidth(3);
	glDrawArrays(GL_LINES, 0, (GLsizei)vertQ.size());
	glBindVertexArray(0);
}

void Curve::setupPipeline() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertQ.size() * sizeof(vec3), &vertQ[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Curve::setupPipelinePt() {
	glGenVertexArrays(1, &VDO);
	glGenBuffers(1, &VCO);

	glBindVertexArray(VDO);

	glBindBuffer(GL_ARRAY_BUFFER, VCO);
	glBufferData(GL_ARRAY_BUFFER, vertQ.size() * sizeof(vec3), &vertPts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Curve::update(GLuint mod, vec3 control_point) {
	controlPoints[mod - 1] = control_point;
	m[0] = controlPoints[0];
	m[1] = controlPoints[1];
	m[2] = controlPoints[2];
	m[3] = controlPoints[3];
	vec3 q0 = m[0];
	vec3 q1 = m[3];

	vertQ.clear();
	findVert(q0, q1);
	setupPipeline();
}

vec4 Curve::calCT(float t) {
	vec4 ct;
	ct.x = pow((1 - t), 3);
	ct.y = 3 * t * pow((1 - t), 2);
	ct.z = 3 * (1 - t) * pow(t, 2);
	ct.w = pow(t, 3);
	return ct;
}

void Curve::findVert(vec3 q0, vec3 q1) {
	vertQ.push_back(q0);

	for (float i = 0.01f; i < 1.0f; i = i + 0.01f) {
		vec4 ctQ = calCT(i);
		vec3 ptQ = m * ctQ;
		vertQ.push_back(ptQ);
		vertQ.push_back(ptQ);
	}
	vertQ.push_back(q1);
}