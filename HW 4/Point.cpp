#include "Point.h"
#include "Window.h"
#include "Track.h"

Point::Point(vec3 position) {
	vertPts.push_back(position);
	this->toWorld = mat4(1.0f);
	setupPipelinePt();
}

void Point::setupPipelinePt() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertPts.size() * sizeof(vec3), &vertPts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Point::drawPts(GLuint shaderProgram, GLuint ID) {
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
	glEnable(GL_POINT_SMOOTH);
	glBindVertexArray(VAO);

	GLuint idLocation = glGetUniformLocation(shaderProgram, "id");
	glUniform1ui(idLocation, ID);

	GLuint id2 = 0;
	if (ID % 3 != 1) {
		id2 = 1;
	}
	GLuint idLocation2 = glGetUniformLocation(shaderProgram, "id2");
	glUniform1ui(idLocation2, id2);
	glDrawArrays(GL_POINTS, 0, (GLsizei)vertPts.size());
	glBindVertexArray(0);
}

vec3 Point::move(vec3 v) {
	vertPts[0] += v;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertPts.size() * sizeof(vec3), &vertPts[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	return vertPts[0];
}