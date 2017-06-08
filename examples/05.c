// example 5
//
// textures and shaders

#ifdef OS_WINDOWS
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/wglew.c"
#else
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/glew.c"
#endif

#include "../world.h"

const unsigned int numPoly = 50;
float poly[numPoly * 3];

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
	GRID = 0;
	polarPerspective();
	horizon[1] = 12;
	horizon[2] = 10;
	spectrum = loadTexture("../examples/data/spectrum.raw", 128, 64);

	int range = 10;
	for(int i = 0; i < numPoly; i++){
		poly[i*3+0] = random()%range - range*0.5;
		poly[i*3+1] = random()%range - range*0.5;
		poly[i*3+2] = random()%range - range*0.5;
	}
}
void update() {
	if(frame%60 == 0){ 
		shader = loadShader(  vertexPath1, fragmentPath1 );
		shader2 = loadShader( vertexPath2, fragmentPath2 );
	}
	float rect[2] = {WIDTH, HEIGHT};
	setShaderUniform1f(shader, "u_time", elapsed);
	setShaderUniform1f(shader2, "u_time", elapsed);
	setShaderUniformVec2f(shader, "u_resolution", rect);
	setShaderUniformVec2f(shader2, "u_resolution", rect);
}
void draw3D() {
	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glPushMatrix();
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		float brightness = -cos(elapsed*0.5)*0.5+0.5;
		glColor4f(1.0, 1.0, 1.0, brightness);
		glBindTexture(GL_TEXTURE_2D, spectrum);
		// glScalef(-1.0, 1.0, -1.0);
		glScalef(100, 100, 100);
		drawSphere(0, 0, 0, 0.5);
		glBindTexture (GL_TEXTURE_2D, 0);
		glDisable(GL_CULL_FACE);
	glPopMatrix();

	glUseProgram(shader2);

	glPushMatrix();
		glRotatef(frame*0.5, 0,1,0);
		float sqW = 16;
		drawRect(-sqW*0.5, -sqW*0.5, -0.01, sqW, sqW);
	glPopMatrix();

	glUseProgram(shader);
	for(int i = 0; i < numPoly; i++){
		glPushMatrix();
			glTranslatef(poly[i*3+0], poly[i*3+1], poly[i*3+2] );
			drawPlatonicSolidFaces(3);
		glPopMatrix();
	}
	glUseProgram(0);

	glColor4f(0.7, 0.7, 0.7, (-cos(elapsed)*0.5+0.5) );
	glLineWidth(1 + 6*(cos(elapsed)*0.5+0.5) );
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
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }