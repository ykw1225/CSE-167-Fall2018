#include "Sphere.h"
#include "Window.h"

Sphere::Sphere() {
	toWorld = mat4(1.0f);
	parse("sphere.obj");
	setupPipeline();
}

// Populate the face indices, vertices, and normals vectors with the OBJ Object data
void Sphere::parse(string filepath) {
	// file stream input
	ifstream readFile;
	// open the file
	readFile.open(filepath, ios::in | ios::binary);
	// get next block
	auto nextBlock = readFile.peek();
	// variables for vertices and vertex normals
	float vx, vy, vz, vnx, vny, vnz;
	// faces
	unsigned int f1, f2, f3;
	// start parsing the file
	while (nextBlock != EOF) {
		// variable for saving a block
		string block;
		readFile >> block;
		// check if this line is a vertex
		if (block == "v") {
			readFile >> vx >> vy >> vz;
			vertices.push_back(vec3(vx, vy, vz)*0.2f);
		}
		// check if this line is a vertex normal
		else if (block == "vn") {
			readFile >> vnx >> vny >> vnz;
			normals.push_back(vec3(vnx, vny, vnz));
			// make a color[0,1] vector from normal
			vec3 color = normalize(vec3(vnx, vny, vnz)) * 0.5f + 0.5f;
			colors.push_back(color);
		}
		// check if this line is a face
		else if (block == "f") {
			readFile >> f1; readFile.ignore(10, ' ');
			readFile >> f2; readFile.ignore(10, ' ');
			readFile >> f3;
			indices.push_back(f1 - 1);
			indices.push_back(f2 - 1);
			indices.push_back(f3 - 1);
		}
		else {
			string skip;
			getline(readFile, skip);
		}
		// read next block
		nextBlock = readFile.peek();
	}
	// finish parsing
	readFile.close();

	for (int i = 0; i < vertices.size(); i++)
		combs.push_back(Combb(vertices[i], normals[i], colors[i]));
}

void Sphere::draw(GLuint shaderProgram) {
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
	if(Window::FPP) glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &Window::camPos[0]);
	else glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &Window::cam_pos[0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, (GLsizei)(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::setupPipeline() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Combb) * combs.size(), &(combs[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Combb), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Combb), (GLvoid*)offsetof(Combb, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Combb), (GLvoid*)offsetof(Combb, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sphere::move(vec3 v) {
	toWorld = translate(mat4(1.0f), v);
}