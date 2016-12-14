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
char *loadFile(char *filename){
	char *buffer = 0;
	long length;
	FILE *f = fopen(filename, "rb");
	if(f){
		fseek(f, 0, SEEK_END);
		length = ftell (f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length);
		if(buffer) fread(buffer, 1, length, f);
		fclose(f);
	}
	return buffer;
}
GLhandleARB loadShaders(char *vertexFilePath, char *fragmentFilePath){
	GLhandleARB program = 0;
	GLuint shader_num_v, shader_num_f;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		printf("ERROR %s\n", glewGetErrorString(err));
		exit(1);
	}
	if (GLEW_ARB_vertex_program){
		glEnable(GL_VERTEX_PROGRAM_ARB);
		glGenProgramsARB(1, &shader_num_v);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, shader_num_v);
	}
	else{  printf("ERROR loading vertex shader\n");	}
	if (GLEW_ARB_fragment_program){
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		glGenProgramsARB(1, &shader_num_f);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, shader_num_f);
	}
	else{  printf("ERROR loading fragment shader\n");	}
	GLint vlength, flength;
	GLint vertexShaderObject, fragmentShaderObject;
	const char *VertexShaderSource = loadFile( vertexFilePath );
	const char *FragmentShaderSource = loadFile( fragmentFilePath );
	if((int)(VertexShaderSource == NULL)){ printf("ERROR loading vertex shader file\n"); return 0; }
	if((int)(FragmentShaderSource == NULL)){ printf("ERROR loading fragment shader file\n"); return 0; }
	vlength = strlen(VertexShaderSource);
	flength = strlen(FragmentShaderSource);
	vertexShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	glShaderSourceARB(vertexShaderObject, 1, &VertexShaderSource, &vlength);
	glShaderSourceARB(fragmentShaderObject, 1, &FragmentShaderSource, &flength);
	glCompileShaderARB(vertexShaderObject);
	glCompileShaderARB(fragmentShaderObject);
	GLint compiled;
	glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &compiled);
	if(!compiled){  printf("ERROR compiling vertex shader\n");  }
	glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if(!compiled){  printf("ERROR compiling fragment shader\n");  }	
	program = glCreateProgramObjectARB();
	glAttachObjectARB(program, fragmentShaderObject);
	glAttachObjectARB(program, vertexShaderObject);
	glLinkProgramARB(program);
	glUseProgramObjectARB(program);
	return program;
}
void setShaderUniform1f(GLhandleARB shader, char *uniform, float value){
	if(shader){
		glUseProgramObjectARB(shader);
		GLint loc = glGetUniformLocation(shader, uniform);
		if(loc != -1) glUniform1f(loc, value);
		glUseProgramObjectARB(0);
	}
}
///////////////////////////////////////
////////      END SHADERS      ////////
///////////////////////////////////////

GLhandleARB shader = 0;

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
	shader = loadShaders( "example/shader.vert", "example/shader.frag" );
	// GROUND = 0;
	PERSPECTIVE = POLAR;
}
void update() {
	setShaderUniform1f(shader, "time", frameNum/60.0);
}
void draw3D() {
	glUseProgramObjectARB(shader);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glEnable(GL_VERTEX_PROGRAM_ARB);
	drawSphere(0, 0, 1, 0.5);
	glUseProgramObjectARB(0);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);
}
void draw2D() { }
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }