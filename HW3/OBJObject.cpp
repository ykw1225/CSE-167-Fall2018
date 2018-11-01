#include "OBJObject.h"
#include "Window.h"
#include "stb_image.h"

// load image file into texture object
GLuint loadTexture() {
	GLuint id;     // storage for one texture
	int twidth, theight, nrComponents;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	// Load image file
	tdata = stbi_load("tex1.jpg", &twidth, &theight, &nrComponents, 0);
	if (tdata == NULL) cout << "Faile to load image." << endl;

	// Create ID for texture
	glGenTextures(1, &id);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, id);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return id;
}

OBJObject::OBJObject(string filepath) {
	toWorld = mat4(1.0f);
	parse(filepath);
	setupPipeline();
	textureID = loadTexture();
}

// Populate the face indices, vertices, and normals vectors with the OBJ Object data
void OBJObject::parse(string filepath) {
	// file stream input
	ifstream readFile;

	// open the file
	readFile.open(filepath, ios::in | ios::binary);

	// get next block
	auto nextBlock = readFile.peek();

	// variables for vertices and vertex normals
	float vx, vy, vz, vnx, vny, vnz, vtx, vty;

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
			vertices.push_back(vec3(vx, vy, vz));
		}

		// check if this line is a vertex normal
		else if (block == "vn") {
			readFile >> vnx >> vny >> vnz;
			normals.push_back(vec3(vnx, vny, vnz));

			// make a color[0,1] vector from normal
			vec3 color = normalize(vec3(vnx, vny, vnz)) * 0.5f + 0.5f;
			colors.push_back(color);
		}

		// check if this line is a texture coordinate
		else if (block == "vt") {
			readFile >> vtx >> vty;
			texCoors.push_back(vec2(vtx, vty));
		}

		// check if this line is a face
		else if (block == "f") {
			readFile >> f1; readFile.ignore(10, ' ');
			readFile >> f2; readFile.ignore(10, ' ');
			readFile >> f3;
			indices.push_back(f1-1);
			indices.push_back(f2-1);
			indices.push_back(f3-1);
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
		combs.push_back(Comb(vertices[i], texCoors[i]));
}

void OBJObject::draw(mat4 C, GLuint shaderProgram) {
	auto proj = Window::P;
	auto modelView = Window::V * toWorld * C;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
		1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelView"),
		1, GL_FALSE, &modelView[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::update() {
}

void OBJObject::setupPipeline() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Comb) * combs.size(), &(combs[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Comb), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Comb), (GLvoid*)offsetof(Comb, vertex));

	/*glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Comb), (GLvoid*)offsetof(Comb, texCoor));*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
