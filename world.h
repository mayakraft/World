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

////////////////////////////////////////////////////////////////////////////////////////
//     WORLD is a hyper minimalist (1 file) framework for graphics (OpenGL) and user
//   input (keyboard, mouse) following the OpenFrameworks / Processing design paradigm
////////////////////////////////////////////////////////////////////////////////////////
//
//   HOW TO USE
//
//   1) make an empty .c file
//   2) #include "world.h"
//   3) implement the following functions:
//      done! type 'make', then 'make run'
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
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// CUSTOMIZE SETTINGS
#define CONTINUOUS_REFRESH 1  // (0) = maximum efficiency, screen can redraw only upon receiving input
static float MOUSE_SENSITIVITY = 0.333f;
static float WALK_INTERVAL = 0.1f;  // WALKING SPEED. @ 60 updates/second, walk speed = 6 units/second
static float ZOOM_SPEED = 0.4f;
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
static float originX = 0.0f;
static float originY = 0.0f;
static float originZ = 0.0f;
static float originDX = 0.0f;
static float originDY = 0.0f;
static float originDZ = 0.0f;
static float ZOOM = 15.0f;  // POLAR PERSPECTIVE    // zoom scale, converted to logarithmic
static float ZOOM_RADIX = 3;
static unsigned char GROUND = 1;  // a 2D grid
static unsigned char GRID = 1;    // a 3D grid
// PERSPECTIVE
enum{  FPP,  POLAR,  ORTHO  } ; // first persion, polar, orthographic
static unsigned char PERSPECTIVE = FPP;  // initialize point of view in this state
// details of each perspective
float polarLookAt[3] = {0.0f, 0.0f, 0.0f}; // x, y, z  // location of the eye
float lookOrientation[3] = {0.0f, 0.0f, 0.0f}; // azimuth, altitude, zoom/FOV
float orthoFrame[4] = {0.0f, 0.0f, 4.0f, 3.0f}; // x, y, width, height
// time
static time_t startTime;
static unsigned long frameNum;

// TABLE OF CONTENTS:
int main(int argc, char **argv);  // initialize Open GL context
void typicalOpenGLSettings();  // colors, line width, glEnable
void reshapeWindow(int windowWidth, int windowHeight);  // contains viewport and frustum calls
void rebuildProjection();  // calls one of the three functions below
// CHANGE PERSPECTIVE
void firstPersonPerspective();//float azimuth, float altitude, float zoom);
void polarPerspective(float x, float y, float z);  //float azimuth, float altitude, float zoom);
void orthoPerspective(float x, float y, float width, float height);
// DRAW, ALIGNMENT, INPUT HANDLING
void display();
void updateWorld();  // process input devices
// INPUT DEVICES
void mouseButtons(int button, int state, int x, int y);  // when mouse button state changes
void mouseMotion(int x, int y);   // when mouse is dragging screen
void mousePassiveMotion(int x, int y);  // when mouse is moving but not pressed
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key,int x,int y);
void specialDown(int key, int x, int y);
void specialUp(int key, int x, int y);
void keyboardSetIdleFunc();
// WORLD SHAPES
// all 2D shapes are in the X Y plane, normal along the Z
void drawRect(float x, float y, float z, float width, float height);
void drawUnitSquare(float x, float y, float z);
void draw3DAxesLines(float x, float y, float z, float scale);
void drawUnitSphere(float x, float y, float z, float radius);
// combinations of shapes
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats);
float modulusContext(float complete, int modulus);
// more
time_t elapsedSeconds();
GLuint loadTexture(const char * filename, int width, int height);
void text(const char *text, float x, float y, float z);

void initPrimitives();
static float _circle_vertices[192];
float *_unit_sphere_vertices, *_unit_sphere_normals, *_unit_sphere_texture;

