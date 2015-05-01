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

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
// TABLE OF CONTENTS
//
// START
void init();  // contains glEnable calls and general setup
void reshape(int w, int h);  // contains viewport and frustum calls
// SHAPES
void unitSquare(float x, float y, float width, float height);
void unitAxis(float x, float y, float z, float scale);
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawAxesGrid(float walkX, float walkY, int span, int repeats);
// DRAW, ALIGNMENT, INPUT HANDLING
void display();
void update();  // process input devices
// INPUT DEVICES
void mouseButtons(int button, int state, int x, int y);  // when mouse button state changes
void mouseMotion(int x, int y);   // when mouse is dragging screen
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key,int x,int y);
//
int main(int argc, char **argv);

////////////////////////////////////////////////////
//
// CHOOSE YOUR OWN PERSPECTIVE
//
// first persion perspective, polar, orthographic
typedef enum{ 
	FPP,  POLAR,  ORTHO 
} perspective_t;

static perspective_t POV = FPP;

// size of window in OS
static int WIDTH = 800;
static int HEIGHT = 400;
static unsigned char FULLSCREEN = 0;  // fullscreen:1   window:0
// INPUT HANDLING
static unsigned int UP_PRESSED = 0;    // KEY UP:0   KEY DOWN:1
static unsigned int DOWN_PRESSED = 0;
static unsigned int RIGHT_PRESSED = 0;
static unsigned int LEFT_PRESSED = 0;
static unsigned int PLUS_PRESSED = 0;
static unsigned int MINUS_PRESSED = 0;
static unsigned int COMMA_PRESSED = 0;
static unsigned int PERIOD_PRESSED = 0;
// set default rotation here:
// how far mouse has been dragged
static int mouseTotalOffsetX = 0;  // since program began
static int mouseTotalOffsetY = 0;
static int mouseDragOffsetX = 0;  // dragging during one session (between click and release)
static int mouseDragOffsetY = 0;
static int mouseDragStartX, mouseDragStartY, mouseTotalOffsetStartX, mouseTotalOffsetStartY;
// PERSPECTIVES
static float walkX = 0.0f;
static float walkY = 0.0f;
static float polarRadius = 15.0f;  // POLAR PERSPECTIVE
static unsigned char landscape = 0;  // checkerboard or axes lines
// zoom scale, converted to logarithmic
static float ZOOM = 0.0;


#define STEP .10f  // WALKING SPEED. @60fps, walk speed = 6 units/second
#define MOUSE_SENSITIVITY 0.333f


void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
	glLineWidth(1);
}

void reshape(int w, int h){
	WIDTH = w;
	HEIGHT = h;
	float a = (float)WIDTH / HEIGHT;
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(POV == FPP || POV == POLAR)
		glFrustum (-.1, .1, -.1/a, .1/a, .1, 100.0);
	else if (POV == ORTHO)
		glOrtho(-20.0f, 20.0f, 
				-20.0f/a, 20.0f/a, 
				-100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// draws a XY 1x1 square in the Z = 0 plane
void unitSquare(float x, float y, float width, float height){
	static const GLfloat _unit_square_vertex[] = { 
		0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f };
	static const GLfloat _unit_square_normals[] = { 
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f };
	glPushMatrix();
	glScalef(width, height, 1.0);
	glTranslatef(x, y, 0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _unit_square_vertex);
	glNormalPointer(GL_FLOAT, 0, _unit_square_normals);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void unitAxis(float x, float y, float z, float scale){
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

void drawCheckerboard(float walkX, float walkY, int numSquares){
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	for(int i = -numSquares; i <= numSquares; i++){
		for(int j = -numSquares; j <= numSquares; j++){
			int b = abs(((i+j+XOffset+YOffset)%2));
			if(b) glColor3f(1.0, 1.0, 1.0);
			else glColor3f(0.0, 0.0, 0.0);
			unitSquare(i-XOffset, j-YOffset, 1, 1);
		}
	}
}

void drawZoomboard(float zoom){
	glPushMatrix();
	glScalef(zoom, zoom, zoom);
	for(int a = -5; a < 8; a++){
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j <= 1; j++){
				int b = abs(((i+j)%2));
				if(b) glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);
				if(!(i == 0 && j == 0))
					unitSquare(i-.5, j-.5, 1.0/powf(3,a), 1.0/powf(3,a));
			}
		}
	}
	glPopMatrix();
}

// span: how many units to skip inbetween each axis
// repeats: how many rows/cols/stacks on either side of center 
void drawAxesGrid(float walkX, float walkY, int span, int repeats){
	float XSpanMod = walkX - floor(walkX/span)*span;
	float YSpanMod = walkY - floor(walkY/span)*span;
	for(int i = -repeats*span; i < repeats*span; i+=span){
		for(int j = -repeats*span; j < repeats*span; j+=span){
			for(int k = -repeats*span; k < repeats*span; k+=span){
				// distance approximation works just fine in this case
				float distance = fabs(i+XSpanMod-1) + fabs(j+YSpanMod-1) + abs(k);
				float brightness = 1.0 - distance/(repeats*span);
				glColor3f(brightness, brightness, brightness);
				// glLineWidth(100.0/distance/distance);
				unitAxis(i + XSpanMod - walkX, j + YSpanMod - walkY, k, 1.0f);
			}
		}
	}
}

float modulusContext(float complete, int modulus){
	double wholePart;
	double fracPart = modf(complete, &wholePart);
	return ( ((int)wholePart) % modulus ) + fracPart;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
		// SETUP PERSPECTIVE
		switch(POV){
			case FPP:
				glRotatef(mouseTotalOffsetY * MOUSE_SENSITIVITY, -1, 0, 0);
				glRotatef(mouseTotalOffsetX * MOUSE_SENSITIVITY, 0, 0, -1);
				// raise POV 1.0 above the floor, 1.0 is an arbitrary value
				glTranslatef(0.0f, 0.0f, -1.0f);
				break;
		
			case POLAR:
				glTranslatef(0, 0, -polarRadius);
				glRotatef(mouseTotalOffsetY * MOUSE_SENSITIVITY, -1, 0, 0);
				glRotatef(mouseTotalOffsetX * MOUSE_SENSITIVITY, 0, 0, -1);
				break;

			case ORTHO:
				glTranslatef(-mouseTotalOffsetX * .05, mouseTotalOffsetY * .05, 0.0f);
		}

		// perspective has been established
		// draw stuff below

		glPushMatrix();
			// CHECKERBOARD LANDSCAPE
			if(landscape == 0){
				float newX = modulusContext(walkX, 2);
				float newY = modulusContext(walkY, 2);
				glTranslatef(newX, newY, 0);
				drawCheckerboard(newX, newY, 8);
			}
			// 3 DIMENSIONS OF SCATTERED AXES
			else if(landscape == 1){
				float newX = modulusContext(walkX, 5);
				float newY = modulusContext(walkY, 5);
				glTranslatef(newX, newY, 0);
				drawAxesGrid(newX, newY, 5, 4);
			}
			else if(landscape == 2){
				// float newX = modulusContext(walkX, 3);
				// float newY = modulusContext(walkY, 3);
				glTranslatef(walkX, walkY, 0);
				static double intpart;
				float zoom = powf(3,modf(ZOOM, &intpart));
				drawZoomboard(zoom);
			}
		glPopMatrix();

	glPopMatrix();
	// bring back buffer to the front on vertical refresh, auto-calls glFlush
	glutSwapBuffers();
	// glFlush();
}

// process input devices if in first person perspective mode
void update(){
	float lookAzimuth = 0;
	// map movement direction to the direction the person is facing
	if(POV == FPP)
		lookAzimuth = (mouseTotalOffsetX * MOUSE_SENSITIVITY)/180.*M_PI;

	if(UP_PRESSED){
		walkX += STEP * sinf(lookAzimuth);
		walkY += STEP * -cosf(lookAzimuth);
	}
	if(DOWN_PRESSED){
		walkX -= STEP * sinf(lookAzimuth);
		walkY -= STEP * -cosf(lookAzimuth);
	}
	if(LEFT_PRESSED){
		walkX -= STEP * sinf(lookAzimuth+M_PI_2);
		walkY -= STEP * -cosf(lookAzimuth+M_PI_2);
	}
	if(RIGHT_PRESSED){
		walkX += STEP * sinf(lookAzimuth+M_PI_2);
		walkY += STEP * -cosf(lookAzimuth+M_PI_2);
	}
	if(MINUS_PRESSED)
		polarRadius += STEP;
	if(PLUS_PRESSED){
		polarRadius -= STEP;
		if(polarRadius < 0) 
			polarRadius = 0;
	}
	if(COMMA_PRESSED)
		ZOOM -= STEP * .25;
	if(PERIOD_PRESSED)
		ZOOM += STEP * .25;


	glutPostRedisplay();
}

// when mouse button state changes
void mouseButtons(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(!state){  // button down
			mouseDragStartX = x;
			mouseDragStartY = y;
			mouseTotalOffsetStartX = mouseTotalOffsetX;
			mouseTotalOffsetStartY = mouseTotalOffsetY;
		}
		else { }    // button up
	}
	// else if(button == GLUT_MIDDLE_BUTTON){}
	// else if(button == GLUT_RIGHT_BUTTON){}
}

// when mouse is dragging screen
void mouseMotion(int x, int y){
	mouseDragOffsetX = mouseDragStartX - x;
	mouseDragOffsetY = mouseDragStartY - y;
	mouseTotalOffsetX = mouseTotalOffsetStartX + mouseDragOffsetX;
	mouseTotalOffsetY = mouseTotalOffsetStartY + mouseDragOffsetY;
	glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y){
	if(key == 27)  // ESCAPE key
		exit (0);
	// W A S D
	else if(key == 119) // W
		UP_PRESSED = 1;
	else if(key == 115) // S
		DOWN_PRESSED = 1;
	else if(key == 97) // A
		RIGHT_PRESSED = 1;
	else if(key == 100)  // D
		LEFT_PRESSED = 1;
	else if(key == '+' || key == '=') // PLUS
		PLUS_PRESSED = 1;
	else if(key == '-' || key == '_') // MINUS
		MINUS_PRESSED = 1;
	else if(key == ',' || key == '<') // COMMA
	// else if(key == 'n') // COMMA
		COMMA_PRESSED = 1;
	else if(key == '.' || key == '>') // PERIOD
	// else if(key == 'm') // PERIOD
		PERIOD_PRESSED = 1;
	else if(key == ' '){  // SPACE BAR
		landscape = (landscape+1)%3;
		if(landscape == 2)
			walkX = walkY = 0.0f;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == 'f'){
		if(!FULLSCREEN)
			glutFullScreen();
		else{
			reshape(WIDTH, HEIGHT);
        	glutPositionWindow(0,0);
		}
		FULLSCREEN = !FULLSCREEN;
	}
	else if(key == '1'){
		POV = FPP;
		// mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == '2'){
		POV = POLAR;
		// mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == '3'){
		POV = ORTHO;
		mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();	
	}
	
	// anything that affects the screen and requires a redisplay
	// put it in this if statement
	if(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED || PLUS_PRESSED || MINUS_PRESSED || PERIOD_PRESSED || COMMA_PRESSED){
		glutIdleFunc(update);
	}
}

void keyboardUp(unsigned char key,int x,int y){
	if(key == 119) // W
		UP_PRESSED = 0;
	else if(key == 115) // S
		DOWN_PRESSED = 0;
	else if(key == 97) // A
		RIGHT_PRESSED = 0;
	else if(key == 100) // D
		LEFT_PRESSED = 0;
	else if(key == '+' || key == '=') // PLUS
		PLUS_PRESSED = 0;
	else if(key == '-' || key == '_') // MINUS
		MINUS_PRESSED = 0;
	else if(key == ',' || key == '<') // COMMA
	// else if(key == 'n') // COMMA
		COMMA_PRESSED = 0;
	else if(key == '.' || key == '>') // PERIOD
	// else if(key == 'm') // PERIOD
		PERIOD_PRESSED = 0;
	
	if(!(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED || PLUS_PRESSED || MINUS_PRESSED || PERIOD_PRESSED || COMMA_PRESSED))
		glutIdleFunc(NULL);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButtons);
	glutMotionFunc(mouseMotion);
	glutKeyboardUpFunc(keyboardUp); 
	glutKeyboardFunc(keyboardDown);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}