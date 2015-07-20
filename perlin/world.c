#ifdef __APPLE__
// #  include "/usr/local/Cellar/glew/1.12.0/include/GL/glew.h"
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "3rdParty/noise.c"

////////////////////////////////////////////////////
//
// TABLE OF CONTENTS
//
int main(int argc, char **argv);
// INITIALIZE
void initOpenGL();  // contains glEnable calls and general setup
// void initShaders();
void reshape(int w, int h);  // contains viewport and frustum calls
// DRAW, ALIGNMENT, INPUT HANDLING
void display();
void update();  // process input devices
// INPUT DEVICES
void mouseButtons(int button, int state, int x, int y);  // when mouse button state changes
void mouseMotion(int x, int y);   // when mouse is dragging screen
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key,int x,int y);
// WORLD SHAPES
void unitSquare(float x, float y, float width, float height);
void unitAxis(float x, float y, float z, float scale);
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats);
void drawCheckerboard(float walkX, float walkY, int numSquares);
void drawZoomboard(float zoom);
float modulusContext(float complete, int modulus);

////////////////////////////////////////////////////
// MANAGE PERSPECTIVE
//     first persion perspective, polar, orthographic
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
static unsigned int FLOAT_UP_PRESSED = 0;
static unsigned int FLOAT_DOWN_PRESSED = 0;
// set initial rotation here:
// how far mouse has been dragged
static int mouseTotalOffsetX = 0;  // since program began
static int mouseTotalOffsetY = 0;
static int mouseDragOffsetX = 0;  // dragging during one session (between click and release)
static int mouseDragOffsetY = 0;
static int mouseDragStartX, mouseDragStartY, mouseTotalOffsetStartX, mouseTotalOffsetStartY;
// PERSPECTIVES
static float originX = 0.0f;
static float originY = 0.0f;
static float originZ = 0.0f;
static unsigned char landscape = 0;  // checkerboard or axes lines
// zoom scale, converted to logarithmic
static float ZOOM = 15.0f;   // POLAR PERSPECTIVE

#define STEP 1.0f  // WALKING SPEED. @60fps, walk speed = 6 units/second
#define MOUSE_SENSITIVITY 0.333f


// PERLIN LANDSCAPE
void buildWorld();
void drawLandscape();
static int LAND_WIDTH = 800;
static int LAND_HEIGHT = 800;
static float *_points;
static uint32_t *_indices;
static float *_colors;
static unsigned int _numPoints;
static unsigned int _numIndices;
#define ZSCALE 100.0

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow(argv[0]);
	initOpenGL();
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

void initOpenGL(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_FLAT);
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// initShaders();

	_points = (float*)malloc(sizeof(float) * LAND_WIDTH*LAND_HEIGHT * 3);
	_indices = (uint32_t*)malloc(sizeof(uint32_t) * 2*(LAND_WIDTH-1)*(LAND_HEIGHT-1) * 3);
	_colors = (float*)malloc(sizeof(float) * LAND_WIDTH*LAND_HEIGHT * 3);
	buildWorld();
}

/*
void initShaders(){
	if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite")){
		printf("OpenGL 1.4 + point sprites is available on this system\n");
	}

	GLuint shader_num_v, shader_num_f;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		// Problem: glewInit failed, something is seriously wrong.
		printf("Error: %s\n", glewGetErrorString(err));
	}
	printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	if (GLEW_ARB_vertex_program)
	{
		glEnable(GL_VERTEX_PROGRAM_ARB);
		glGenProgramsARB(1, &shader_num_v);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, shader_num_v);
		printf("vertex shaders loaded.\n");
	}
	if (GLEW_ARB_fragment_program)
	{
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		glGenProgramsARB(1, &shader_num_f);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, shader_num_f);
		printf("fragment shaders loaded.\n");
	}

	GLint vertexShaderObject, fragmentShaderObject;
	GLint vlength, flength;

	const char *VertexShaderSource = "void main(void)\n{\nvec4 a = gl_Vertex;\na.x = a.x * 1.0;\na.y = a.y * 1.0;\na.z = a.z * 1.0;\ngl_Position = gl_ModelViewProjectionMatrix * a;\n}";

	const char *FragmentShaderSource = "void main (void)\n{\ngl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n}";

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
	if(compiled)
		printf("vertex shader successfully compiled\n");
	glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if(compiled)
		printf("fragment shader successfully compiled\n");
	
	GLhandleARB program;
	//Create a program handle.
	program = glCreateProgramObjectARB();
	//Attach the shaders. Here, assume that fragmentHandle is a handle to a fragment shader object, 
	//and that vertexHandle is a handle to a vertex shader object.
	glAttachObjectARB(program, fragmentShaderObject);
	glAttachObjectARB(program, vertexShaderObject);
	//Link the program.
	glLinkProgramARB(program);

	glUseProgramObjectARB(program);
}
*/

void reshape(int w, int h){
	WIDTH = w;
	HEIGHT = h;
	float a = (float)WIDTH / HEIGHT;
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(POV == FPP || POV == POLAR)
		glFrustum(-1, 1, -1/a, 1/a, 1, 1000.0);
	else if (POV == ORTHO)
		glOrtho(-ZOOM, ZOOM, 
				-ZOOM/a, ZOOM/a, 
				-100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
				glTranslatef(0, 0, -ZOOM);
				glRotatef(mouseTotalOffsetY * MOUSE_SENSITIVITY, -1, 0, 0);
				glRotatef(mouseTotalOffsetX * MOUSE_SENSITIVITY, 0, 0, -1);
				break;

			case ORTHO:
				glTranslatef(-mouseTotalOffsetX * (ZOOM/400.0f), mouseTotalOffsetY * (ZOOM/400.0f), 0.0f);
				break;
		}

		// perspective has been established
		// draw stuff below


		glPushMatrix();
			if(landscape == 0)
				glTranslatef(originX, originY, originZ);
			drawLandscape();
		glPopMatrix();


		glPushMatrix();
			// 2 DIMENSIONAL REPEATED MATERIAL
			// CHECKERBOARD
			if(landscape == 0){
				float newX = modulusContext(originX, 2);
				float newY = modulusContext(originY, 2);
				glTranslatef(newX, newY, originZ);
				// drawCheckerboard(newX, newY, 8);
			}
			// 3 DIMENSIONAL OF REPEATED MATERIAL
			// SCATTERED AXES
			else if(landscape == 1){
				float newX = modulusContext(originX, 5);
				float newY = modulusContext(originY, 5);
				float newZ = modulusContext(originZ, 5);
				glTranslatef(newX, newY, newZ);
				drawAxesGrid(newX, newY, newZ, 5, 4);
			}
			else if(landscape == 2){
				static double intpart;
				float zoom = powf(3,modf(-originY, &intpart));
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
		originX += STEP * sinf(lookAzimuth);
		originY += STEP * -cosf(lookAzimuth);
	}
	if(DOWN_PRESSED){
		originX -= STEP * sinf(lookAzimuth);
		originY -= STEP * -cosf(lookAzimuth);
	}
	if(LEFT_PRESSED){
		originX -= STEP * sinf(lookAzimuth+M_PI_2);
		originY -= STEP * -cosf(lookAzimuth+M_PI_2);
	}
	if(RIGHT_PRESSED){
		originX += STEP * sinf(lookAzimuth+M_PI_2);
		originY += STEP * -cosf(lookAzimuth+M_PI_2);
	}
	if(FLOAT_UP_PRESSED){
		originZ -= STEP;
	}
	if(FLOAT_DOWN_PRESSED){
		originZ += STEP;
	}
	if(MINUS_PRESSED){
		ZOOM += STEP;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	if(PLUS_PRESSED){
		ZOOM -= STEP;
		if(ZOOM < 0) 
			ZOOM = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	// if(COMMA_PRESSED)
	// 	ZOOM -= STEP * .25;
	// if(PERIOD_PRESSED)
	// 	ZOOM += STEP * .25;


	glutPostRedisplay();
}


// draws a XY 1x1 square in the Z = 0 plane
void unitSquare(float x, float y, float WIDTH, float HEIGHT){
	static const GLfloat _unit_square_vertex[] = { 
		0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f };
	static const GLfloat _unit_square_normals[] = { 
		0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f };
	glPushMatrix();
	glScalef(WIDTH, HEIGHT, 1.0);
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
				unitAxis(i + XSpanMod - walkX, 
					     j + YSpanMod - walkY, 
					     k + ZSpanMod - walkZ, 1.0);
			}
		}
	}
}

void drawLandscape(){
	glPushMatrix();
		glScalef(-1.0f, 1.0f, .10f);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(0.5f, 1.0f, 0.5f);
		glColorPointer(3, GL_FLOAT, 0, _colors);
		glVertexPointer(3, GL_FLOAT, 0, _points);
		// glDrawArrays(GL_POINTS, 0, _numPoints);
		glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, _indices);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	glPopMatrix();
}

float modulusContext(float complete, int modulus){
	double wholePart;
	double fracPart = modf(complete, &wholePart);
	return ( ((int)wholePart) % modulus ) + fracPart;
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
	else if(key == 113)  // Q
		FLOAT_UP_PRESSED = 1;
	else if (key == 122) // Z
		FLOAT_DOWN_PRESSED = 1;
	else if(key == '+' || key == '=') // PLUS
		PLUS_PRESSED = 1;
	else if(key == '-' || key == '_') // MINUS
		MINUS_PRESSED = 1;
	else if(key == ',' || key == '<') // COMMA
		COMMA_PRESSED = 1;
	else if(key == '.' || key == '>') // PERIOD
		PERIOD_PRESSED = 1;
	else if(key == ' '){  // SPACE BAR
		buildWorld();
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == 'l'){  // L
		landscape = !landscape;//(landscape+1)%3;
		// if(landscape == 2)
		// 	walkX = walkY = 0.0f;
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
		originX = originY = originZ = 0.0f;
		// mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == '2'){
		POV = POLAR;
		originX = originY = originZ = 0.0f;
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
	if(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED || PLUS_PRESSED || MINUS_PRESSED || PERIOD_PRESSED || COMMA_PRESSED || FLOAT_UP_PRESSED || FLOAT_DOWN_PRESSED){
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
	else if(key == 113)  // Q
		FLOAT_UP_PRESSED = 0;
	else if (key == 122) // Z
		FLOAT_DOWN_PRESSED = 0;
	else if(key == '+' || key == '=') // PLUS
		PLUS_PRESSED = 0;
	else if(key == '-' || key == '_') // MINUS
		MINUS_PRESSED = 0;
	else if(key == ',' || key == '<') // COMMA
		COMMA_PRESSED = 0;
	else if(key == '.' || key == '>') // PERIOD
		PERIOD_PRESSED = 0;
	
	if(!(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED || PLUS_PRESSED || MINUS_PRESSED || PERIOD_PRESSED || COMMA_PRESSED || FLOAT_UP_PRESSED || FLOAT_DOWN_PRESSED))
		glutIdleFunc(NULL);
}

void buildWorld(){
	
	#define START_FREQ .002

	float freq = START_FREQ;
	// smaller dents

	// set X Y Z back to 0.0
	memset(_points, 0, sizeof(float)*LAND_HEIGHT*LAND_WIDTH*3);
	// set X Y to the grid
	for(int h = 0; h < LAND_HEIGHT; h++){
		for(int w = 0; w < LAND_WIDTH; w++){
			_points[(h*LAND_WIDTH+w)*3+0] = (w - LAND_WIDTH*.5) * .1;         // x
			_points[(h*LAND_WIDTH+w)*3+1] = (h - LAND_HEIGHT*.5) * .1;        // y
		}
	}

	for(int loop = 0; loop < 20; loop++){
		freq *= 2;
		float RX = rand()%1000/500.0 - 1.0;
		float RY = rand()%1000/500.0 - 1.0;
		for(int h = 0; h < LAND_HEIGHT; h++){
			for(int w = 0; w < LAND_WIDTH; w++){
				float vec[2] = {w*freq + RX, h*freq + RY};
				_points[(h*LAND_WIDTH+w)*3+2] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ);///1000.0;    // z, convert meters to km
			}
		}
	}

// these next 2 blocks distort along X and Y,
// potentially makes the land non-continuous, and may create bad geometry
	//////////////////////////////////////////////////////
	freq = START_FREQ*.5;
	for(int loop = 0; loop < 4; loop++){
		freq *= 2;
		float RX = rand()%1000/500.0 - 1.0;
		float RY = rand()%1000/500.0 - 1.0;
		for(int h = 0; h < LAND_HEIGHT; h++){
			for(int w = 0; w < LAND_WIDTH; w++){
				float vec[2] = {w*freq + RX, h*freq + RY};
				_points[(h*LAND_WIDTH+w)*3+0] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ*12);///1000.0;    // z, convert meters to km
			}
		}
	}
	freq = START_FREQ*.5;
	for(int loop = 0; loop < 4; loop++){
		freq *= 2;
		float RX = rand()%1000/500.0 - 1.0;
		float RY = rand()%1000/500.0 - 1.0;
		for(int h = 0; h < LAND_HEIGHT; h++){
			for(int w = 0; w < LAND_WIDTH; w++){
				float vec[2] = {w*freq + RX, h*freq + RY};
				_points[(h*LAND_WIDTH+w)*3+1] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ*12);///1000.0;    // z, convert meters to km
			}
		}
	}
	
	/////////////////////////////////////////////////////////
	// inside INDICES, (WIDTH-1) and (HEIGHT-1) are max
	// inside POINTS, WIDTH and HEIGHT are max
	for(int h = 0; h < LAND_HEIGHT-1; h++){
		for(int w = 0; w < LAND_WIDTH-1; w++){
			_indices[(h*(LAND_WIDTH-1)+w)*6+0] = 1*(h*LAND_WIDTH+w);
			_indices[(h*(LAND_WIDTH-1)+w)*6+1] = 1*((h+1)*LAND_WIDTH+w);
			_indices[(h*(LAND_WIDTH-1)+w)*6+2] = 1*(h*LAND_WIDTH+w+1);
			_indices[(h*(LAND_WIDTH-1)+w)*6+3] = 1*((h+1)*LAND_WIDTH+w);
			_indices[(h*(LAND_WIDTH-1)+w)*6+4] = 1*((h+1)*LAND_WIDTH+w+1);
			_indices[(h*(LAND_WIDTH-1)+w)*6+5] = 1*(h*LAND_WIDTH+w+1);
		}
	}
	
	for(int i = 0; i < LAND_WIDTH*LAND_HEIGHT; i++){
		// float RANDOM_WATER_LEVEL = rand()%100/500.0;
		float scale = (.5*ZSCALE + _points[i*3+2]) / ZSCALE;// - RANDOM_WATER_LEVEL;
		if(scale < 0.0) scale = 0.0;
		if(scale > 1.0) scale = 1.0;

		// below .3
		if(scale < .3){
			_colors[i*3+0] = 0.0f    + 1.0 * scale;
			_colors[i*3+1] = 0.24f   + .76 * scale;
			_colors[i*3+2] = 0.666f  + .334 * scale;
		}
		// .8333  to  1.0
		else if(scale > .8333){
		    float white = (scale-.83333)/0.166666666667;//(scale-.7);
		// else if(data[i] > 1200){
		//     float white = (data[i]-1200) / 500.0;
		// else if(data[i] > 900){
		//     float white = (data[i]-900) / 300.0;
			if(white > 1.0f) 
				white = 1.0;
			_colors[i*3+0] = white;
			_colors[i*3+1] = 0.3f + 0.7f*white;
			_colors[i*3+2] = white;
		}
		// .3 to .3333
		else if(scale < .333){
			float orange = 1.2;//(100-data[i]) / 100.0;
			if(orange < 0.0f) orange = 0.0f;
			_colors[i*3+0] = orange * .85;
			_colors[i*3+1] = 0.5f;
			_colors[i*3+2] = 0.0;
		}
		// .3333 to .8333
		else{
			// green
			float dark = (scale-.3333)/.5;
			if(dark > 1.0f) dark = 1.0f;
			_colors[i*3+0] = 0.0f;
			_colors[i*3+1] = 0.5f - 0.2f*dark;
			_colors[i*3+2] = 0.0f;
		}
	}

	_numPoints = LAND_HEIGHT * LAND_WIDTH;
	_numIndices = 2*(LAND_WIDTH-1)*(LAND_HEIGHT-1)*3;

// crop ocean
	for(int i = 0; i < LAND_WIDTH * LAND_HEIGHT; i++){
		if(_points[i*3+2] < -.33 * ZSCALE*.5){
			_points[i*3+2] = -.33 * ZSCALE*.5;///1000.0;    // z, convert meters to km
		}
	}
}