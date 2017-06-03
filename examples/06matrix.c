// example 1
// playing with rotation matrices

#include "../world.h"

int zoom = 1;

// float m[16] = {
// 	1.0, 0.0, 0.0, 0.0,
// 	0.0, -1.0, 0.0, 0.0,
// 	0.0, 0.0, 1.0, 0.0,
// 	0.0, 0.0, 0.0, 1.0
// };

float m[16] = {
	0.0, 0.0, 1.0, 0.0,
	0.0, -1.0, 0.0, 0.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};


void drawUnitCubeWireframe(){
	float l = 0.66;
	drawLine(1.0, 1.0, 1.0, 1.0, 1.0, l);
	drawLine(1.0, 1.0, 1.0, 1.0, l, 1.0);
	drawLine(1.0, 1.0, 1.0, l, 1.0, 1.0);
	drawLine(-1.0, 1.0, 1.0, -1.0, 1.0, l);
	drawLine(-1.0, 1.0, 1.0, -1.0, l, 1.0);
	drawLine(-1.0, 1.0, 1.0, -l, 1.0, 1.0);
	drawLine(1.0, -1.0, 1.0, 1.0, -1.0, l);
	drawLine(1.0, -1.0, 1.0, 1.0, -l, 1.0);
	drawLine(1.0, -1.0, 1.0, l, -1.0, 1.0);
	drawLine(1.0, 1.0, -1.0, 1.0, 1.0, -l);
	drawLine(1.0, 1.0, -1.0, 1.0, l, -1.0);
	drawLine(1.0, 1.0, -1.0, l, 1.0, -1.0);
	drawLine(-1.0, -1.0, 1.0, -1.0, -1.0, l);
	drawLine(-1.0, -1.0, 1.0, -1.0, -l, 1.0);
	drawLine(-1.0, -1.0, 1.0, -l, -1.0, 1.0);
	drawLine(-1.0, 1.0, -1.0, -1.0, 1.0, -l);
	drawLine(-1.0, 1.0, -1.0, -1.0, l, -1.0);
	drawLine(-1.0, 1.0, -1.0, -l, 1.0, -1.0);
	drawLine(1.0, -1.0, -1.0, 1.0, -1.0, -l);
	drawLine(1.0, -1.0, -1.0, 1.0, -l, -1.0);
	drawLine(1.0, -1.0, -1.0, l, -1.0, -1.0);
	drawLine(-1.0, -1.0, -1.0, -1.0, -1.0, -l);
	drawLine(-1.0, -1.0, -1.0, -1.0, -l, -1.0);
	drawLine(-1.0, -1.0, -1.0, -l, -1.0, -1.0);
}

void drawUnitCubeLabels(){
	text("1 1 1", 1, 1, 1);
	text("-1 1 1", -1, 1, 1);
	text("1 -1 1", 1, -1, 1);
	text("1 1 -1", 1, 1, -1);
	text("-1 -1 1", -1, -1, 1);
	text("-1 1 -1", -1, 1, -1);
	text("1 -1 -1", 1, -1, -1);
	text("-1 -1 -1", -1, -1, -1);
}

void drawUnitAxesLabels(float scale){
	text("+X", scale, 0, 0);
	text("+Y", 0, scale, 0);
	text("+Z", 0, 0, scale);
	text("-X", -scale, 0, 0);
	text("-Y", 0, -scale, 0);
	text("-Z", 0, 0, -scale);
}



void setup() {
	// glutReshapeWindow(400, 400);
	// hideHelpfulOrientation();
	polarPerspective(0, 0, 0);
	GROUND = 0;
	// GRID = 0;
	lookOrientation[0] = 90;
	lookOrientation[1] = 90;
	lookOrientation[2] = sqrt( 1 + powf(4,zoom));
}
void update() { }
void draw3D() {
	glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glColor3f(1.0, 1.0, 1.0);
		drawUnitCubeWireframe();
		glColor3f(1.0, 1.0, 0.0);
		drawUnitCubeLabels();
		drawUnitAxesLabels(1);
	glPopMatrix();

	glPushMatrix();
		glMultMatrixf(m);
		glColor3f(1.0, 1.0, 1.0);
		drawUnitCubeWireframe();
		glColor3f(0.3, 0.6, 1.0);
		drawUnitCubeLabels();
		drawUnitAxesLabels(1);
	glPopMatrix();
}
void draw2D() { }
void keyDown(unsigned int key) {
	if(key == ' '){
		zoom = (zoom+1)%4;
		lookOrientation[2] = sqrt( 1 + powf(4,zoom));
	}
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }