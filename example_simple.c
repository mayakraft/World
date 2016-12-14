#include "world.h"

GLuint texture, spectrum;
float matrix[16];
unsigned char showOverlay = 0;

#include "example/518stars.c"
#include "example/1619stars.c"

void renderStars(){
	glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		// bright stars
		glVertexPointer(3, GL_FLOAT, 0, _518_stars);
		glColor3f(1.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 518);
		// dim stars
		glVertexPointer(3, GL_FLOAT, 0, _1619_stars);
		glColor3f(0.5, 0.5, 0.5);
		glDrawArrays(GL_POINTS, 0, 1619);
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void setupLighting(){
	GLfloat light_position1[] = { 0.0, 0.0, 10.0, 1.0 };
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 0.0f};
	// GLfloat light_position2[] = { 10.0, 10.0, -15.0, 0.0 };
	GLfloat light_position2[] = { 3.0, 3.0, -3.0, 0.0 };
	GLfloat color[] = {0.0f, 0.2f, 1.0f, 0.0f};
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 40.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	// glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
}

void setup() {
	texture = loadTexture("example/texture.raw", 32, 32);
	spectrum = loadTexture("example/spectrum.raw", 128, 64);
	setMat4Identity(matrix);
	// setupLighting();
	polarPerspective(0, 0, 0);
	lookOrientation[1] = 78;
}
void update() { 
	static float ROT_SPEED = 0.001;
	float rot1[16];
	float rot2[16];
	makeMat4XRot(rot1, originY * ROT_SPEED);
	makeMat4YRot(rot2, originX * ROT_SPEED);
	mat4x4Mult(rot1, rot2, matrix);
	if(PERSPECTIVE == POLAR)
		lookOrientation[0] = frameNum * 0.1;
}
void draw3D() {
	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glColor3f(1.0, 1.0, 1.0);
	if(GRID){
		label3DAxes(5);
		glPushMatrix();
			glScalef(5, 5, 5);
			drawUVSphereLines();
		glPopMatrix();
	}

	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(10, 10, 10);
		renderStars();
	glPopMatrix();

	glPushMatrix();
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		float brightness = 1.0;
		glColor4f(1.0, 1.0, 1.0, brightness);
		glTranslatef(0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, spectrum);
		drawSphere(0, 0, 0, 0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
		glDisable(GL_CULL_FACE);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(4,4,1);
		drawPlatonicSolidLines(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,4,1);
		drawPlatonicSolidLines(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,-4,1);
		drawPlatonicSolidLines(3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(4,-4,1);
		drawPlatonicSolidLines(4);
	glPopMatrix();
}
void draw2D() {
	if(showOverlay){
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texture);
			glScalef(HEIGHT, HEIGHT, HEIGHT);
			drawUnitSquare((WIDTH*0.5 - HEIGHT*0.5)/((float)HEIGHT), 0, 0);
			glBindTexture (GL_TEXTURE_2D, 0);
		glPopMatrix();
		glDisable(GL_BLEND);
	}
	switch(PERSPECTIVE){
		case FPP: {
			char locationString[50];
			sprintf(locationString, "%d, %d, %d", (int)originX, (int)originY, (int)originZ );
			text(locationString, 4, 18, 0);
		}
		break;
		case POLAR:{
			char orientationString[50];
			sprintf(orientationString, "%d, %d, %d", (int)lookOrientation[0], (int)lookOrientation[1], (int)lookOrientation[2] );
			text(orientationString, 4, 18, 0);
		}
		break;
		case ORTHO:{
			char frameString[50];
			sprintf(frameString, "%d, %d, %d, %d", (int)orthoFrame[0], (int)orthoFrame[1], (int)orthoFrame[2], (int)orthoFrame[3] );
			text(frameString, 4, 18, 0);
		}
		break;
	}
}
void keyDown(unsigned int key) { 
	if(key == ' '){
		showOverlay = !showOverlay;
	}
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }