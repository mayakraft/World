#include "world.h"

GLuint texture;

void setup() {
	texture = loadTexture("texture.raw", 32, 32);
}
void update() { }
void draw() {
	glPushMatrix();
		glTranslatef(0, 0, 2);
		glBindTexture(GL_TEXTURE_2D, texture);
		drawUnitSquare(-0.5, -0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
	glPopMatrix();
}
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }