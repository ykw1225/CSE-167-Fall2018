#include "Track.h"
#include "Window.h"

vec3 x4_Old, x3_Old;

void Track::newC(vec3 x3, vec3 x4) {
	vec3 x1 = x4_Old, x2 = x3_Old + (x4_Old - x3_Old) * 2.0f;
	x3_Old = x3; x4_Old = x4;

	Curve* newC = new Curve(x1, x2, x3, x4);

	curves.push_back(newC);
	vertPts.push_back(x2); vertPts.push_back(x3);
	pts.push_back(new Point(x1)); pts.push_back(new Point(x2)); pts.push_back(new Point(x3));
	addVer();
}

void Track::addVer() {
	for (auto i : curves.back()->vertQ) vertices.push_back(i);
}

Track::Track() {
	toWorld = mat4(1.0f);

	vec3 x1, x2, x3, x4;
	x1 = vec3(0.0f, 2.0f, 0.0f); x2 = vec3(1.0f, 1.0f, 0.0f); x3 = x3_Old = vec3(2.0f,-1.0f, 0.0f); x4 = x4_Old = vec3(3.0f, 0.0f, 0.0f);
	Curve* c1 = new Curve(x1, x2, x3, x4); curves.push_back(c1);
	vertPts.push_back(x3); pts.push_back(new Point(x1)); pts.push_back(new Point(x2)); pts.push_back(new Point(x3));
	addVer();

	newC(vec3(2.0f,-1.0f,-2.0f), vec3(2.0f,-1.0f,-1.0f));	newC(vec3(4.0f, 2.0f, 2.0f), vec3(4.0f, 1.0f, 3.0f));
	newC(vec3(3.0f,-1.0f, 2.0f), vec3(2.0f,-1.0f, 2.0f));	newC(vec3(2.0f,-0.5f,-0.5f), vec3(1.0f,-0.5f, 0.0f));
	newC(vec3(1.5f,-0.5f, 0.5f), vec3(0.5f, 0.0f, 1.0f));	newC(vec3(1.0f, 1.0f, 0.5f), vec3(0.0f, 0.0f, 0.0f));
	newC(vec3(1.0f, 1.0f, 0.0f) + (vec3(0.0f, 2.0f, 0.0f) - vec3(1.0f, 1.0f, 0.0f))*2.0f, vec3(0.0f, 2.0f, 0.0f));

	vertPts.push_back(vec3(1.0f, 1.0f, 0.0f));
	setupPipeline();
}

void Track::setupPipeline() {
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

void Track::draw(GLuint shaderProgram) {
	for (int i = 0; i < curves.size(); i++)
		curves[i]->draw(shaderProgram);
}

void Track::drawpoint(GLuint ptshader) {
	for (int i = 0; i < pts.size(); i++) pts[i]->drawPts(ptshader, i+1);
}

void Track::drawLine(GLuint shaderProgram) {
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

	glBindVertexArray(VAO);
	glLineWidth(1);
	glDrawArrays(GL_LINES, 0, (GLsizei)vertPts.size());
	glBindVertexArray(0);
}

void Track::update(GLuint curPt, GLuint m, vec3 ctrlPtPos) {
	curves[curPt]->update(m, ctrlPtPos);
	if (m == 2 || m == 3) {
		if (curPt == 0 && m == 2) vertPts[15] = ctrlPtPos;
		else vertPts[(curPt - 1) * 2 + m - 1] = ctrlPtPos;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertPts.size()*sizeof(vec3), &vertPts[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	vertices.clear();
	for (int i = 0; i < curves.size(); i++) {
		for (auto j : curves[i]->vertQ) vertices.push_back(j);
	}
}