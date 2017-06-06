#include "world.h"

void setup(){
	// points large and circular
	glPointSize(75.0);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
}
void update(){ }
void draw3D(){ 
	// sun circle
	glColor3f(1.0, 1.0, 0.8);
	drawPoint(50, 50, 3);
	// moon circle
	glColor3f(0.3, 0.3, 0.3);
	drawPoint(-50, -50, 1);
	glColor3f(1.0, 1.0, 1.0);
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }