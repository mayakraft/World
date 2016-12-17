#ifdef OS_WINDOWS
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/wglew.c"
#else
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/glew.c"
#endif

#include "../world.h"

#include "data/518stars.c"
#include "data/1619stars.c"

unsigned char showSphere = 1;

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

GLuint shader = 0;
GLuint shader2 = 0;
GLuint spectrum;

char *vertexPath1 = "shaders/simple.vert";
char *fragmentPath1 = "shaders/fog.frag";
char *vertexPath2 = "shaders/wobble.vert";
char *fragmentPath2 = "shaders/worley.frag";

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
	// setupLighting();
	shader = loadShader(  vertexPath1, fragmentPath1 );
	shader2 = loadShader( vertexPath2, fragmentPath2 );
	GROUND = 0;
	// GRID = 0;
	polarPerspective(0, 0, 0);
	lookOrientation[1] = 78;
	spectrum = loadTexture("data/spectrum.raw", 128, 64);
}
void update() {
	if(frameNum%60 == 0){ 
		shader = loadShader(  vertexPath1, fragmentPath1 );
		shader2 = loadShader( vertexPath2, fragmentPath2 );
	}

	float frame[2] = {WIDTH, HEIGHT};
	setShaderUniform1f(shader, "u_time", frameNum/60.0);
	setShaderUniform1f(shader2, "u_time", frameNum/60.0);
	setShaderUniformVec2f(shader, "u_resolution", frame);
	setShaderUniformVec2f(shader2, "u_resolution", frame);
	// if(PERSPECTIVE == POLAR)
	// 	lookOrientation[0] = frameNum * 0.1;
}
// void draw3D() {
	// glUseProgram(shader);
	// drawRect(-5,-5, 0, 10, 10);
	// glUseProgram(0);
// }
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

	if(showSphere){
	glPushMatrix();
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		float brightness = 1.0;
		glColor4f(1.0, 1.0, 1.0, brightness);
		glTranslatef(0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, spectrum);
		glScalef(-1.0, 1.0, -1.0);
		drawSphere(0, 0, 0, 0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
		glDisable(GL_CULL_FACE);
	glPopMatrix();
	}

	glUseProgram(shader2);

	glPushMatrix();
		glRotatef(frameNum*0.5, 0,1,0);
		float sqW = 6;
		drawRect(-sqW*0.5, -sqW*0.5, -0.01, sqW, sqW);
	glPopMatrix();


	glUseProgram(shader);

	glPushMatrix();
		glScalef(10, 10, 10);
		renderStars();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(4,4,1);
		drawPlatonicSolidFaces(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,4,1);
		drawPlatonicSolidFaces(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,-4,1);
		drawPlatonicSolidFaces(3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(4,-4,1);
		drawPlatonicSolidFaces(4);
	glPopMatrix();
	glUseProgram(0);

glLineWidth(2);
	glPushMatrix();
		glTranslatef(4,4,1);
		glScalef(1.001, 1.001, 1.001);
		drawPlatonicSolidLines(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,4,1);
		glScalef(1.001, 1.001, 1.001);
		drawPlatonicSolidLines(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-4,-4,1);
		glScalef(1.001, 1.001, 1.001);
		drawPlatonicSolidLines(3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(4,-4,1);
		glScalef(1.001, 1.001, 1.001);
		drawPlatonicSolidLines(4);
	glPopMatrix();
glLineWidth(1);

}
void draw2D() { }
void keyDown(unsigned int key) { 
	if(key == 'o' || key == 'O') 
		showSphere = !showSphere;
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }