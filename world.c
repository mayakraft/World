#include "world.h"

void setup(){ }
void update(){ }
void draw3D(){ 
	glPushMatrix();
		glTranslatef(3, 3, 1);
		drawPlatonicSolidLines(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-3, -3, 1);
		drawPlatonicSolidLines(4);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3, -3, 1);
		drawPlatonicSolidLines(5);
	glPopMatrix();

}
void draw2D(){ 
	worldInfoText(30, 30, 10);
}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }