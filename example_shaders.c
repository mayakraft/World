#ifdef OS_WINDOWS
#  include "lib/glew-2.0.0/include/GL/glew.h"
#  include "lib/glew-2.0.0/src/wglew.c"
#else
#  include "lib/glew-2.0.0/include/GL/glew.h"
#  include "lib/glew-2.0.0/src/glew.c"
#endif

#include "world.h"

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
	setupLighting();
	shader = loadShader( "example/fog.vert", "example/fog.frag" );
	GROUND = 0;
	GRID = 0;
	PERSPECTIVE = POLAR;
}
void update() {
	setShaderUniform1f(shader, "u_time", frameNum/60.0);
}
void draw3D() {
	glUseProgram(shader);
	drawRect(-5,-5, 0, 10, 10);
	glUseProgram(0);
}
void draw2D() { }
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }