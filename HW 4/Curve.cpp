#include "Curve.h"
#include "Window.h"

Curve::Curve() {
	pointsNum = 1;
	seg = 2000;
	float scaler = 2.0f;

	ctrlPoints.push_back(vec3(-50.0f, 40.0f, 50.0f) * scaler);
	ctrlPoints.push_back(vec3(-30.0f, 30.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(10.0f, 20.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(30.0f, 20.0f, 50.0f) * scaler);

	ctrlPoints.push_back(vec3(20.0f, -10.0f, 50.0f) * scaler);
	ctrlPoints.push_back(vec3(10.0f, 10.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(-30.0f, 0.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(-20.0f, 10.0f, 50.0f) * scaler);

	ctrlPoints.push_back(vec3(10.0f, 20.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(30.0f, 20.0f, 50.0f) * scaler);
	ctrlPoints.push_back(vec3(-30.0f, 30.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(-50.0f, 40.0f, 50.0f) * scaler);

	ctrlPoints.push_back(vec3(-10.0f, 20.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(20.0f, -10.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(30.0f, 10.0f, 50.0f) * scaler);
	ctrlPoints.push_back(vec3(10.0f, -20.0f, 50.0f) * scaler);

	ctrlPoints.push_back(vec3(20.0f, 10.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(10.0f, 10.0f, 0.0f) * scaler);
	ctrlPoints.push_back(vec3(-30.0f, -10.0f, 50.0f) * scaler);
	ctrlPoints.push_back(vec3(-40.0f, 20.0f, 0.0f) * scaler);

	setup();
}

Curve::~Curve() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Curve::setup() {
    currPos = ctrlPoints[0];
	for (int i = 0; i < pointsNum; i++) {
		vec3 p0 = ctrlPoints[3 * i];
		vec3 p1 = ctrlPoints[3 * i + 1];
		vec3 p2 = ctrlPoints[3 * i + 2];
		vec3 p3 = ctrlPoints[(3 * i + 3) % (3 * pointsNum)];
		for (float j = 0; j < seg; j++) {
			float t = j / seg;
			vec3 p = (p3 - 3.0f*p2 + 3.0f*p1 - p0) * t*t*t + (3.0f*p0 - 6.0f* p1 + 3.0f*p2) * t* t + (3.0f*p1 - 3.0f*p0) * t + p0;
			//vec3 p = pow((1.0f - t), 3.0f)*p0 + 3.0f*t*pow((1.0f - t), 2.0f)*p1 + 3.0f*pow(t, 2.0f)*(1.0f - t)*p2 + pow(t, 3.0f)*p3;
			points.push_back(p);
			indices.push_back(i*seg + (int)j);
			indices.push_back(i*seg + (int)j + 1);
		}
	}
    points.push_back(points[0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Curve::draw(GLuint shaderProgram) {
	glUseProgram(shaderProgram);
	auto uProjection = glGetUniformLocation(shaderProgram, "projection");
	auto uView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_LOOP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	currIndex++;
	currIndex = currIndex > points.size() - 1 ? 0 : currIndex;
	currPos = points[currIndex];
}

vec3 Curve::getPos() { return currPos; }// translate(mat4(1.0f), currPos + vec3(0.5f, 0.5f, 0.0f)); }