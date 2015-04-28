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

// CHOOSE YOUR OWN PERSPECTIVE
//
// 0: first person perspective, X Y movement
// 1: polar, focus on origin, Y radius
// 2: orthographic from above, X Y panning
static unsigned char PERSPECTIVE = 0;
static unsigned char FULLSCREEN = 0;
// size of window in OS
static int WIDTH = 800;
static int HEIGHT = 400;
// INPUT HANDLING
static unsigned int UP_PRESSED = 0;    // KEY UP:0   KEY DOWN:1
static unsigned int DOWN_PRESSED = 0;
static unsigned int RIGHT_PRESSED = 0;
static unsigned int LEFT_PRESSED = 0;
static int mouseDragOffsetX = 0;  // how far mouse is dragged during one session (between click and release)
static int mouseDragOffsetY = 0;
static int mouseTotalOffsetX = 0;  // how far mouse has been dragged since program began
static int mouseTotalOffsetY = 0;
static int mouseDragStartX, mouseDragStartY, mouseTotalOffsetStartX, mouseTotalOffsetStartY;


#define STEP .10f  // WALKING SPEED. @60fps, walk speed = 6 units/second
#define MOUSE_SENSITIVITY 0.333f

// PERSPECTIVES
// FIRST PERSON PERSPECTIVE USING KEYBOARD (WASD), MOUSE (LOOK)
static float walkX = 0.0f;
static float walkY = 0.0f;
// POLAR PERSPECTIVE
static float cameraRadius = 15.0f;
// ORTHOGRAPHIC
static float panX = 0.0f;
static float panY = 0.0f;

static unsigned char landscape = 0;  // checkerboard or axes lines


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
	if(PERSPECTIVE == 0 || PERSPECTIVE == 1)
		glFrustum (-.1, .1, -.1/a, .1/a, .1, 100.0);
	else if (PERSPECTIVE == 2)
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
	glTranslatef(x, y, 0.0);
	glScalef(width, height, 1.0);
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
		1.0f, 0.0f, 0.0f,    
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,  0.0f,    
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,     
		0.0f, 0.0f, -1.0f};
	static const GLfloat _unit_axis_normals[] = { 
		0.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 1.0f, 
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f};
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

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
		// SETUP PERSPECTIVE
		if(PERSPECTIVE == 0){
			glRotatef(mouseTotalOffsetY * MOUSE_SENSITIVITY, -1, 0, 0);
			glRotatef(mouseTotalOffsetX * MOUSE_SENSITIVITY, 0, 0, -1);
			glTranslatef(walkX, walkY, 0);
		}
		if(PERSPECTIVE == 1){
			glTranslatef(0, 0, -cameraRadius);
			glRotatef(mouseTotalOffsetY * MOUSE_SENSITIVITY, -1, 0, 0);
			glRotatef(mouseTotalOffsetX * MOUSE_SENSITIVITY, 0, 0, -1);
		}
		if(PERSPECTIVE == 2){
			glTranslatef(-mouseTotalOffsetX * .05 - panX*2, mouseTotalOffsetY * .05 - panY*2, 0.0f);
		}

		// perspective has been established
		// draw stuff below

		glPushMatrix();
			// raise POV 1.0 above the floor
			glTranslatef(0.0f, 0.0f, -1.0f);

			// CHECKERBOARD LANDSCAPE
			if(!landscape){
				static int nSqrs = 8;
				int XOffset = 0;
				int YOffset = 0;
				if(PERSPECTIVE == 0){
					XOffset = ceil(walkX);
					YOffset = ceil(walkY);
				}
				for(int i = -nSqrs; i <= nSqrs; i++){
					for(int j = -nSqrs; j <= nSqrs; j++){
						int b = abs(((i+j+XOffset+YOffset)%2));
						if(b) glColor3f(1.0, 1.0, 1.0);
						else glColor3f(0.0, 0.0, 0.0);
						unitSquare(i-XOffset, j-YOffset, 1, 1);
					}
				}
			}
			// 3 DIMENSIONS OF SCATTERED AXES
			else{
				static int span = 5;
				static int numRepeat = 4;  //20 // how many rows/cols/stacks on either side of center
				float XSpanMod = walkX - floor(walkX/span)*span;
				float YSpanMod = walkY - floor(walkY/span)*span;
				glColor3f(1.0, 1.0, 1.0);
				for(int i = -numRepeat*span; i < numRepeat*span; i+=span){
					for(int j = -numRepeat*span; j < numRepeat*span; j+=span){
						for(int k = -numRepeat*span; k < numRepeat*span; k+=span){
							float distance = sqrtf(powf(i+XSpanMod,2) + powf(j+YSpanMod,2) + powf(k,2));
							float brightness = 1.0 - distance/(numRepeat*span);
							glColor3f(brightness, brightness, brightness);
							// glLineWidth(100.0/distance/distance);
							if(PERSPECTIVE == 0)
								unitAxis(i + XSpanMod - walkX, j + YSpanMod - walkY, k, 1.0f);
							else
								unitAxis(i, j, k, 1.0f);
						}
					}
				}
			}
		glPopMatrix();

	glPopMatrix();
	// bring back buffer to the front on vertical refresh, auto-calls glFlush
	glutSwapBuffers();
	// glFlush();
}

// process input devices if in orthographic mode
void updateOrthographic(){
	if(UP_PRESSED)
		panY -= STEP;
	if(DOWN_PRESSED)
		panY += STEP;
	if(LEFT_PRESSED)
		panX -= STEP;
	if(RIGHT_PRESSED)
		panX += STEP;
	glutPostRedisplay();
}

// process input devices if in polar perspective mode
void updatePolar(){
	if(UP_PRESSED)
		cameraRadius -= STEP;
	if(DOWN_PRESSED)
		cameraRadius += STEP;
	if(cameraRadius < 0) 
		cameraRadius = 0;
	glutPostRedisplay();
}

// process input devices if in first person perspective mode
void updateFirstPerson(){
	// map movement direction to the direction the person is facing
	float lookAzimuth = 0;
	float mouseAzimuth = (mouseTotalOffsetX * MOUSE_SENSITIVITY)/180.*M_PI;    
	lookAzimuth += mouseAzimuth;
	if(UP_PRESSED){
		float x = STEP * sinf(lookAzimuth);
		float y = STEP * -cosf(lookAzimuth);
		walkX += x;
		walkY += y;
	}
	if(DOWN_PRESSED){
		float x = STEP * sinf(lookAzimuth);
		float y = STEP * -cosf(lookAzimuth);
		walkX -= x;
		walkY -= y;
	}
	if(LEFT_PRESSED){
		float x = STEP * sinf(lookAzimuth+M_PI_2);
		float y = STEP * -cosf(lookAzimuth+M_PI_2);
		walkX -= x;
		walkY -= y;
	}
	if(RIGHT_PRESSED){
		float x = STEP * sinf(lookAzimuth+M_PI_2);
		float y = STEP * -cosf(lookAzimuth+M_PI_2);
		walkX += x;
		walkY += y;
	}
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
	else if(key == ' '){  // SPACE BAR
		landscape = !landscape;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == 'f'){
		if(!FULLSCREEN)
			glutFullScreen();
		else{
			reshape(WIDTH * .5, HEIGHT * .5);
        	glutPositionWindow(0,0);
		}
		FULLSCREEN = !FULLSCREEN;
	}
	else if(key == '1'){
		PERSPECTIVE = 0;
		// mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == '2'){
		PERSPECTIVE = 1;
		// mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();
	}
	else if(key == '3'){
		PERSPECTIVE = 2;
		mouseTotalOffsetX = mouseTotalOffsetY = 0;
		reshape(WIDTH, HEIGHT);
		glutPostRedisplay();	
	}
	
	// anything that affects the screen and requires a redisplay
	// put it in this if statement
	if(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED){
		if(PERSPECTIVE == 0)
			glutIdleFunc(updateFirstPerson);
		if(PERSPECTIVE == 1)
			glutIdleFunc(updatePolar);
		if(PERSPECTIVE == 2)
			glutIdleFunc(updateOrthographic);
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
	
	if(!(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED))
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