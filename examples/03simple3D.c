#include "../world.h"

GLuint texture, spectrum;
float matrix[16];
unsigned char showOverlay = 0;

#include "../examples/data/518stars.c"
#include "../examples/data/1619stars.c"

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
	glEnable(GL_LIGHTING);
}

void setup() {
	// glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	texture = loadTexture("../examples/data/texture.raw", 32, 32);
	spectrum = loadTextureSmooth("../examples/data/spectrum.raw", 128, 64);
	setMat4Identity(matrix);
	setupLighting();
	polarPerspective(0, 0, 0);
	lookOrientation[1] = 78;
	lookOrientation[2] = 5*1.414;
}
void update() { 
	static float ROT_SPEED = 0.001;
	float rot1[16];
	float rot2[16];
	makeMat4XRot(rot1, originY * ROT_SPEED);
	makeMat4YRot(rot2, originX * ROT_SPEED);
	mat4x4Mult(rot1, rot2, matrix);
	// if(PERSPECTIVE == POLAR)
	// 	lookOrientation[0] = frameNum * 0.1;
}
void draw3D() {
	glDisable(GL_LIGHTING);

	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glColor3f(1.0, 1.0, 1.0);
	if(GRID){
		glColor4f(1.0, 1.0, 1.0, 1.0);
		label3DAxes(5);
		glPushMatrix();
			glScalef(100, 100, 100);
			glColor4f(1.0, 1.0, 1.0, 0.33);
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
		glTranslatef(0, 0, 1.5 + 1.0 * sinf(frameNum*0.01) );
		glBindTexture(GL_TEXTURE_2D, spectrum);
		glScalef(-1.0, 1.0, -1.0);
		drawSphere(0, 0, 0, 0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
		glDisable(GL_CULL_FACE);
	glPopMatrix();

	// glPushMatrix();
	// 	glTranslatef(4,4,1);
	// 	drawPlatonicSolidLines(0);
	// glPopMatrix();
	// glPushMatrix();
	// 	glTranslatef(-4,4,1);
	// 	drawPlatonicSolidLines(1);
	// glPopMatrix();
	// glPushMatrix();
	// 	glTranslatef(-4,-4,1);
	// 	drawPlatonicSolidLines(3);
	// glPopMatrix();
	// glPushMatrix();
	// 	glTranslatef(4,-4,1);
	// 	drawPlatonicSolidLines(4);
	// glPopMatrix();
		glEnable(GL_LIGHTING);

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
	worldInfoText(0, 10, 0);

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