#include "world.h"

#include "518stars.c"
#include "1619stars.c"

GLuint texture;
float matrix[16];
unsigned char showOverlay = 0;

void renderStars(){
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _star_vertices_518);
	glDrawArrays(GL_POINTS, 0, 518);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor3f(0.5, 0.5, 0.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _star_vertices_1619);
	glDrawArrays(GL_POINTS, 0, 1619);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void setup() {
	texture = loadTexture("texture.raw", 32, 32);
	setMat4Identity(matrix);
}
void update() { 
	float rot1[16];
	float rot2[16];
	float rot[16];
	float newMatrix[16];	
	makeMat4XRot(rot1, originDY * 0.005);
	makeMat4YRot(rot2, originDX * 0.005);
	mat4x4Mult(rot1, rot2, rot);
	mat4x4Mult(matrix, rot, newMatrix);
	memcpy(matrix, newMatrix, sizeof(float)*16);
}
void draw3D() {
	// glTranslatef(originX, originY, originZ + newOriginZ);		
	glMultMatrixf(matrix);
	glPushMatrix();
		glScalef(10, 10, 10);
		renderStars();
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
}
void draw2D() {
	if(showOverlay){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texture);
			glScalef(HEIGHT, HEIGHT, HEIGHT);
			drawUnitSquare((WIDTH*0.5 - HEIGHT*0.5)/((float)HEIGHT), 0);
			glBindTexture (GL_TEXTURE_2D, 0);
		glPopMatrix();

		glDisable(GL_BLEND);		
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