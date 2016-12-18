#ifdef OS_WINDOWS
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/wglew.c"
#else
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/glew.c"
#endif

#include "../world.h"

#include "../examples/data/518stars.c"
#include "../examples/data/1619stars.c"

unsigned char showSphere = 1;

const unsigned int numPoly = 50;
float poly[numPoly * 3];

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

char *vertexPath1 = "../examples/shaders/simple.vert";
char *fragmentPath1 = "../examples/shaders/fog.frag";
char *vertexPath2 = "../examples/shaders/wobble.vert";
char *fragmentPath2 = "../examples/shaders/water.frag";

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
	lookOrientation[2] = 5*1.414;
	spectrum = loadTexture("../examples/data/spectrum.raw", 128, 64);

	int range = 10;
	for(int i = 0; i < numPoly; i++){
		poly[i*3+0] = random()%range - range*0.5;
		poly[i*3+1] = random()%range - range*0.5;
		poly[i*3+2] = random()%range - range*0.5;
	}
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
void draw3D() {
	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glColor3f(1.0, 1.0, 1.0);
	if(GRID){
		label3DAxes(5);
		glPushMatrix();
			glScalef(5, 5, 5);
			glColor4f(1.0, 1.0, 1.0, 0.33);
			drawUVSphereLines();
		glPopMatrix();
	}

	if(showSphere){
		glPushMatrix();
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			float brightness = 1.0;
			glColor4f(1.0, 1.0, 1.0, brightness);
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
		float sqW = 16;
		drawRect(-sqW*0.5, -sqW*0.5, -0.01, sqW, sqW);
	glPopMatrix();

	glUseProgram(shader);

	glPushMatrix();
		glScalef(10, 10, 10);
		renderStars();
	glPopMatrix();

	for(int i = 0; i < numPoly; i++){
		glPushMatrix();
			glTranslatef(poly[i*3+0], poly[i*3+1], poly[i*3+2] );
			drawPlatonicSolidFaces(3);
		glPopMatrix();
	}
	glUseProgram(0);

	glLineWidth(2);
	for(int i = 0; i < numPoly; i++){
		glPushMatrix();
			glTranslatef(poly[i*3+0], poly[i*3+1], poly[i*3+2] );
			glScalef(1.001, 1.001, 1.001);
			drawPlatonicSolidLines(3);
		glPopMatrix();
	}
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