void drawUnitCircle(float x, float y, float z);
void drawUVSphereLines();

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
	if(CONTINUOUS_REFRESH)
		glutIdleFunc(updateWorld);
	// setup this program
	orthoFrame[0] = -WIDTH*0.5;
	orthoFrame[1] = -HEIGHT*0.5;
	orthoFrame[2] = WIDTH;
	orthoFrame[3] = HEIGHT;

	memset(keyboard,0,256);
	startTime = time(NULL);
	frameNum = 0;
	initPrimitives();
	time_t t;
	srand((unsigned) time(&t));
	typicalOpenGLSettings();
	glutPostRedisplay();
	setup();  // user defined function
	// begin main loop
	glutMainLoop();
	return 0;
}
time_t elapsedSeconds(){
	return time(NULL) - startTime;
}
void typicalOpenGLSettings(){
	firstPersonPerspective();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
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
}
void rebuildProjection(){
	switch(PERSPECTIVE){
		case FPP:
			firstPersonPerspective(); break;
		case POLAR:
			polarPerspective(polarLookAt[0], polarLookAt[1], polarLookAt[2]); break;
		case ORTHO:
			orthoPerspective(orthoFrame[0], orthoFrame[1], orthoFrame[2], orthoFrame[3]); break;
	}
}
void firstPersonPerspective(){
	PERSPECTIVE = FPP;
	float a = (float)WIDTH / HEIGHT;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum (-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP);
	// change POV
	glRotatef(-lookOrientation[1], 1, 0, 0);
	glRotatef(-lookOrientation[0], 0, 0, 1);
	// raise POV 1.0 above the floor, 1.0 is an arbitrary value
	glTranslatef(0.0f, 0.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
}
void polarPerspective(float x, float y, float z){
	PERSPECTIVE = POLAR;
	polarLookAt[0] = x;
	polarLookAt[1] = y;
	polarLookAt[2] = z;
	float a = (float)WIDTH / HEIGHT;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum (-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP);
	// change POV
	glTranslatef(0, 0, -ZOOM);
	glRotatef(-lookOrientation[1], 1, 0, 0);
	glRotatef(-lookOrientation[0], 0, 0, 1);
	glTranslatef(x, y, z);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glPushMatrix();
			glColor4f(1.0, 1.0, 1.0, 1.0);
			// unclear if world should move with
			// glTranslatef(originX, originY, originZ);
			draw3D();
		glPopMatrix();
		// 3D REPEATED STRUCTURE
		if(GRID){
			float newX = modulusContext(-originX, 5);
			float newY = modulusContext(-originY, 5);
			float newZ = modulusContext(-originZ, 5);
			glPushMatrix();
				glTranslatef(newX, newY, newZ);
				drawAxesGrid(newX, newY, newZ, 5, 4);
			glPopMatrix();
		}
		// 2D REPEATED STRUCTURE
		if(GROUND){
			glPushMatrix();
			float newX = modulusContext(-originX, 2);
			float newY = modulusContext(-originY, 2);
			glTranslatef(newX, newY, -originZ);
			drawCheckerboard(newX, newY, 8);
			glPopMatrix();
		}
	glPopMatrix();
	
	glDisable(GL_BLEND);

	// TO ORTHOGRAPHIC
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
		draw2D();
	glPopMatrix();
	rebuildProjection();

	// bring back buffer to the front on vertical refresh, auto-calls glFlush
	glutSwapBuffers();
	// glFlush();
}
// process input devices if in first person perspective mode
void updateWorld(){
	frameNum += 1;
	// map movement direction to the direction the person is facing
	float lookAzimuth = lookOrientation[0]/180.0*M_PI;
	originDX = originDY = originDZ = 0;
	if(keyboard[UP_KEY] || keyboard['W'] || keyboard['w']){
		originDX += WALK_INTERVAL * sinf(lookAzimuth);
		originDY += WALK_INTERVAL * -cosf(lookAzimuth);
	}
	if(keyboard[DOWN_KEY] || keyboard['S'] || keyboard['s']){
		originDX -= WALK_INTERVAL * sinf(lookAzimuth);
		originDY -= WALK_INTERVAL * -cosf(lookAzimuth);
	}
	if(keyboard[LEFT_KEY] || keyboard['A'] || keyboard['a']){
		originDX += WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
		originDY += WALK_INTERVAL * -cosf(lookAzimuth+M_PI_2);
	}
	if(keyboard[RIGHT_KEY] || keyboard['D'] || keyboard['d']){
		originDX -= WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
		originDY -= WALK_INTERVAL * -cosf(lookAzimuth+M_PI_2);
	}
	if(keyboard['Q'] || keyboard['q'])
		originDZ -= WALK_INTERVAL;
	if(keyboard['Z'] || keyboard['z'])
		originDZ += WALK_INTERVAL;
	if(keyboard[MINUS_KEY]){
		ZOOM += ZOOM_SPEED;
		rebuildProjection();
	}
	if(keyboard[PLUS_KEY]){
		ZOOM -= ZOOM_SPEED;
		if(ZOOM < 0)
			ZOOM = 0;
		rebuildProjection();
	}
	originX -= originDX;
	originY -= originDY;
	originZ -= originDZ;
	update();
	glutPostRedisplay();
}
///////////////////////////////////////
//////////       INPUT       //////////
///////////////////////////////////////
static int mouseDragStartX, mouseDragStartY;
void mouseUpdatePerspective(int dx, int dy){
	switch(PERSPECTIVE){
		case FPP:
			lookOrientation[0] += (dx * MOUSE_SENSITIVITY);
			lookOrientation[1] += (dy * MOUSE_SENSITIVITY);
			// lookOrientation[2] = 0.0;
			firstPersonPerspective();
		break;
		case POLAR:
			lookOrientation[0] += (dx * MOUSE_SENSITIVITY);
			lookOrientation[1] += (dy * MOUSE_SENSITIVITY);
			// lookOrientation[2] = 0.0;
			polarPerspective(polarLookAt[0], polarLookAt[1], polarLookAt[2]);
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
	mouseUpdatePerspective(mouseX - x, mouseY - y);
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

	if(key == ESCAPE_KEY)  // ESCAPE key
		exit (0);
	else if(key == 'F' || key == 'f'){
		if(!FULLSCREEN)
			glutFullScreen();
		else{
			reshapeWindow(WIDTH, HEIGHT);
			glutPositionWindow(0,0);
		}
		FULLSCREEN = !FULLSCREEN;
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
		GROUND = !GROUND;
		glutPostRedisplay();
	}
	else if (key == 'X' || key == 'x'){
		GRID = !GRID;
		glutPostRedisplay();
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
GLuint loadTexture(const char * filename, int width, int height){
	GLuint texture;
	unsigned char * data;
	FILE * file;
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
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
float modulusContext(float complete, int modulus){
	double wholePart;
	double fracPart = modf(complete, &wholePart);
	return ( ((int)wholePart) % modulus ) + fracPart;
}
void drawPoint(float x, float y, float z){
	static const GLfloat _zero_point_vertex[] = { 0.0f, 0.0f, 0.0f };
	glPushMatrix();
		glTranslatef(x, y, z);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _zero_point_vertex);
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
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
// GLint _sphere_stacks = 7; 
// GLint _sphere_slices = 13;
GLint _sphere_stacks = 20; 
GLint _sphere_slices = 30;
void initPrimitives(){
	static unsigned char _geometry_initialized = 0;
	if (!_geometry_initialized) {
		// CIRCLE
		for(int i = 0; i < 64; i++){
			_circle_vertices[i*3+0] = -sinf(M_PI*2/64.0f*i);
			_circle_vertices[i*3+1] = cosf(M_PI*2/64.0f*i);
			_circle_vertices[i*3+2] = 0.0f;
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
void drawUnitSphere(float x, float y, float z, float radius){
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(radius, radius, radius);
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
	glPopMatrix();
}
void drawUnitCircle(float x, float y, float z){
	glPushMatrix();
	glTranslatef(x, y, z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _circle_vertices);
	glDrawArrays(GL_LINE_LOOP, 0, 64);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}
/////////////////////////        HELPFUL ORIENTATION         //////////////////////////
void label3DAxes(float scale){
	int scaleInt = scale;
	char string[50];
	sprintf(string, "(%d, 0, 0)", scaleInt);
	text(string, scale, 0, 0);
	sprintf(string, "(0, %d, 0)", scaleInt);
	text(string, 0, scale, 0);
	sprintf(string, "(0, 0, %d)", scaleInt);
	text(string, 0, 0, scale);
	sprintf(string, "(%d, 0, 0)", -scaleInt);
	text(string, -scale, 0, 0);
	sprintf(string, "(0, %d, 0)", -scaleInt);
	text(string, 0, -scale, 0);
	sprintf(string, "(0, 0, %d)", -scaleInt);
	text(string, 0, 0, -scale);
}
void drawCheckerboard(float walkX, float walkY, int numSquares){
	static GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat mat_black[] = { 0.0, 0.0, 0.0, 1.0 };
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	// if even split
	if(numSquares%2 == 0){
		for(int i = -numSquares*.5; i <= numSquares*.5; i++){
			for(int j = -numSquares*.5; j <= numSquares*.5; j++){
				int b = abs(((i+j+XOffset+YOffset)%2));
				if(b) { glColor3f(1.0, 1.0, 1.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white); }
				else { glColor3f(0.0, 0.0, 0.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black); }
				drawUnitSquare(i-XOffset, j-YOffset, 0);
			}
		}
	}
// if odd number
	else{
		numSquares--;
		for(int i = -numSquares*.5; i <= numSquares*.5; i++){
			for(int j = -numSquares*.5; j <= numSquares*.5; j++){
				int b = abs(((i+j+XOffset+YOffset)%2));
				if(b) { glColor3f(1.0, 1.0, 1.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white); }
				else { glColor3f(0.0, 0.0, 0.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black); }
				drawUnitSquare(i-XOffset - .5, j-YOffset - .5, 0);
			}
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
void mat4x4Mult(const float *a, const float *b, float *c){
	c[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12]; 
	c[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13]; 
	c[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14]; 
	c[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15]; 
	c[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12]; 
	c[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13]; 
	c[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14]; 
	c[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15]; 
	c[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12]; 
	c[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13]; 
	c[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14]; 
	c[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15]; 
	c[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12]; 
	c[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13]; 
	c[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14]; 
	c[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]; 
}
void mat4x4MultFast(const float *a, const float *b, float *result){
	// this is counting on a != b != c   eg: cannot do mat4x4MultFast(a, b, a);
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
	float c[16];
	mat4x4MultFast(a, b, c);
	memcpy(result, c, sizeof(float)*16);
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
