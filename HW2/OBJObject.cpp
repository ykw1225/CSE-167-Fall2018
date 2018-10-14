#include "OBJObject.h"
#include "Window.h"

float OBJObject::randFloat(float min, float max) {
	return(max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

OBJObject::OBJObject(string filepath) {
	speed = 0.003f;
	alterColor = false;
	angle = 0.0f;
	point = 1.0f;
	toWorld = mat4(1.0f);
	xMax = yMax = zMax = FLT_MIN;
	xMin = yMin = zMin = FLT_MAX;
	parse(filepath);
	setupPipeline();
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
			/*vec3 color = normalize(vec3(vx, vy, vz)) * 0.5f + 0.5f;
			altColors.push_back(color);*/

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

	// cout << "min(" << xMin << "," << yMin << "," << zMin << ")   max(" << xMax << "," << yMax << "," << zMax << ")" << endl;
	centerAndScale();

	// randomly initialize vertices within the min and max
	/*srand(static_cast <unsigned> (time(0)));
	for (int i = 0; i < vertices.size(); i++) {
		auto randX = randFloat(xMin*5.0f, xMax*5.0f);
		auto randY = randFloat(yMin*5.0f, yMax*5.0f);
		auto randZ = randFloat(zMin*5.0f, zMax*5.0f);
		randVer.push_back(vec3(randX, randY, randZ));
	}*/

	for (int i = 0; i < vertices.size(); i++)
		combs.push_back(Comb(vertices[i], normals[i], colors[i]));
}

void OBJObject::draw(GLuint shaderProgram) {
	//glMatrixMode(GL_MODELVIEW);

	//// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	//glPushMatrix();
	//glMultMatrixf(&(toWorld[0][0]));

	//glBegin(GL_POINTS);

	//// Loop through all the vertices of this OBJ Object and render them
	//for (unsigned int i = 0; i < vertices.size(); ++i) {
	//	if (alterColor)glColor3f(altColors[i].x, altColors[i].y, altColors[i].z);
	//	else glColor3f(colors[i].x, colors[i].y, colors[i].z);
	//	glVertex3f(randVer[i].x, randVer[i].y, randVer[i].z);
	//}

	//glEnd();

	//// Pop the save state off the matrix stack
	//// This will undo the multiply we did earlier
	//glPopMatrix();

	auto view = Window::V;
	auto proj = Window::P;
	auto model = toWorld;

	auto uProjection = glGetUniformLocation(shaderProgram, "projection");
	auto uView = glGetUniformLocation(shaderProgram, "view");
	auto uToWorld = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &model[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::update() {
	//spin(1.0f);
	//verMove();
	//speed *= 1.5f;
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
		vertices[i] /= scaler;
		vertices[i] *= 4.0f;
	}

	// determine the new min and max after scaling
	xMax = yMax = zMax = FLT_MIN; xMin = yMin = zMin = FLT_MAX;
	for (auto i : vertices) minMax(i.x, i.y, i.z);
}

void OBJObject::floatChange(float &f, float distance) { f = f + distance * speed; }

void OBJObject::verMove() {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x != randVer[i].x)
			floatChange(randVer[i].x, vertices[i].x - randVer[i].x);
		if (vertices[i].y != randVer[i].y)
			floatChange(randVer[i].y, vertices[i].y - randVer[i].y);
		if (vertices[i].z != randVer[i].z)
			floatChange(randVer[i].z, vertices[i].z - randVer[i].z);
	}
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Comb), (GLvoid*)offsetof(Comb, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Comb), (GLvoid*)offsetof(Comb, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}