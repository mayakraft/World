
#include "../world.h"

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

// walking will rotate the celestial sphere
float starMatrix[16];

void setup(){
	OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_MOVE | SET_KEYBOARD_FUNCTIONS | SET_SHOW_GROUND;
	setMat4Identity(starMatrix);
}
void update(){ 
	static float ROT_SPEED = 0.04;
	float mX[16], mY[16];
	makeMat4XRot(mX, origin[1] * ROT_SPEED);
	makeMat4YRot(mY, origin[0] * ROT_SPEED);
	mat4x4Mult(mX, mY, starMatrix);
}
void draw3D(){ 
	// celestial sphere
	glPushMatrix();
		glScalef(100, 100, 100);
		glColor4f(1.0, 1.0, 1.0, 0.1);
		drawUVSphereLines();
	glPopMatrix();
	// stars
	glPushMatrix();
		glMultMatrixf(starMatrix);
		glScalef(10, 10, 10);
		renderStars();
	glPopMatrix();
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }