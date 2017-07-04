// example 8
//
// multiple windows

#include "../world.h"

void setup(){
	// simpleLights();
	// OPTIONS ^= SET_SHOW_GROUND;
	// OPTIONS ^= SET_SHOW_GRID;
}
void update(){ }
void draw3D(){
	// glEnable(GL_LIGHTING);
	// glPushMatrix();
	// 	glTranslatef(3,0,1);
	// 	drawPlatonicSolidFaces(0);
	// glPopMatrix();
	// glPushMatrix();
	// 	glTranslatef(0,3,1);
	// 	drawPlatonicSolidFaces(3);
	// glPopMatrix();
	// glDisable(GL_LIGHTING);
	// drawAxesLabels(5);
	glViewport(0, 0, (GLsizei) WIDTH*0.5, (GLsizei) HEIGHT*0.5);
	firstPersonPerspective();
		if(OPTIONS & (1 << BIT_SHOW_GRID)){
			float newX = modulusContext(-origin[0], 5);
			float newY = modulusContext(-origin[1], 5);
			float newZ = modulusContext(-origin[2], 5);
			glPushMatrix();
				glTranslatef(newX, newY, newZ);
				drawAxesGrid(newX, newY, newZ, 5, 4);
			glPopMatrix();
		}
		// 2D REPEATED STRUCTURE
		if(OPTIONS & (1 << BIT_SHOW_GROUND)){
			float newX = modulusContext(-origin[0], 2);
			float newY = modulusContext(-origin[1], 2);
			glPushMatrix();
				glTranslatef(newX, newY, -origin[2]);
				drawCheckerboard(newX, newY, 8);
			glPopMatrix();
		}

	glViewport((GLsizei) WIDTH*0.5, (GLsizei) HEIGHT*0.5, (GLsizei) WIDTH*0.5, (GLsizei) HEIGHT*0.5);
	orthoPerspective(orthoFrame[0], orthoFrame[1], orthoFrame[2], orthoFrame[3]);
		if(OPTIONS & (1 << BIT_SHOW_GRID)){
			float newX = modulusContext(-origin[0], 5);
			float newY = modulusContext(-origin[1], 5);
			float newZ = modulusContext(-origin[2], 5);
			glPushMatrix();
				glTranslatef(newX, newY, newZ);
				drawAxesGrid(newX, newY, newZ, 5, 4);
			glPopMatrix();
		}
		// 2D REPEATED STRUCTURE
		if(OPTIONS & (1 << BIT_SHOW_GROUND)){
			float newX = modulusContext(-origin[0], 2);
			float newY = modulusContext(-origin[1], 2);
			glPushMatrix();
				glTranslatef(newX, newY, -origin[2]);
				drawCheckerboard(newX, newY, 8);
			glPopMatrix();
		}

	glViewport((GLsizei) WIDTH*0.5, 0, (GLsizei) WIDTH*0.5, (GLsizei) HEIGHT*0.5);
	polarPerspective();

	// update orthographic frame with new aspect ratio
	// float newW = orthoFrame[3] * ((float)WIDTH / (float)HEIGHT);
	// float dW = orthoFrame[2] - newW;
	// orthoFrame[2] = newW;
	// orthoFrame[0] += dW * 0.5;
}
void draw2D(){ 
	orientationText(5, 15, 10);
}
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }