#ifdef OS_WINDOWS
#  include "lib/glew-2.0.0/include/GL/glew.h"
#  include "lib/glew-2.0.0/src/wglew.c"
#else
#  include "lib/glew-2.0.0/include/GL/glew.h"
#  include "lib/glew-2.0.0/src/glew.c"
#endif

#include "world.h"

#include "example/518stars.c"
#include "example/1619stars.c"

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

///////////////////////////////////////
//////////      SHADERS      //////////
///////////////////////////////////////
char *readFile(char *filename){
	char *buffer = 0;
	long length;
	FILE *f = fopen(filename, "rb");
	if(f){
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length);
		if(buffer) fread(buffer, 1, length, f);
		fclose(f);
		buffer[length] = 0; // fixes occasional extra characters at end of buffer
	}
	return buffer;
}
GLuint loadShader(char *vertex_path, char *fragment_path) {
	//  http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
	GLenum err = glewInit();
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Read shaders
	char *vertShaderSrc = readFile(vertex_path);
	char *fragShaderSrc = readFile(fragment_path);
	GLint result = GL_FALSE;
	int logLength;
	// Compile vertex shader
	glShaderSource(vertShader, 1, (const char *const *)&vertShaderSrc, NULL);
	glCompileShader(vertShader);
	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char vertShaderError[logLength]; vertShaderError[0] = 0;
		glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
		printf("VERTEX SHADER COMPILE %s", &vertShaderError[0]);
	}
	// Compile fragment shader
	glShaderSource(fragShader, 1, (const char *const *)&fragShaderSrc, NULL);
	glCompileShader(fragShader);
	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char fragShaderError[logLength]; fragShaderError[0] = 0;
		glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
		printf("FRAGMENT SHADER COMPILE %s", &fragShaderError[0]);
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char programError[logLength]; programError[0] = 0;
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		printf("LINKER %s", &programError[0]);
	}
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return program;
}
void setShaderUniform1f(GLuint shader, char *uniform, float value){
	if(shader){
		glUseProgram(shader);
		GLint loc = glGetUniformLocation(shader, uniform);
		if(loc != -1) glUniform1f(loc, value);
		glUseProgram(0);
	}
}
void setShaderUniformVec3f(GLuint shader, char *uniform, float *array){
	if(shader){
		glUseProgram(shader);
		GLint loc = glGetUniformLocation(shader, uniform);
		if(loc != -1) glUniform3fv(loc, 1, &array[0]);
		glUseProgram(0);
	}
}
void setShaderUniformVec4f(GLuint shader, char *uniform, float *array){
	if(shader){
		glUseProgram(shader);
		GLint loc = glGetUniformLocation(shader, uniform);
		if(loc != -1) glUniform4fv(loc, 1, &array[0]);
		glUseProgram(0);
	}
}
///////////////////////////////////////
////////      END SHADERS      ////////
///////////////////////////////////////

GLuint shader = 0;
GLuint shader2 = 0;
GLuint spectrum;

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
	shader = loadShader( "example/shaders/simple.vert", "example/shaders/fog.frag" );
	shader2 = loadShader( "example/shaders/simple.vert", "example/shaders/metaballs.frag" );
	GROUND = 0;
	// GRID = 0;
	polarPerspective(0, 0, 0);
	lookOrientation[1] = 78;
	spectrum = loadTexture("example/spectrum.raw", 128, 64);
}
void update() {
	setShaderUniform1f(shader, "u_time", frameNum/60.0);
	setShaderUniform1f(shader2, "u_time", frameNum/60.0);
	if(PERSPECTIVE == POLAR)
		lookOrientation[0] = frameNum * 0.1;
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
		drawRect(-4, -4, -0.01, 8, 8);
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