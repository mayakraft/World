#ifndef WORLD_FRAMEWORK
#define WORLD_FRAMEWORK

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////////////
//      WORLD is a hyper minimalist framework for graphics (OpenGL) 
//   and user input (keyboard, mouse) in the Processing design paradigm
///////////////////////////////////////////////////////////////////////////////////////
//
//   HOW TO USE
//
//   1) make an empty .c file
//   2) #include "world.h"
//   3) implement the following functions:
//      done! build with makefile: 'make', 'make run'
//
void setup();
void update();
void draw3D();
void draw2D();
void keyDown(unsigned int key);
void keyUp(unsigned int key);
void mouseDown(unsigned int button);
void mouseUp(unsigned int button);
void mouseMoved(int x, int y);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// CUSTOMIZE SETTINGS
enum{
	SET_MOUSE_LOOK = 1 << 0,
	SET_KEYBOARD_MOVE = 1 << 1,
	SET_KEYBOARD_FUNCTIONS = 1 << 2,
	SET_SHOW_GROUND = 1 << 3,  // a 2D grid
	SET_SHOW_GRID = 1 << 4    // a 3D grid
};
enum { BIT_MOUSE_LOOK, BIT_KEYBOARD_MOVE, BIT_KEYBOARD_FUNCTIONS, BIT_SHOW_GROUND, BIT_SHOW_GRID };
static unsigned char ADVANCED = 0;
static unsigned char BEGINNER = 255; // BEGINNER (default) hooks helpful keyboard and visual feedback
static unsigned char OPTIONS = 255;
#define CONTINUOUS_REFRESH 1  // (0) = maximum efficiency, only redraws screen if received input
static float MOUSE_SENSITIVITY = 0.333f;
static float WALK_INTERVAL = 0.077f;  // WALKING SPEED. @ 60 UPS (updates/sec), walk speed (units/sec) = INTERVAL * UPS
static float ZOOM_SPEED = 0.05f;
// WINDOW size upon boot
static int WIDTH = 800;  // (readonly) set these values here
static int HEIGHT = 600; // (readonly) setting during runtime will not re-size window
static unsigned char FULLSCREEN = 0;  // fullscreen:1   window:0
// INPUT
static int mouseX = 0;  // get mouse location at any point, units in pixels
static int mouseY = 0;
static int mouseDragX = 0;  // dragging during one session (between click and release)
static int mouseDragY = 0;
static unsigned char keyboard[256];  // query this at any point for the state of a key (0:up, 1:pressed)
// GRAPHICS
static float NEAR_CLIP = 0.1;
static float FAR_CLIP = 10000.0;
static float FOV = 0.1;
static float ZOOM_RADIX = 3;
// PERSPECTIVE
enum{  FPP,  POLAR,  ORTHO  }; // first persion, polar, orthographic
static unsigned char PERSPECTIVE = FPP;  // initialize point of view in this state
// details of each perspective
float origin[3] = {0.0f, 0.0f, 0.0f};  // x, y, z, location of the eye
float horizon[3] = {0.0f, 0.0f, 7.0f};   // azimuth, altitude, zoom (log)
float orthoFrame[4] = {0.0f, 0.0f, 4.0f, 3.0f}; // x, y, width, height
static float FPP_BODY_HEIGHT = 1.0;  // height of the person off of the ground
// TIME
static unsigned long frame;  // # times the screen has drawn 
struct timespec startTime, currentTime;
static float elapsed;  // elapsed time in seconds, includes fractional part

// TABLE OF CONTENTS:
int main(int argc, char **argv);  // initialize Open GL context
void typicalOpenGLSettings();  // colors, line width, glEnable
void reshapeWindow(int windowWidth, int windowHeight);  // contains viewport and frustum calls
void rebuildProjection();  // calls one of the three functions below
void toggleFullscreen();
// CHANGE PERSPECTIVE
void firstPersonPerspective();
void polarPerspective();
void orthoPerspective(float x, float y, float width, float height);
// DRAW, ALIGNMENT, INPUT HANDLING
void display();
void updateWorld();  // process input devices
// INPUT DEVICES
void moveOriginWithArrowKeys();
void mouseButtons(int button, int state, int x, int y);  // when mouse button state changes
void mouseMotion(int x, int y);   // when mouse is dragging screen
void mousePassiveMotion(int x, int y);  // when mouse is moving but not pressed
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key,int x,int y);
void specialDown(int key, int x, int y);
void specialUp(int key, int x, int y);
void keyboardSetIdleFunc();
// TINY OPENGL TOOLBOX: all 2D shapes are in the X Y plane, normal along the Z
void text(const char *text, float x, float y, float z);
void drawPoint(float x, float y, float z);
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
void drawUnitOriginSquare();
void drawRect(float x, float y, float z, float width, float height);
void drawUnitSquare(float x, float y, float z);
void drawUnitOriginSphere();
void drawUnitSphere(float x, float y, float z);
void drawSphere(float x, float y, float z, float radius);
void drawUnitOriginCircle();
void drawUnitCircle(float x, float y, float z);
void drawCircle(float x, float y, float z, float radius);
void draw3DAxesLines(float x, float y, float z, float scale);
void drawUVSphereLines();
void drawPlatonicSolidFaces(char solidType);
void drawPlatonicSolidLines(char solidType);
void drawPlatonicSolidPoints(char solidType);
// combinations of shapes
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats);
float modulusContext(float complete, int modulus);
float min(float one, float two);
float max(float one, float two);
// TEXTURES, SHADERS
GLuint loadTexture(const char * filename, int width, int height);
GLuint loadShader(char *vertex_path, char *fragment_path);
void setShaderUniform1f(GLuint shader, char *uniform, float value);
void setShaderUniformVec2f(GLuint shader, char *uniform, float *array);
void setShaderUniformVec3f(GLuint shader, char *uniform, float *array);
void setShaderUniformVec4f(GLuint shader, char *uniform, float *array);
// preload for geometry primitives
void initPrimitives();
GLint _sphere_stacks = 20; //7;
GLint _sphere_slices = 30; //13;
static float _unit_circle_vertices[192];
static float _unit_circle_normals[192];
static float _unit_circle_texCoord[192];
float *_unit_sphere_vertices, *_unit_sphere_normals, *_unit_sphere_texture;

#define ESCAPE_KEY 27
#define SPACE_BAR 32
#define RETURN_KEY 13
#define DELETE_KEY 127
#define EQUAL_KEY 61
#define PLUS_KEY 43
#define MINUS_KEY 45
#define UNDERBAR_KEY 95
#define PERIOD_KEY 46
#define GREATER_THAN_KEY 62
#define COMMA_KEY 44
#define LESS_THAN_KEY 60
// special key codes conflict with 0-127 ASCII codes, augmented them to 128-255 index range
#define UP_KEY GLUT_KEY_UP+128//229
#define DOWN_KEY GLUT_KEY_DOWN+128//231
#define RIGHT_KEY GLUT_KEY_RIGHT+128//230
#define LEFT_KEY GLUT_KEY_LEFT+128//228
int main(int argc, char **argv){
	// initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow(argv[0]);
	// tie this program's functions to glut
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);
	glutMouseFunc(mouseButtons);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutKeyboardUpFunc(keyboardUp);
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialDown);
	glutSpecialUpFunc(specialUp);
	if(CONTINUOUS_REFRESH){
		glutIdleFunc(updateWorld); }
	// setup this program
	orthoFrame[0] = -WIDTH*0.5;
	orthoFrame[1] = -HEIGHT*0.5;
	orthoFrame[2] = WIDTH;
	orthoFrame[3] = HEIGHT;

	memset(keyboard,0,256);
	clock_gettime(CLOCK_MONOTONIC, &startTime);
	frame = 0;
	initPrimitives();
	time_t t;
	srand((unsigned) time(&t));
	typicalOpenGLSettings();
	updateWorld();  // update must be called before draw, incl first draw call
	glutPostRedisplay();
	setup();  // user defined function
	// begin main loop
	glutMainLoop();
	return 0;
}
void typicalOpenGLSettings(){
	firstPersonPerspective();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1);
}
void reshapeWindow(int windowWidth, int windowHeight){
	WIDTH = windowWidth;
	HEIGHT = windowHeight;
	glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
	// this overwrites the user's settings
	// orthoFrame[2] = WIDTH;
	// orthoFrame[3] = HEIGHT;
	rebuildProjection();
	updateWorld();
}
void rebuildProjection(){
	switch(PERSPECTIVE){
		case FPP:
			firstPersonPerspective(); break;
		case POLAR:
			polarPerspective(); break;
		case ORTHO:
			orthoPerspective(orthoFrame[0], orthoFrame[1], orthoFrame[2], orthoFrame[3]); break;
	}
}
void toggleFullscreen(){
	if(!FULLSCREEN)
		glutFullScreen();
	else{
		reshapeWindow(WIDTH, HEIGHT);
		glutPositionWindow(0,0);
	}
	FULLSCREEN = !FULLSCREEN;
}
void firstPersonPerspective(){
	PERSPECTIVE = FPP;
	float a = (float)min(WIDTH, HEIGHT) / max(WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(WIDTH < HEIGHT) glFrustum (-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP);
	else               glFrustum (-FOV/a, FOV/a, -FOV, FOV, NEAR_CLIP, FAR_CLIP);
	// change POV
	glRotatef(-90-horizon[1], 1, 0, 0);
	glRotatef(90+horizon[0], 0, 0, 1);
	// raise POV 1.0 above the floor, 1.0 is an arbitrary value
	glTranslatef(0.0f, 0.0f, -FPP_BODY_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}
void polarPerspective(){
	PERSPECTIVE = POLAR;
	float a = (float)min(WIDTH, HEIGHT) / max(WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(WIDTH < HEIGHT) glFrustum (-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP);
	else               glFrustum (-FOV/a, FOV/a, -FOV, FOV, NEAR_CLIP, FAR_CLIP);
	// change POV
	glTranslatef(0, 0, -horizon[2]);
	glRotatef(-90+horizon[1], 1, 0, 0);
	glRotatef(90+180+horizon[0], 0, 0, 1);
	// glTranslatef(x, y, z);
	glMatrixMode(GL_MODELVIEW);
}
void orthoPerspective(float x, float y, float width, float height){
	PERSPECTIVE = ORTHO;
	orthoFrame[0] = x;
	orthoFrame[1] = y;
	orthoFrame[2] = width;
	orthoFrame[3] = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, width + x, height + y, y, -FAR_CLIP, FAR_CLIP);
	glMatrixMode(GL_MODELVIEW);
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glPushMatrix();
			glColor4f(1.0, 1.0, 1.0, 1.0);
			if(OPTIONS & (1 << BIT_KEYBOARD_MOVE)){ glTranslatef(-origin[0], -origin[1], -origin[2]); }
			draw3D();
		glPopMatrix();
		// 3D REPEATED STRUCTURE
		if(OPTIONS & (1 << BIT_SHOW_GRID)){
			float newX = modulusContext(-origin[0], 5);
			float newY = modulusContext(-origin[1], 5);
			float newZ = modulusContext(-origin[2], 5);
			glPushMatrix();
				glTranslatef(newX, newY, newZ);
				drawAxesGrid(newX, newY, newZ, 5, 4);
			glPopMatrix();
		}
		// 2D REPEATED STRUCTURE
		if(OPTIONS & (1 << BIT_SHOW_GROUND)){
			float newX = modulusContext(-origin[0], 2);
			float newY = modulusContext(-origin[1], 2);
			glPushMatrix();
				glTranslatef(newX, newY, -origin[2]);
				drawCheckerboard(newX, newY, 8);
			glPopMatrix();
		}
	glPopMatrix();
	
	// TO ORTHOGRAPHIC
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glColor4f(1.0, 1.0, 1.0, 1.0);
		draw2D();
	glPopMatrix();
	rebuildProjection();

	// bring back buffer to the front on vertical refresh, auto-calls glFlush
	glutSwapBuffers();
	// glFlush();
}
void updateWorld(){
	frame += 1;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	elapsed = (currentTime.tv_sec - startTime.tv_sec);
	elapsed += (currentTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;
	// keyboard input
	if(OPTIONS & (1 << BIT_KEYBOARD_MOVE)){
		moveOriginWithArrowKeys();
		if(keyboard[MINUS_KEY]){
			horizon[2] += ZOOM_SPEED;
			rebuildProjection();
		}
		if(keyboard[PLUS_KEY]){
			horizon[2] -= ZOOM_SPEED;
			if(horizon[2] < 0)
				horizon[2] = 0;
			rebuildProjection();
		}		
	}
	update();
	glutPostRedisplay();
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
		buffer = malloc(length + 1);
		if(buffer) fread(buffer, 1, length, f);
		fclose(f);
		buffer[length] = 0; // fixes occasional extra characters at end of buffer
	}
	return buffer;
}
#ifdef __glew_h__
GLuint loadShader(char *vertex_path, char *fragment_path) {
	GLenum err = glewInit();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char *vSource = readFile(vertex_path);
	char *fSource = readFile(fragment_path);
	GLint result = GL_FALSE;
	int logLength;
	// vertex
	glShaderSource(vertexShader, 1, (const char *const *)&vSource, NULL);
	glCompileShader(vertexShader);
	// Check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char errorLog[logLength];
		glGetShaderInfoLog(vertexShader, logLength, NULL, &errorLog[0]);
		printf("VERTEX SHADER COMPILE %s", &errorLog[0]);
	}
	// fragment
	glShaderSource(fragmentShader, 1, (const char *const *)&fSource, NULL);
	glCompileShader(fragmentShader);
	// Check fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char errorLog[logLength];
		glGetShaderInfoLog(fragmentShader, logLength, NULL, &errorLog[0]);
		printf("FRAGMENT SHADER COMPILE %s", &errorLog[0]);
	}
	free(vSource);
	free(fSource);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength){
		char programError[logLength]; programError[0] = 0;
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		printf("LINKER %s", &programError[0]);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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
void setShaderUniformVec2f(GLuint shader, char *uniform, float *array){
	if(shader){
		glUseProgram(shader);
		GLint loc = glGetUniformLocation(shader, uniform);
		if(loc != -1) glUniform2fv(loc, 1, &array[0]);
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
#endif
///////////////////////////////////////
//////////       INPUT       //////////
///////////////////////////////////////
void moveOriginWithArrowKeys(){
	// process input devices if in first person perspective mode
	// map movement direction to the direction the person is facing
	// float lookAzimuth = lookOrientation[0]/180.0*M_PI;
	float lookAzimuth = horizon[0]/180.0*M_PI;
	if(PERSPECTIVE == POLAR){ lookAzimuth += M_PI; }
	float dOrigin[3] = {0.0f, 0.0f, 0.0f};
	if(keyboard[UP_KEY] || keyboard['W'] || keyboard['w']){
		dOrigin[0] += WALK_INTERVAL * cosf(lookAzimuth);
		dOrigin[1] -= WALK_INTERVAL * sinf(lookAzimuth);
	}
	if(keyboard[DOWN_KEY] || keyboard['S'] || keyboard['s']){
		dOrigin[0] -= WALK_INTERVAL * cosf(lookAzimuth);
		dOrigin[1] += WALK_INTERVAL * sinf(lookAzimuth);
	}
	if(keyboard[LEFT_KEY] || keyboard['A'] || keyboard['a']){
		dOrigin[0] -= WALK_INTERVAL * cosf(lookAzimuth+M_PI_2);
		dOrigin[1] += WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
	}
	if(keyboard[RIGHT_KEY] || keyboard['D'] || keyboard['d']){
		dOrigin[0] += WALK_INTERVAL * cosf(lookAzimuth+M_PI_2);
		dOrigin[1] -= WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
	}
	if(keyboard['Q'] || keyboard['q'])
		dOrigin[2] += WALK_INTERVAL;
	if(keyboard['Z'] || keyboard['z'])
		dOrigin[2] -= WALK_INTERVAL;
	origin[0] += dOrigin[0];
	origin[1] += dOrigin[1];
	origin[2] += dOrigin[2];
}
static int mouseDragStartX, mouseDragStartY;
void mouseUpdatePerspective(int dx, int dy){
	switch(PERSPECTIVE){
		case FPP:
			horizon[0] -= (dx * MOUSE_SENSITIVITY);
			horizon[1] += (dy * MOUSE_SENSITIVITY);
			// horizon[2] = 0.0;
			firstPersonPerspective();
		break;
		case POLAR:
			horizon[0] -= (dx * MOUSE_SENSITIVITY);
			horizon[1] -= (dy * MOUSE_SENSITIVITY);
			// horizon[2] = 0.0;
			polarPerspective();
			break;
		case ORTHO:
			orthoFrame[0] += dx / (WIDTH / orthoFrame[2]);
			orthoFrame[1] += dy / (HEIGHT / orthoFrame[3]);
			orthoPerspective(orthoFrame[0], orthoFrame[1], orthoFrame[2], orthoFrame[3]);
			break;
	}
}
// when mouse button state changes
void mouseButtons(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(!state){  // button down
			mouseX = x;
			mouseY = y;
			mouseDragStartX = x;
			mouseDragStartY = y;
			mouseDown(button);
		}
		else {  // button up
			mouseUp(button);
		}
	}
	else if(button == GLUT_MIDDLE_BUTTON){
		if(!state)  mouseDown(button);
		else        mouseUp(button);
	}
	else if(button == GLUT_RIGHT_BUTTON){
		if(!state)  mouseDown(button);
		else        mouseUp(button);
	}
}
// when mouse is dragging screen
void mouseMotion(int x, int y){
	// change since last mouse event
	if(OPTIONS & (1 << BIT_MOUSE_LOOK)){
		mouseUpdatePerspective(mouseX - x, mouseY - y);
	}
	// update new state
	mouseX = x;
	mouseY = y;
	mouseDragX = mouseDragStartX - x;
	mouseDragY = mouseDragStartY - y;
	mouseMoved(x, y);
	glutPostRedisplay();
}
// when mouse is moving but no buttons are pressed
void mousePassiveMotion(int x, int y){
	mouseX = x;
	mouseY = y;
	mouseMoved(x, y);
}
void keyboardDown(unsigned char key, int x, int y){
	if(keyboard[key] == 1)
		return;   // prevent repeated keyboard calls
	keyboard[key] = 1;

	if(OPTIONS & (1 << BIT_KEYBOARD_FUNCTIONS)){
		if(key == ESCAPE_KEY)  // ESCAPE key
			exit (0);
		else if(key == 'F' || key == 'f'){
			toggleFullscreen();
		}
		else if(key == 'P' || key == 'p'){
			PERSPECTIVE = (PERSPECTIVE+1)%3;
			rebuildProjection();
		}
		else if(key == '.'){
			FOV += 0.01;
			rebuildProjection();
		}
		else if(key == ','){
			FOV -= 0.01;
			rebuildProjection();
		}
		else if(key == 'G' || key == 'g'){
			OPTIONS ^= SET_SHOW_GROUND;
			glutPostRedisplay();
		}
		else if (key == 'X' || key == 'x'){
			OPTIONS ^= SET_SHOW_GRID;
			glutPostRedisplay();
		}
	}
	keyDown(key);
	if(!CONTINUOUS_REFRESH)
		keyboardSetIdleFunc(); // for efficient screen draw, trigger redraw if needed
}
void keyboardUp(unsigned char key, int x, int y){
	if(keyboard[key] == 0)
		return;   // prevent repeated keyboard calls
	keyboard[key] = 0;
	keyUp(key);
	if(!CONTINUOUS_REFRESH)
		keyboardSetIdleFunc();  // for efficient screen draw, turn off redraw if needed
}
void specialDown(int key, int x, int y){
	key += 128;  // special keys get stored in the 128-255 index range
	if(keyboard[key] == 1)
		return;   // prevent repeated keyboard calls
	keyboard[key] = 1;
	keyDown(key);
	if(!CONTINUOUS_REFRESH)
		keyboardSetIdleFunc();
}
void specialUp(int key, int x, int y){
	key += 128;  // special keys get stored in the 128-255 index range
	if(keyboard[key] == 0)
		return;   // prevent repeated keyboard calls
	keyboard[key] = 0;
	keyUp(key);
	if(!CONTINUOUS_REFRESH)
		keyboardSetIdleFunc();
}
void keyboardSetIdleFunc(){
	// if any key is pressed, idle function is set to re-draw screen
	unsigned char keyDown = 0;
	for(int i = 0; i < 256; i++){
		if(keyboard[i] == 1){
			keyDown = 1;
			break;
		}
	}
	if(keyDown){
		glutIdleFunc(updateWorld);
	} else {
		glutIdleFunc(NULL);
	}
}
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        TINY OPENGL TOOLBOX         //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// void text(const char *text, float x, float y, void *font){
void text(const char *text, float x, float y, float z){
	// GLUT_BITMAP_8_BY_13
	// GLUT_BITMAP_9_BY_15
	// GLUT_BITMAP_TIMES_ROMAN_10
	// GLUT_BITMAP_TIMES_ROMAN_24
	// GLUT_BITMAP_HELVETICA_10
	// GLUT_BITMAP_HELVETICA_12
	// GLUT_BITMAP_HELVETICA_18
	const char *c;
	// glRasterPos2f(x, 24+y);
	glRasterPos3f(x, y, z);
	for (c = text; *c != '\0'; c++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
} 
void drawPoint(float x, float y, float z){
	GLfloat _point_vertex[] = { x, y, z };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _point_vertex);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	GLfloat _lines_vertices[6] = {x1, y1, z1, x2, y2, z2};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _lines_vertices);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginSquare(){
	static const GLfloat _unit_square_vertex[] = {
		0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f };
	static const GLfloat _unit_square_normals[] = {
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f };
	static const GLfloat _texture_coordinates[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
	glVertexPointer(3, GL_FLOAT, 0, _unit_square_vertex);
	glNormalPointer(GL_FLOAT, 0, _unit_square_normals);
	glTexCoordPointer(2, GL_FLOAT, 0, _texture_coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void drawRect(float x, float y, float z, float width, float height){
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(width, height, 1.0);
		drawUnitOriginSquare();
	glPopMatrix();
}
void drawUnitSquare(float x, float y, float z){
	glPushMatrix();
		glTranslatef(x, y, z);
		drawUnitOriginSquare();
	glPopMatrix();
}
void drawUnitOriginSphere(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
	glVertexPointer(3, GL_FLOAT, 0, _unit_sphere_vertices);
	glNormalPointer(GL_FLOAT, 0, _unit_sphere_normals);
	glTexCoordPointer(2, GL_FLOAT, 0, _unit_sphere_texture);
	// glDrawArrays(GL_LINE_LOOP, 0, _sphere_slices * _sphere_stacks * 2 );//(_sphere_slices+1) * 2 * (_sphere_stacks-1)+2  );
	glDrawArrays(GL_TRIANGLE_STRIP, 0,  _sphere_slices * _sphere_stacks * 2 );// (_sphere_slices+1) * 2 * (_sphere_stacks-1)+2  );
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitSphere(float x, float y, float z){
	glPushMatrix();
		glTranslatef(x, y, z);
		drawUnitOriginSphere();
	glPopMatrix();
}
void drawSphere(float x, float y, float z, float radius){
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(radius, radius, radius);
		drawUnitOriginSphere();
	glPopMatrix();
}
void drawUnitOriginCircle(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _unit_circle_vertices);
	glDrawArrays(GL_LINE_LOOP, 0, 64);
	glDisableClientState(GL_VERTEX_ARRAY);	
}
void drawUnitCircle(float x, float y, float z){
	glPushMatrix();
		glTranslatef(x, y, z);
		drawUnitOriginCircle();
	glPopMatrix();
}
void drawCircle(float x, float y, float z, float radius){
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(radius, radius, 1.0);
		drawUnitOriginCircle();
	glPopMatrix();
}
void draw3DAxesLines(float x, float y, float z, float scale){
	static const GLfloat _axis_lines_vertices[] = {
		1.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, -1.0f};
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _axis_lines_vertices);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}
void drawUVSphereLines(){
	float a1 = 0.33;
	float a2 = 0.166;
	glPushMatrix();
		// equator
		// glColor4f(1.0, 1.0, 1.0, a1);
			drawUnitCircle(0, 0, 0);
		// latitude
		// glColor4f(1.0, 1.0, 1.0, a2);
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
		// glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(90, 0, 1, 0);
			drawUnitCircle(0, 0, 0);
		// glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		// glColor4f(1.0, 1.0, 1.0, a1);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
		// glColor4f(1.0, 1.0, 1.0, a2);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
			glRotatef(30, 1, 0, 0);
			drawUnitCircle(0, 0, 0);
	glPopMatrix();
	// glColor4f(1.0, 1.0, 1.0, 1.0);
}
const float _tetrahedron_points[12] = {1.0,0.0,0.0,-0.3333,-0.9428,0.0,-0.3333,0.4714,0.81649,-0.3333,0.4714,-0.8164};
const unsigned short _tetrahedron_lines[12] = {2,3,2,0,2,1,3,0,3,1,0,1};
const unsigned short _tetrahedron_faces[12] = {2,1,3,2,3,0,2,0,1,3,1,0};
const float _tetrahedron_dual_points[12] = {-1.0,0.0,0.0,0.3333,0.9428,0.0,0.3333,-0.4714,0.81649,0.3333,-0.4714,-0.81649};
const unsigned short _tetrahedron_dual_lines[12] = {2,3,2,0,2,1,3,0,3,1,0,1};
const unsigned short _tetrahedron_dual_faces[12] = {2,1,3,2,3,0,2,0,1,3,1,0};
const float _octahedron_points[18] = {1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,-1.0,0.0,0.0,0.0,-1.0,0.0,0.0,0.0,-1.0};
const unsigned short _octahedron_lines[24] = {1,0,1,2,1,5,1,3,3,2,2,0,0,5,5,3,4,2,4,3,4,5,4,0};
const unsigned short _octahedron_faces[24] = {1,0,2,1,5,0,4,0,5,4,2,0,1,2,3,1,3,5,4,5,3,4,3,2};
const float _hexahedron_points[24] = {0.57735,0.57735,0.57735,0.57735,0.57735,-0.57735,0.57735,-0.57735,-0.57735,0.57735,-0.57735,0.57735,-0.57735,0.57735,0.57735,-0.57735,0.57735,-0.57735,-0.57735,-0.57735,-0.57735,-0.57735,-0.57735,0.57735};
const unsigned short _hexahedron_lines[24] = {0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4};
const unsigned short _hexahedron_faces[24] = {0,3,2,1,4,0,1,5,0,3,7,4,7,4,5,6,3,7,6,2,1,5,6,2};
const unsigned short _hexahedron_triangle_faces[36] = {0,2,3,2,0,1,4,1,0,1,4,5,0,3,7,7,4,0,7,5,4,5,7,6,3,6,7,6,3,2,1,5,6,6,2,1};
const float _icosahedron_points[36] = {0.44721,-0.2763,0.85065,-0.44721,0.2763,0.85065,-0.44721,0.2763,-0.85065,0.44721,-0.2763,-0.85065,-0.44721,-0.7236,0.52573,0.44721,0.7236,0.52573,0.44721,0.7236,-0.52573,-0.44721,-0.7236,-0.52573,0.44721,-0.8944,0.0,1.0,0.0,0.0,-0.44721,0.8944,0.0,-1.0,0.0,0.0};
const unsigned short _icosahedron_lines[60] = {0,8,0,9,0,1,0,4,0,5,8,3,8,9,8,7,8,4,9,3,9,6,9,5,7,4,7,3,7,2,7,11,2,10,2,11,2,3,2,6,10,11,10,5,10,6,10,1,11,1,11,4,4,1,5,1,5,6,6,3};
const unsigned short _icosahedron_faces[60] = {8,7,4,7,8,3,8,4,0,8,0,9,9,3,8,9,0,5,9,5,6,9,6,3,3,2,7,3,6,2,0,4,1,0,1,5,11,4,7,11,7,2,11,2,10,1,11,10,11,1,4,10,6,5,10,5,1,10,2,6};
const float _dodecahedron_points[60] = {-0.7946,0.4911,0.3568,-0.1875,0.7946,-0.5773,-0.7946,0.4911,-0.3568,-0.7946,-0.1875,-0.5773,0.1875,0.3035,-0.9341,0.1875,0.9822,0.0,0.1875,-0.7946,-0.5773,-0.1875,-0.3035,-0.9341,-0.1875,-0.9822,0.0,-0.7946,-0.6070,0.0,0.7946,-0.4911,-0.3568,0.1875,-0.7946,0.5773,-0.1875,-0.3035,0.9341,-0.7946,-0.1875,0.5773,0.7946,-0.4911,0.3568,-0.1875,0.7946,0.5773,0.1875,0.3035,0.9341,0.7946,0.1875,0.5773,0.7946,0.6070,0.0,0.7946,0.1875,-0.5773};
const unsigned short _dodecahedron_lines[60] = {19,18,18,17,17,14,14,10,10,19,14,11,11,8,8,6,6,10,19,4,4,1,1,5,5,18,9,3,3,7,7,6,8,9,13,9,3,2,2,0,0,13,12,11,12,13,1,2,5,15,15,0,15,16,16,17,16,12,4,7};
const unsigned short _dodecahedron_triangle_faces[108] = {5,2,1,15,0,5,2,5,0,9,2,13,2,0,13,3,2,9,10,17,19,18,19,17,14,17,10,19,1,4,6,9,8,1,18,5,11,14,8,18,1,19,8,9,11,10,8,14,8,10,6,6,7,9,9,7,3,13,12,9,12,11,9,18,15,5,17,15,18,15,17,16,11,12,14,14,12,17,17,12,16,12,13,16,13,0,16,0,15,16,7,2,3,2,7,1,1,7,4,4,7,19,19,7,10,6,10,7};
const unsigned int _platonic_num_vertices[6] = {4,6,8,12,20,4};
const unsigned int _platonic_num_lines[6] = {6,12,12,30,30,6};
const unsigned int _platonic_num_faces[6] = {4,8,12,20,36,4};//{4,8,6,20,12,4};
const float* _platonic_point_arrays[6] = {_tetrahedron_points,_octahedron_points,_hexahedron_points,_icosahedron_points,_dodecahedron_points,_tetrahedron_dual_points};
const unsigned short* _platonic_line_array[6] = {_tetrahedron_lines,_octahedron_lines,_hexahedron_lines,_icosahedron_lines,_dodecahedron_lines,_tetrahedron_dual_lines};
const unsigned short* _platonic_face_array[6] = {_tetrahedron_faces,_octahedron_faces,_hexahedron_triangle_faces,_icosahedron_faces,_dodecahedron_triangle_faces,_tetrahedron_dual_faces};
const int _platonic_dual_index[6] = { 5,2,1,4,3,0 };
void drawPlatonicSolidFaces(char solidType){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
	glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
	glDrawElements(GL_TRIANGLES, 3*_platonic_num_faces[solidType], GL_UNSIGNED_SHORT, _platonic_face_array[solidType]);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void drawPlatonicSolidLines(char solidType){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
	glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
	glDrawElements(GL_LINES, 2*_platonic_num_lines[solidType], GL_UNSIGNED_SHORT, _platonic_line_array[solidType]);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void drawPlatonicSolidPoints(char solidType){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
	glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
	glDrawArrays(GL_POINTS, 0, _platonic_num_vertices[solidType]);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
unsigned char * getTextureData(const char * filename, int width, int height){
	FILE * file;
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	unsigned char *data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	for(int i = 0; i < width * height; i++){
		int index = i*3;
		unsigned char B,R;
		B = data[index];
		R = data[index+2];
		data[index] = R;
		data[index+2] = B;
	}
	return data;
}
GLuint loadTexture(const char * filename, int width, int height){
	GLuint texture;
	unsigned char *data = getTextureData(filename, width, height);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
GLuint loadTextureSmooth(const char * filename, int width, int height){
	GLuint texture;
	unsigned char * data = getTextureData(filename, width, height);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
void initPrimitives(){
	static unsigned char _geometry_initialized = 0;
	if (!_geometry_initialized) {
		// CIRCLE
		for(int i = 0; i < 64; i++){
			_unit_circle_vertices[i*3+0] = -sinf(M_PI*2/64.0f*i);
			_unit_circle_vertices[i*3+1] = cosf(M_PI*2/64.0f*i);
			_unit_circle_vertices[i*3+2] = 0.0f;
			_unit_circle_normals[i*3+0] = _unit_circle_normals[i*3+1] = 0.0;
			_unit_circle_normals[i*3+2] = 1.0;
			_unit_circle_texCoord[i*3+0] = -sinf(M_PI*2/64.0f*i)*0.5 + 0.5;
			_unit_circle_texCoord[i*3+1] = cosf(M_PI*2/64.0f*i)*0.5 + 0.5;
		}
		// SPHERE
		GLfloat m_Scale = 1;
		GLfloat *vPtr = _unit_sphere_vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_sphere_slices*2+2) * (_sphere_stacks)));
		GLfloat *nPtr = _unit_sphere_normals = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_sphere_slices*2+2) * (_sphere_stacks)));
		GLfloat *tPtr = _unit_sphere_texture = (GLfloat*)malloc(sizeof(GLfloat) * 2 * ((_sphere_slices*2+2) * (_sphere_stacks)));
		for(unsigned int phiIdx = 0; phiIdx < _sphere_stacks; phiIdx++){
			// Latitude
			//starts at -pi/2 goes to pi/2
			float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)(_sphere_stacks)) - 0.5);  // the first circle
			float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)(_sphere_stacks)) - 0.5);  // second one
			float cosPhi0 = cos(phi0);
			float sinPhi0 = sin(phi0);
			float cosPhi1 = cos(phi1);
			float sinPhi1 = sin(phi1);
			for(unsigned int thetaIdx = 0; thetaIdx < _sphere_slices; thetaIdx++){
				//longitude
				float theta = 2.0*M_PI * ((float)thetaIdx) * (1.0/(float)(_sphere_slices - 1));
				float cosTheta = cos(theta+M_PI*.5);
				float sinTheta = sin(theta+M_PI*.5);
				vPtr[0] = m_Scale*cosPhi0 * cosTheta;
				vPtr[1] = m_Scale*(cosPhi0 * sinTheta);
				vPtr[2] = -m_Scale*sinPhi0;
				vPtr[3] = m_Scale*cosPhi1 * cosTheta;
				vPtr[4] = m_Scale*(cosPhi1 * sinTheta);
				vPtr[5] = -m_Scale*sinPhi1;
				nPtr[0] = cosPhi0 * cosTheta;
				nPtr[1] = cosPhi0 * sinTheta;
				nPtr[2] = -sinPhi0;
				nPtr[3] = cosPhi1 * cosTheta;
				nPtr[4] = cosPhi1 * sinTheta;
				nPtr[5] = -sinPhi1;
				GLfloat texX = (float)thetaIdx * (1.0f/(float)(_sphere_slices-1));
				tPtr[0] = texX;
				tPtr[1] = (float)(phiIdx + 0) * (1.0f/(float)(_sphere_stacks));
				tPtr[2] = texX;
				tPtr[3] = (float)(phiIdx + 1) * (1.0f/(float)(_sphere_stacks));
				vPtr += 2*3;
				nPtr += 2*3;
				tPtr += 2*2;
			}
		}
		_geometry_initialized = 1;
	}
}
/////////////////////////    HELPFUL ORIENTATION    //////////////////////////
void worldInfoText(int x, int y, int z){
	switch(PERSPECTIVE){
		case FPP:   text("First Person Perspective", x, y, z); break;
		case POLAR: text("Polar Perspective", x, y, z); break;
		case ORTHO: text("Orthographic Perspective", x, y, z); break;
	}
	char string[35];
	sprintf(string, "(%.2f, %.2f, %.2f)", horizon[0], horizon[1], horizon[2]);   
	text(string, x, y+13*1, z);
	if(PERSPECTIVE == ORTHO){
		char frameString[50];
		sprintf(frameString, "%d, %d, %d, %d", (int)orthoFrame[0], (int)orthoFrame[1], (int)orthoFrame[2], (int)orthoFrame[3] );
		text(frameString, x, y+13*2, z);
	}
}
void drawAxesLabels(float scale){
	text("+X", scale, 0, 0);  text("-X", -scale, 0, 0);
	text("+Y", 0, scale, 0);  text("-Y", 0, -scale, 0);
	text("+Z", 0, 0, scale);  text("-Z", 0, 0, -scale);
}
void drawAxesCoordinates(float scale){
	int scaleInt = scale;
	char string[50];
	sprintf(string, "(%d, 0, 0)", scaleInt);   text(string, scale, 0, 0);
	sprintf(string, "(0, %d, 0)", scaleInt);   text(string, 0, scale, 0);
	sprintf(string, "(0, 0, %d)", scaleInt);   text(string, 0, 0, scale);
	sprintf(string, "(%d, 0, 0)", -scaleInt);  text(string, -scale, 0, 0);
	sprintf(string, "(0, %d, 0)", -scaleInt);  text(string, 0, -scale, 0);
	sprintf(string, "(0, 0, %d)", -scaleInt);  text(string, 0, 0, -scale);
}
void drawCheckerboard(float walkX, float walkY, int numSquares){
	static GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat mat_black[] = { 0.0, 0.0, 0.0, 1.0 };
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	int evenOdd = (numSquares%2);
	if(evenOdd) 
		numSquares--;
	for(int i = -numSquares*.5; i < numSquares*.5; i++){
		for(int j = -numSquares*.5; j < numSquares*.5; j++){
			int b = abs(((i+j+XOffset+YOffset)%2));
			if(b) { glColor3f(1.0, 1.0, 1.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white); }
			else { glColor3f(0.0, 0.0, 0.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black); }
			drawUnitSquare(i-XOffset - evenOdd, j-YOffset - evenOdd, 0);
		}
	}
}
// span: how many units to skip inbetween each axis
// repeats: how many rows/cols/stacks on either side of center
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats){
	float XSpanMod = walkX - floor(walkX/span)*span;
	float YSpanMod = walkY - floor(walkY/span)*span;
	float ZSpanMod = walkZ - floor(walkZ/span)*span;
	for(int i = -repeats*span; i < repeats*span; i+=span){
		for(int j = -repeats*span; j < repeats*span; j+=span){
			for(int k = -repeats*span; k < repeats*span; k+=span){
				// distance approximation works just fine in this case
				float distance = fabs(i+XSpanMod-1) + fabs(j+YSpanMod-1) + abs(k);
				float brightness = 1.0 - distance/(repeats*span);
				GLfloat material[] = { 1.0, 1.0, 1.0, brightness };
				glColor4f(1.0, 1.0, 1.0, brightness);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
				// glLineWidth(100.0/distance/distance);
				draw3DAxesLines(i + XSpanMod - walkX,
				                j + YSpanMod - walkY,
				                k + ZSpanMod - walkZ, 1.0);
			}
		}
	}
}
/////////////////////////        MATH         //////////////////////////
// ALGEBRA
#define D2R 0.01745329251994    // degrees to radians
#define R2D 57.295779513082321  // radians to degrees
float min(float one, float two){
	if(one > two) return two;
	return one;
}
float max(float one, float two){
	if(one < two) return two;
	return one;
}
float modulusContext(float complete, int modulus){
	double wholePart;
	double fracPart = modf(complete, &wholePart);
	return ( ((int)wholePart) % modulus ) + fracPart;
}
// MATRICES
unsigned char mat4Inverse(const float m[16], float inverse[16]){
	float inv[16], det;
	int i;
	inv[0] = m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] + m[9]*m[7]*m[14] +m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] = -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] = m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] = -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] = m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] = -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] = m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] = m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15] + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] = -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15] - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] = m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15] + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0] *m[6]*m[13] + m[4]*m[1]*m[14] - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] = -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11] - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] = m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11] + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11] - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
	inv[15] = m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10] + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
	det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if (det == 0)
		return 0;
	det = 1.0 / det;
	for (i = 0; i < 16; i++)
		inverse[i] = inv[i]*det;
	return 1;
}
void mat4x4MultUnique(const float *a, const float *b, float *result){
	// this is counting on a or b != result   eg: cannot do mat4x4MultUnique(a, b, a);
	result[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	result[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	result[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	result[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];
	result[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	result[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	result[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	result[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];
	result[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	result[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	result[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	result[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];
	result[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	result[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	result[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	result[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}
void mat4x4Mult(const float *a, const float *b, float *result) {
	float c[16];
	mat4x4MultUnique(a, b, c);
	memcpy(result, c, sizeof(float)*16);
}
void mat3x3MultUnique(const float *a, const float *b, float *result){
	// this is counting on a or b != result   eg: cannot do mat3x3MultUnique(a, b, a);
	result[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
	result[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
	result[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];
	result[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	result[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	result[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
	result[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	result[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	result[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
}
void mat3x3Mult(const float *a, const float *b, float *result) {
	float c[9];
	mat3x3MultUnique(a, b, c);
	memcpy(result, c, sizeof(float)*9);
}
void mat3ToMat4(float *i, float *result){
	result[0] = i[0];   result[1] = i[1];   result[2] = i[2];   result[3] = 0.0f;
	result[4] = i[3];   result[5] = i[4];   result[6] = i[5];   result[7] = 0.0f;
	result[8] = i[6];   result[9] = i[7];   result[10] = i[8];  result[11] = 0.0f;
	result[12] = 0.0f;  result[13] = 0.0f;  result[14] = 0.0f;  result[15] = 1.0f;
}
void mat4Transpose(float *m) {
	float t[16];
	t[0] = m[0];   t[1] = m[4];   t[2] = m[8];   t[3] = m[12];
	t[4] = m[1];   t[5] = m[5];   t[6] = m[9];   t[7] = m[13];
	t[8] = m[2];   t[9] = m[6];   t[10] = m[10]; t[11] = m[14];
	t[12] = m[3];  t[13] = m[7];  t[14] = m[11]; t[15] = m[15];
	memcpy(m, t, sizeof(float)*16);
}
void mat3Transpose(float *m) {
	float t[9];
	t[0] = m[0];  t[1] = m[3];  t[2] = m[6];
	t[3] = m[1];  t[4] = m[4];  t[5] = m[7];
	t[6] = m[2];  t[7] = m[5];  t[8] = m[8];
	memcpy(m, t, sizeof(float)*9);
}
void makeMat3XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;
	m[3] = 0;	m[4] = cos(angle);	m[5] = -sin(angle);
	m[6] = 0;	m[7] = sin(angle);	m[8] = cos(angle);
}
void makeMat3YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);
	m[3] = 0;			m[4] = 1;	m[5] = 0;
	m[6] = sin(angle);	m[7] = 0;	m[8] = cos(angle);
}
void makeMat3ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;
	m[3] = sin(angle);	m[4] = cos(angle);	m[5] = 0;
	m[6] = 0;			m[7] = 0;			m[8] = 1;
}
void makeMat4XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;			m[3] = 0;
	m[4] = 0;	m[5] = cos(angle);	m[6] = -sin(angle);	m[7] = 0;
	m[8] = 0;	m[9] = sin(angle);	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;	m[13] = 0;			m[14] = 0;			m[15] = 1;
}
void makeMat4YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);	m[3] = 0;
	m[4] = 0;			m[5] = 1;	m[6] = 0;			m[7] = 0;
	m[8] = sin(angle);	m[9] = 0;	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;			m[13] = 0;	m[14] = 0;			m[15] = 1;
}
void makeMat4ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;	m[3] = 0;
	m[4] = sin(angle);	m[5] = cos(angle);	m[6] = 0;	m[7] = 0;
	m[8] = 0;			m[9] = 0;			m[10] = 1;	m[11] = 0;
	m[12] = 0;			m[13] = 0;			m[14] = 0;	m[15] = 1;
}
void setMat3FromMat4(float *m3, float *m4){
	m3[0] = m4[0]; m3[1] = m4[1]; m3[2] = m4[2];
	m3[3] = m4[4]; m3[4] = m4[5]; m3[5] = m4[6];
	m3[6] = m4[8]; m3[7] = m4[9]; m3[8] = m4[10];
}
void setMat4FromMat3(float *m4, float *m3){
	m4[0] = m3[0]; m4[1] = m3[1]; m4[2] = m3[2]; m4[3] = 0;
	m4[4] = m3[3]; m4[5] = m3[4]; m4[6] = m3[5]; m4[7] = 0;
	m4[8] = m3[6]; m4[9] = m3[7]; m4[10] = m3[8]; m4[11] = 0;
	m4[12] = 0; m4[13] = 0; m4[14] = 0; m4[15] = 1;
}
void setMat4Identity(float *m){
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}
// VECTORS
void mat4Vec4Mult(const float m[16], const float v[4], float result[4]){
	result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3];
	result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7] * v[3];
	result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11] * v[3];
	result[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];
}
void mat3Vec3Mult(const float m[9], const float v[3], float result[3]){
	result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
	result[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
	result[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}
#endif /* WORLD_FRAMEWORK */
