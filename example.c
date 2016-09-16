#include "world.h"

GLuint texture, spectrum;
float matrix[16];
unsigned char showOverlay = 0;

#include "example/518stars.c"
#include "example/1619stars.c"
void renderStars(){
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _518_stars);
		glDrawArrays(GL_POINTS, 0, 518);
		glColor3f(0.5, 0.5, 0.5);
		glVertexPointer(3, GL_FLOAT, 0, _1619_stars);
		glDrawArrays(GL_POINTS, 0, 1619);
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void renderCelestialLines(){
	float a1 = 0.33;
	float a2 = 0.166;
	glPushMatrix();
		// equator
		glColor4f(1.0, 1.0, 1.0, a1);
			drawUnitCircle(0, 0, 0);
		// latitude
		glColor4f(1.0, 1.0, 1.0, a2);
		for(float pos = 1.0/3; pos < 1.0; pos += 1.0/3){
			glPushMatrix();
				float r = cosf(pos*M_PI*0.5);
				r = sqrt(1 - powf(pos,2));
				glScalef(r, r, 1.0);
					drawUnitCircle(0, 0, -pos);
					drawUnitCircle(0, 0, pos);
			glPopMatrix();
		}
		// longitude
		glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(90, 0, 1, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
	glPopMatrix();	
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

void setup() {
	texture = loadTexture("example/texture.raw", 32, 32);
	spectrum = loadTexture("example/spectrum.raw", 128, 64);
	setMat4Identity(matrix);
}
void update() { 
	static float ROT_SPEED = 0.001;
	float rot1[16];
	float rot2[16];
	makeMat4XRot(rot1, originY * ROT_SPEED);
	makeMat4YRot(rot2, originX * ROT_SPEED);
	mat4x4Mult(rot1, rot2, matrix);
}
void draw3D() {
	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glPushMatrix();
		glMultMatrixf(matrix);
		glScalef(10, 10, 10);
		renderStars();
		glPushMatrix();
			float brightness = powf(sinf(frameNum*0.003), 12);
			glColor4f(1.0, 1.0, 1.0, brightness);
			glScalef(2, 2, 2);
			glBindTexture(GL_TEXTURE_2D, spectrum);
			drawUnitSphere(0, 0, 0, 2.0);
			glBindTexture (GL_TEXTURE_2D, 0);
		glPopMatrix();
		glPushMatrix();
			// glScalef(100, 100, 100);
			renderCelestialLines();
		glPopMatrix();
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	text("(10, 0, 0)", 10, 0, 0);
	text("(0, 10, 0)", 0, 10, 0);
	text("(0, 0, 10)", 0, 0, 10);
	text("(-10, 0, 0)", -10, 0, 0);
	text("(0, -10, 0)", 0, -10, 0);
	text("(0, 0, -10)", 0, 0, -10);
}
void draw2D() {
	if(showOverlay){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texture);
			glScalef(HEIGHT, HEIGHT, HEIGHT);
			drawUnitSquare((WIDTH*0.5 - HEIGHT*0.5)/((float)HEIGHT), 0, 0);
			glBindTexture (GL_TEXTURE_2D, 0);
		glPopMatrix();
		glDisable(GL_BLEND);
	}
	glColor3f(1.0, 1.0, 1.0);
	char locationString[50];
	sprintf(locationString, "%d, %d, %d", (int)originX, (int)originY, (int)originZ );
	text(locationString, 4, 18, 0);
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