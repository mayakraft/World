#include "../world.h"

void setup(){
	simpleLights();
}
void update(){ }
void draw3D(){
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(3,0,1);
		drawPlatonicSolidFaces(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,3,1);
		drawPlatonicSolidFaces(3);
	glPopMatrix();
	glDisable(GL_LIGHTING);
	drawAxesLabels(5);
}
void draw2D(){ 
	orientationText(5, 15, 10);
}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }