#include "OBJObject.h"

OBJObject::OBJObject(string filepath) {
	toWorld = mat4(1.0f);
	parse(filepath);
}

// Populate the face indices, vertices, and normals vectors with the OBJ Object data
void OBJObject::parse(string filepath) {
	// file stream input
	ifstream readFile;

	// open the file
	readFile.open(filepath, ios::in | ios::binary);

	// check if the file is open correctly
	if (!readFile.is_open()) cout << "Fail to read file" << filepath << endl;
	
	// get next block
	auto nextBlock = readFile.peek();

	// variables for vertices and vertex normals
	float vx, vy, vz, vnx, vny, vnz;

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
			colors.push_back(normalize(vec3(vnx, vny, vnz)));
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
}

void OBJObject::draw() {
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);

	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		glColor3f(colors[i].x, colors[i].y, colors[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}

	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}