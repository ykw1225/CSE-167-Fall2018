#ifndef _TRACK_h
#define _TRACK_h

#include "Curve.h"
#include "point.h"
#include <vector>

using namespace std;
using namespace glm;

class Track : public Curve {
private:
	void setupPipeline();
	void newC(vec3, vec3);
	
	vector<vec3> vertPts;
public:
	Track();
	void draw(GLuint);
	void drawpoint(GLuint);
	void drawLine(GLuint);
	void update(GLuint id, GLuint mod, vec3 contorl_point_position);
	void addVer();

	vector<Curve*> curves;
	vector<Point*> pts;
	vector<vec3> vertices;
	vec3 curPos, nextPos;
};

#endif

