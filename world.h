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
#define CONTINUOUS_REFRESH 0  // (0) = maximum efficiency, screen will only redraw upon receiving input
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
static float FAR_CLIP = 1000.0;
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
void drawRect(float x, float y, float width, float height);
void drawUnitSquare(float x, float y);
void drawUnitAxis(float x, float y, float z, float scale);
void drawUnitSphere();
// combinations of shapes
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats);
void drawZoomboard(float zoom);
float modulusContext(float complete, int modulus);
// time
time_t elapsedSeconds();

GLuint loadTexture(const char * filename, int width, int height);
void drawUnitSphere();

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
	orthoFrame[2] = WIDTH;
	orthoFrame[3] = HEIGHT;
	memset(keyboard,0,256);
	startTime = time(NULL);
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
	orthoFrame[2] = WIDTH;
	orthoFrame[3] = HEIGHT;
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
	// PERSPECTIVE = ORTHO;
	orthoFrame[0] = x;
	orthoFrame[1] = y;
	orthoFrame[2] = width;
	orthoFrame[3] = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, width + x, height + y, y, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		// 3D REPEATED STRUCTURE
		if(GRID){
			glPushMatrix();
			float newX = modulusContext(originX, 5);
			float newY = modulusContext(originY, 5);
			float newZ = modulusContext(originZ, 5);
			glTranslatef(newX, newY, newZ);
			drawAxesGrid(newX, newY, newZ, 5, 4);
			glPopMatrix();
		}
		// 2D REPEATED STRUCTURE
		if(GROUND){
			glPushMatrix();
			float newX = modulusContext(originX, 2);
			float newY = modulusContext(originY, 2);
			glTranslatef(newX, newY, originZ);
			drawCheckerboard(newX, newY, 8);
			glPopMatrix();
		}
		// if(ZOOM_GROUND){
		// 	static double intpart;
		// 	float zoom = powf(3,modf(-originY, &intpart));
		// 	drawZoomboard(zoom);
		// }

		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			// unclear if world should move with
			// glTranslatef(originX, originY, originZ);
			draw3D();
		glPopMatrix();
	glPopMatrix();

	orthoPerspective(orthoFrame[0], orthoFrame[1], orthoFrame[2], orthoFrame[3]);
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
	originX += originDX;
	originY += originDY;
	originZ += originDZ;
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
	if(keyDown)
		glutIdleFunc(updateWorld);
	else
		glutIdleFunc(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        TINY OPENGL TOOLBOX         //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/////////////////////////        PRIMITIVES         //////////////////////////

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
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
void drawRect(float x, float y, float width, float height){
	glPushMatrix();
	glScalef(width, height, 1.0);
	drawUnitSquare(x, y);
	glPopMatrix();
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
// draws a XY 1x1 square in the Z = 0 plane
void drawUnitSquare(float x, float y){
	static const GLfloat _unit_square_vertex[] = {
		0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f };
	static const GLfloat _unit_square_normals[] = {
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f };
	static const GLfloat _texture_coordinates[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glPushMatrix();
	glTranslatef(x, y, 0.0);
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
	glPopMatrix();
}
void drawUnitAxis(float x, float y, float z, float scale){
	static const GLfloat _unit_axis_vertices[] = {
		1.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, -1.0f};
	static const GLfloat _unit_axis_normals[] = {
		0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f};
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _unit_axis_vertices);
	glNormalPointer(GL_FLOAT, 0, _unit_axis_normals);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}
void drawUnitSphere(){
	static const GLfloat _unit_sphere_vertices[] = {-0.0,-1.0,-0.0,0.0,-0.866025,0.5,-0.0,-1.0,-0.0,0.270320,-0.866025,0.420627,-0.0,-1.0,-0.0,0.454816,-0.866025,0.207708,-0.0,-1.0,0.0,0.494911,-0.866025,-0.071157,-0.0,-1.0,0.0,0.377875,-0.866025,-0.327430,-0.0,-1.0,0.0,0.140866,-0.866025,-0.479746,0.0,-1.0,0.0,-0.140866,-0.866025,-0.479746,0.0,-1.0,0.0,-0.377875,-0.866025,-0.327430,0.0,-1.0,0.0,-0.494911,-0.866025,-0.071158,0.0,-1.0,-0.0,-0.454816,-0.866025,0.207708,0.0,-1.0,-0.0,-0.270320,-0.866025,0.420627,-0.0,-1.0,-0.0,0.0,-0.866025,0.5,0.0,-0.866025,0.5,0.0,-0.5,0.866025,0.270320,-0.866025,0.420627,0.468209,-0.5,0.728547,0.454816,-0.866025,0.207708,0.787764,-0.5,0.359760,0.494911,-0.866025,-0.071157,0.857211,-0.5,-0.123248,0.377875,-0.866025,-0.327430,0.654498,-0.5,-0.567126,0.140866,-0.866025,-0.479746,0.243988,-0.5,-0.830945,-0.140866,-0.866025,-0.479746,-0.243988,-0.5,-0.830945,-0.377875,-0.866025,-0.327430,-0.654498,-0.5,-0.567126,-0.494911,-0.866025,-0.071158,-0.857211,-0.5,-0.123248,-0.454816,-0.866025,0.207708,-0.787764,-0.5,0.359760,-0.270320,-0.866025,0.420627,-0.468209,-0.5,0.728547,0.0,-0.866025,0.5,0.0,-0.5,0.866025,0.0,-0.5,0.866025,0.0,0.0,1.0,0.468209,-0.5,0.728547,0.540641,0.0,0.841254,0.787764,-0.5,0.359760,0.909632,0.0,0.415415,0.857211,-0.5,-0.123248,0.989821,0.0,-0.142315,0.654498,-0.5,-0.567126,0.755750,0.0,-0.654861,0.243988,-0.5,-0.830945,0.281733,0.0,-0.959493,-0.243988,-0.5,-0.830945,-0.281733,0.0,-0.959493,-0.654498,-0.5,-0.567126,-0.755750,0.0,-0.654861,-0.857211,-0.5,-0.123248,-0.989821,0.0,-0.142315,-0.787764,-0.5,0.359760,-0.909632,0.0,0.415415,-0.468209,-0.5,0.728547,-0.540641,0.0,0.841253,0.0,-0.5,0.866025,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.5,0.866025,0.540641,0.0,0.841254,0.468209,0.5,0.728547,0.909632,0.0,0.415415,0.787764,0.5,0.359760,0.989821,0.0,-0.142315,0.857211,0.5,-0.123248,0.755750,0.0,-0.654861,0.654498,0.5,-0.567126,0.281733,0.0,-0.959493,0.243988,0.5,-0.830945,-0.281733,0.0,-0.959493,-0.243988,0.5,-0.830945,-0.755750,0.0,-0.654861,-0.654498,0.5,-0.567126,-0.989821,0.0,-0.142315,-0.857211,0.5,-0.123248,-0.909632,0.0,0.415415,-0.787764,0.5,0.359760,-0.540641,0.0,0.841253,-0.468209,0.5,0.728547,0.0,0.0,1.0,0.0,0.5,0.866025,0.0,0.5,0.866025,0.0,0.866025,0.5,0.468209,0.5,0.728547,0.270320,0.866025,0.420627,0.787764,0.5,0.359760,0.454816,0.866025,0.207708,0.857211,0.5,-0.123248,0.494911,0.866025,-0.071157,0.654498,0.5,-0.567126,0.377875,0.866025,-0.327430,0.243988,0.5,-0.830945,0.140866,0.866025,-0.479746,-0.243988,0.5,-0.830945,-0.140866,0.866025,-0.479746,-0.654498,0.5,-0.567126,-0.377875,0.866025,-0.327430,-0.857211,0.5,-0.123248,-0.494911,0.866025,-0.071158,-0.787764,0.5,0.359760,-0.454816,0.866025,0.207708,-0.468209,0.5,0.728547,-0.270320,0.866025,0.420627,0.0,0.5,0.866025,0.0,0.866025,0.5,0.0,0.866025,0.5,-0.0,1.0,-0.0,0.270320,0.866025,0.420627,-0.0,1.0,-0.0,0.454816,0.866025,0.207708,-0.0,1.0,-0.0,0.494911,0.866025,-0.071157,-0.0,1.0,0.0,0.377875,0.866025,-0.327430,-0.0,1.0,0.0,0.140866,0.866025,-0.479746,-0.0,1.0,0.0,-0.140866,0.866025,-0.479746,0.0,1.0,0.0,-0.377875,0.866025,-0.327430,0.0,1.0,0.0,-0.494911,0.866025,-0.071158,0.0,1.0,0.0,-0.454816,0.866025,0.207708,0.0,1.0,-0.0,-0.270320,0.866025,0.420627,0.0,1.0,-0.0,0.0,0.866025,0.5,-0.0,1.0,-0.0,-0.0,1.0,-0.0,-0.0,1.0,-0.0,0.0,0.0};
	static const GLfloat _unit_sphere_normals[] = {-0.0,-1.0,-0.0,0.0,-0.866025,0.5,-0.0,-1.0,-0.0,0.270320,-0.866025,0.420627,-0.0,-1.0,-0.0,0.454816,-0.866025,0.207708,-0.0,-1.0,0.0,0.494911,-0.866025,-0.071157,-0.0,-1.0,0.0,0.377875,-0.866025,-0.327430,-0.0,-1.0,0.0,0.140866,-0.866025,-0.479746,0.0,-1.0,0.0,-0.140866,-0.866025,-0.479746,0.0,-1.0,0.0,-0.377875,-0.866025,-0.327430,0.0,-1.0,0.0,-0.494911,-0.866025,-0.071158,0.0,-1.0,-0.0,-0.454816,-0.866025,0.207708,0.0,-1.0,-0.0,-0.270320,-0.866025,0.420627,-0.0,-1.0,-0.0,0.0,-0.866025,0.5,0.0,-0.866025,0.5,0.0,-0.5,0.866025,0.270320,-0.866025,0.420627,0.468209,-0.5,0.728547,0.454816,-0.866025,0.207708,0.787764,-0.5,0.359760,0.494911,-0.866025,-0.071157,0.857211,-0.5,-0.123248,0.377875,-0.866025,-0.327430,0.654498,-0.5,-0.567126,0.140866,-0.866025,-0.479746,0.243988,-0.5,-0.830945,-0.140866,-0.866025,-0.479746,-0.243988,-0.5,-0.830945,-0.377875,-0.866025,-0.327430,-0.654498,-0.5,-0.567126,-0.494911,-0.866025,-0.071158,-0.857211,-0.5,-0.123248,-0.454816,-0.866025,0.207708,-0.787764,-0.5,0.359760,-0.270320,-0.866025,0.420627,-0.468209,-0.5,0.728547,0.0,-0.866025,0.5,0.0,-0.5,0.866025,0.0,-0.5,0.866025,0.0,0.0,1.0,0.468209,-0.5,0.728547,0.540641,0.0,0.841254,0.787764,-0.5,0.359760,0.909632,0.0,0.415415,0.857211,-0.5,-0.123248,0.989821,0.0,-0.142315,0.654498,-0.5,-0.567126,0.755750,0.0,-0.654861,0.243988,-0.5,-0.830945,0.281733,0.0,-0.959493,-0.243988,-0.5,-0.830945,-0.281733,0.0,-0.959493,-0.654498,-0.5,-0.567126,-0.755750,0.0,-0.654861,-0.857211,-0.5,-0.123248,-0.989821,0.0,-0.142315,-0.787764,-0.5,0.359760,-0.909632,0.0,0.415415,-0.468209,-0.5,0.728547,-0.540641,0.0,0.841253,0.0,-0.5,0.866025,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.5,0.866025,0.540641,0.0,0.841254,0.468209,0.5,0.728547,0.909632,0.0,0.415415,0.787764,0.5,0.359760,0.989821,0.0,-0.142315,0.857211,0.5,-0.123248,0.755750,0.0,-0.654861,0.654498,0.5,-0.567126,0.281733,0.0,-0.959493,0.243988,0.5,-0.830945,-0.281733,0.0,-0.959493,-0.243988,0.5,-0.830945,-0.755750,0.0,-0.654861,-0.654498,0.5,-0.567126,-0.989821,0.0,-0.142315,-0.857211,0.5,-0.123248,-0.909632,0.0,0.415415,-0.787764,0.5,0.359760,-0.540641,0.0,0.841253,-0.468209,0.5,0.728547,0.0,0.0,1.0,0.0,0.5,0.866025,0.0,0.5,0.866025,0.0,0.866025,0.5,0.468209,0.5,0.728547,0.270320,0.866025,0.420627,0.787764,0.5,0.359760,0.454816,0.866025,0.207708,0.857211,0.5,-0.123248,0.494911,0.866025,-0.071157,0.654498,0.5,-0.567126,0.377875,0.866025,-0.327430,0.243988,0.5,-0.830945,0.140866,0.866025,-0.479746,-0.243988,0.5,-0.830945,-0.140866,0.866025,-0.479746,-0.654498,0.5,-0.567126,-0.377875,0.866025,-0.327430,-0.857211,0.5,-0.123248,-0.494911,0.866025,-0.071158,-0.787764,0.5,0.359760,-0.454816,0.866025,0.207708,-0.468209,0.5,0.728547,-0.270320,0.866025,0.420627,0.0,0.5,0.866025,0.0,0.866025,0.5,0.0,0.866025,0.5,-0.0,1.0,-0.0,0.270320,0.866025,0.420627,-0.0,1.0,-0.0,0.454816,0.866025,0.207708,-0.0,1.0,-0.0,0.494911,0.866025,-0.071157,-0.0,1.0,0.0,0.377875,0.866025,-0.327430,-0.0,1.0,0.0,0.140866,0.866025,-0.479746,-0.0,1.0,0.0,-0.140866,0.866025,-0.479746,0.0,1.0,0.0,-0.377875,0.866025,-0.327430,0.0,1.0,0.0,-0.494911,0.866025,-0.071158,0.0,1.0,0.0,-0.454816,0.866025,0.207708,0.0,1.0,-0.0,-0.270320,0.866025,0.420627,0.0,1.0,-0.0,0.0,0.866025,0.5,-0.0,1.0,-0.0,-0.0,1.0,-0.0,-0.0,1.0,-0.0,0.0,0.0};
	static const GLfloat _unit_sphere_texture[] = {1.0,0.0,1.0,0.166667,0.909091,0.0,0.909091,0.166667,0.818182,0.0,0.818182,0.166667,0.727273,0.0,0.727273,0.166667,0.636364,0.0,0.636364,0.166667,0.545455,0.0,0.545455,0.166667,0.454545,0.0,0.454545,0.166667,0.363636,0.0,0.363636,0.166667,0.272727,0.0,0.272727,0.166667,0.181818,0.0,0.181818,0.166667,0.090909,0.0,0.090909,0.166667,0.0,0.0,0.0,0.166667,1.0,0.166667,1.0,0.333333,0.909091,0.166667,0.909091,0.333333,0.818182,0.166667,0.818182,0.333333,0.727273,0.166667,0.727273,0.333333,0.636364,0.166667,0.636364,0.333333,0.545455,0.166667,0.545455,0.333333,0.454545,0.166667,0.454545,0.333333,0.363636,0.166667,0.363636,0.333333,0.272727,0.166667,0.272727,0.333333,0.181818,0.166667,0.181818,0.333333,0.090909,0.166667,0.090909,0.333333,0.0,0.166667,0.0,0.333333,1.0,0.333333,1.0,0.5,0.909091,0.333333,0.909091,0.5,0.818182,0.333333,0.818182,0.5,0.727273,0.333333,0.727273,0.5,0.636364,0.333333,0.636364,0.5,0.545455,0.333333,0.545455,0.5,0.454545,0.333333,0.454545,0.5,0.363636,0.333333,0.363636,0.5,0.272727,0.333333,0.272727,0.5,0.181818,0.333333,0.181818,0.5,0.090909,0.333333,0.090909,0.5,0.0,0.333333,0.0,0.5,1.0,0.5,1.0,0.666667,0.909091,0.5,0.909091,0.666667,0.818182,0.5,0.818182,0.666667,0.727273,0.5,0.727273,0.666667,0.636364,0.5,0.636364,0.666667,0.545455,0.5,0.545455,0.666667,0.454545,0.5,0.454545,0.666667,0.363636,0.5,0.363636,0.666667,0.272727,0.5,0.272727,0.666667,0.181818,0.5,0.181818,0.666667,0.090909,0.5,0.090909,0.666667,0.0,0.5,0.0,0.666667,1.0,0.666667,1.0,0.833333,0.909091,0.666667,0.909091,0.833333,0.818182,0.666667,0.818182,0.833333,0.727273,0.666667,0.727273,0.833333,0.636364,0.666667,0.636364,0.833333,0.545455,0.666667,0.545455,0.833333,0.454545,0.666667,0.454545,0.833333,0.363636,0.666667,0.363636,0.833333,0.272727,0.666667,0.272727,0.833333,0.181818,0.666667,0.181818,0.833333,0.090909,0.666667,0.090909,0.833333,0.0,0.666667,0.0,0.833333,1.0,0.833333,1.0,1.0,0.909091,0.833333,0.909091,1.0,0.818182,0.833333,0.818182,1.0,0.727273,0.833333,0.727273,1.0,0.636364,0.833333,0.636364,1.0,0.545455,0.833333,0.545455,1.0,0.454545,0.833333,0.454545,1.0,0.363636,0.833333,0.363636,1.0,0.272727,0.833333,0.272727,1.0,0.181818,0.833333,0.181818,1.0,0.090909,0.833333,0.090909,1.0,0.0,0.833333,0.0,1.0,0.0,1.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
	glVertexPointer(3, GL_FLOAT, 0, _unit_sphere_vertices);
	glNormalPointer(GL_FLOAT, 0, _unit_sphere_normals);
	glTexCoordPointer(2, GL_FLOAT, 0, _unit_sphere_texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 147);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
/////////////////////////        SCENES         //////////////////////////

void drawCheckerboard(float walkX, float walkY, int numSquares){
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	// if even split
	if(numSquares%2 == 0){
		for(int i = -numSquares*.5; i <= numSquares*.5; i++){
			for(int j = -numSquares*.5; j <= numSquares*.5; j++){
				int b = abs(((i+j+XOffset+YOffset)%2));
				if(b) glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);
				drawUnitSquare(i-XOffset, j-YOffset);
			}
		}
	}
// if odd number
	else{
		numSquares--;
		for(int i = -numSquares*.5; i <= numSquares*.5; i++){
			for(int j = -numSquares*.5; j <= numSquares*.5; j++){
				int b = abs(((i+j+XOffset+YOffset)%2));
				if(b) glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);
				drawUnitSquare(i-XOffset - .5, j-YOffset - .5);
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
				glColor3f(brightness, brightness, brightness);
				// glLineWidth(100.0/distance/distance);
				drawUnitAxis(i + XSpanMod - walkX,
				             j + YSpanMod - walkY,
				             k + ZSpanMod - walkZ, 1.0);
			}
		}
	}
}
void drawZoomboard(float zoom){
	glPushMatrix();
	glScalef(zoom, zoom, zoom);
	for(int a = -5; a < 8; a++){
		glPushMatrix();
		// glScalef(1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a), 1.0/powf(ZOOM_RADIX,a));
		glScalef(1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX, 1.0f/ZOOM_RADIX);
		drawCheckerboard(0,0,ZOOM_RADIX);
		glPopMatrix();
		// for(int i = -1; i <= 1; i++){
		// 	for(int j = -1; j <= 1; j++){
		// 		int b = abs(((i+j)%2));
		// 		if(b) glColor3f(1.0, 1.0, 1.0);
		// 		else glColor3f(0.0, 0.0, 0.0);
		// 		if(!(i == 0 && j == 0))
		// 			drawUnitSquare(i-.5, j-.5, 1.0/powf(3,a), 1.0/powf(3,a));
		// 	}
		// }
	}
	glPopMatrix();
}

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
// multiply two 3x3 matrices, a and b, store result into c
void mat3x3Mult(const float* a, const float* b, float* c) {
	c[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
	c[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
	c[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];
	c[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	c[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	c[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
	c[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	c[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	c[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
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
#endif /* WORLD_FRAMEWORK */
