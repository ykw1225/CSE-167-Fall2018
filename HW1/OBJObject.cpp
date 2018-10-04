#include "OBJObject.h"

OBJObject::OBJObject(string filepath) {
	angle = 0.0f;
	point = 1.0f;
	toWorld = mat4(1.0f);
	xMax = yMax = zMax = FLT_MIN;
	xMin = yMin = zMin = FLT_MAX;
	parse(filepath);
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

			// find minimum and maximum coordinates along each dimension
			minMax(vx, vy, vz);
		}

		// check if this line is a vertex normal
		else if (block == "vn") {
			readFile >> vnx >> vny >> vnz;
			normals.push_back(vec3(vnx, vny, vnz));

			// make a color[0,1] vector from normal
			vec3 color = normalize(vec3(vnx, vny, vnz)) * 0.5f + 0.5f;
			colors.push_back(color);
			//if (color.x < 0.0f || color.x > 1.0f) cout << "Bad color!!!" << color.x << endl;
			//else if (color.y < 0.0f || color.y > 1.0f) cout << "Bad color!!!" << color.y << endl;
			//else if (color.z < 0.0f || color.z > 1.0f) cout << "Bad color!!!" << color.z << endl;
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

	cout << "min(" << xMin << "," << yMin << "," << zMin << ")   max(" << xMax << "," << yMax << "," << zMax << ")" << endl;
	centerAndScale();
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

void OBJObject::update() {
	spin(1.0f);
}

void OBJObject::spin(float deg) {
	angle += deg;
	if (angle > 360.0f || angle < -360.0f) angle = 0.0f;
	toWorld = rotate(glm::mat4(1.0f), angle / 180.0f * pi<float>(), vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::minMax(float x, float y, float z) {
	xMax = x > xMax ? x : xMax; xMin = x < xMin ? x : xMin;
	yMax = y > yMax ? y : yMax; yMin = y < yMin ? y : yMin;
	zMax = z > zMax ? z : zMax; zMin = z < zMin ? z : zMin;
}

void OBJObject::centerAndScale() {
	// determine the center points
	auto xCenter = (xMax + xMin) / 2.0f;
	auto yCenter = (yMax + yMin) / 2.0f;
	auto zCenter = (xMax + zMin) / 2.0f;
	auto center = vec3(xCenter, yCenter, zCenter);

	// determine the scalers
	auto xScaler = xMax - xCenter, yScaler = yMax - yCenter, zScaler = zMax - zCenter;
	auto scaler = FLT_MIN;
	scaler = scaler > xScaler ? scaler : xScaler;
	scaler = scaler > yScaler ? scaler : yScaler;
	scaler = scaler > zScaler ? scaler : zScaler;

	// centering and scaling the model
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] -= center;
		vertices[i] *= 1.0f / scaler;
	}
}