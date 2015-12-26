#include "world.c"

void setup(){
	glPointSize(64);
	glEnable(GL_POINT_SMOOTH);
}

void update(){

}

void draw(){
	glColor3f(1.0, 1.0, 0.5);
	drawPoint(7,7,7);
}

void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }