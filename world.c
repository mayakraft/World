#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static int windowWidth = 800;
static int windowHeight = 400;


// INPUT HANDLING
static unsigned int UP_PRESSED = 0;
static unsigned int DOWN_PRESSED = 0;
static unsigned int RIGHT_PRESSED = 0;
static unsigned int LEFT_PRESSED = 0;
static int mouseDownX = 0;
static int mouseDownY = 0;


// FIRST PERSON PERSPECTIVE USING KEYBOARD (WASD), MOUSE (LOOK)
#define STEP .10f
static float cameraX = 0.0f;
static float cameraY = 0.0f;
static float mouseRotationX = 180.0f;
static float mouseRotationY = 0.0f;
static float startRotationX = 0.0f;  // POV heading on mouse down
static float startRotationY = 0.0f;  // POV heading on mouse down

void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
}

void unitSquare(float x, float y, float width, float height){
	static const GLfloat _unit_square[] = {
	    -0.5f, 0.5f, 0.0f,     0.5f, 0.5f, 0.0f,     -0.5f, -0.5f, 0.0f,     0.5f, -0.5f, 0.0f  };
	static const GLfloat _unit_square_normals[] = {
	    0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f  };
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(width, height, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _unit_square);
	glNormalPointer(GL_FLOAT, 0, _unit_square_normals);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
		glRotatef(mouseRotationY, -1, 0, 0);
		glRotatef(mouseRotationX, 0, 0, -1);
		glTranslatef(cameraX, cameraY, 0);
		// first person perspective has been established
		// draw below

		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -1.5f);
			int XOffset = cameraX;
			int ZOffset = cameraY;
			for(int i = -8; i <= 8; i++){
				for(int j = -8; j <= 8; j++){
					float b = abs(((i+j+XOffset+ZOffset)%2));
					if(b) glColor3f(0.2, 0.2, 0.2);
					else glColor3f(0.4, 0.4, 0.4);
					unitSquare(i-XOffset, j-ZOffset, 1, 1);
				}
			}
		glPopMatrix();

		// glRotatef(180, -1, 0, 0);  // ORTHO 1
		// glRotatef(120+cos(spin*.0015)*30, -1, 0, 0);  // PERSPECTIVE 1

		// glRotatef(sin(spin*.004)*90, 0.0f, 0.0f, 1.0f);  // PERSPECTIVE 2
		glPushMatrix();
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			// glColor3f(0.5f, 1.0f, 0.5f);
			// glColorPointer(3, GL_FLOAT, 0, _colors);
			// glVertexPointer(3, GL_FLOAT, 0, _points);
			// glDrawArrays(GL_POINTS, 0, _numPoints);
			// glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, _indices);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		glPopMatrix();

		// glPushMatrix();
		// glTranslatef(0.0f, 0.0f, 0.0f);
		// glLineWidth(2);
		// glColor3f(1.0f, 1.0f, 1.0f);
		// glEnableClientState(GL_VERTEX_ARRAY);
		// glVertexPointer(2, GL_FLOAT, 0, political);
		// glDrawArrays(GL_LINE_LOOP, 0, 128);
		// glDisableClientState(GL_VERTEX_ARRAY);
		// glPopMatrix();


	glPopMatrix();

	glutSwapBuffers();
	// glFlush();
}

// void spinDisplay(void){
// 	spin = spin - 2.0;
// 	if(spin > 360.0){
// 		spin = spin - 360.0;
// 	}
// 	glutPostRedisplay();
// }

void update(){

	float lookAzimuth = 0;
	float mouseAzimuth = mouseRotationX/180.*M_PI;    
	lookAzimuth += mouseAzimuth;
	
	if(UP_PRESSED){
		float x = STEP * sinf(lookAzimuth);
		float y = STEP * -cosf(lookAzimuth);
		cameraX += x;
		cameraY += y;
	}
	if(DOWN_PRESSED){
		float x = STEP * sinf(lookAzimuth);
		float y = STEP * -cosf(lookAzimuth);
		cameraX -= x;
		cameraY -= y;
	}
	if(LEFT_PRESSED){
		float x = STEP * sinf(lookAzimuth+M_PI_2);
		float y = STEP * -cosf(lookAzimuth+M_PI_2);
		cameraX -= x;
		cameraY -= y;
	}
	if(RIGHT_PRESSED){
		float x = STEP * sinf(lookAzimuth+M_PI_2);
		float y = STEP * -cosf(lookAzimuth+M_PI_2);
		cameraX += x;
		cameraY += y;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			// if(state == GLUT_DOWN) { }
				// glutIdleFunc(spinDisplay);
			mouseDownX = x;
			mouseDownY = y;
			startRotationX = mouseRotationX;
			startRotationY = mouseRotationY;
		break;
		case GLUT_MIDDLE_BUTTON:
		break;
		case GLUT_RIGHT_BUTTON:
		break;
		default:
			break;
	}
}

void mouseMotion(int x, int y)
{
	mouseRotationX = startRotationX + mouseDownX - x;
	mouseRotationY = startRotationY + mouseDownY - y;
	glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y){
	switch (key){
		case 27:             // ESCAPE key
			exit (0);
			break;
		case 119:
		case GLUT_KEY_UP:
			UP_PRESSED = 1;
			break;
		case 115:
		case GLUT_KEY_DOWN:
			DOWN_PRESSED = 1;
			break;
		case 97:
		case GLUT_KEY_RIGHT:
			RIGHT_PRESSED = 1;
			break;
		// case 100:
		case GLUT_KEY_LEFT:
			LEFT_PRESSED = 1;
			break;
		// case 't':
		// 	break;
	}
	if(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED)
		glutIdleFunc(update);
}

void keyboardUp(unsigned char key,int x,int y){
	switch (key){
		case 27:             // ESCAPE key
			exit (0);
			break;
		case 119:
		case GLUT_KEY_UP:
			UP_PRESSED = 0;
			break;
		case 115:
		case GLUT_KEY_DOWN:
			DOWN_PRESSED = 0;
			break;
		case 97:
		case GLUT_KEY_RIGHT:
			RIGHT_PRESSED = 0;
			break;
		// case 100:
		case GLUT_KEY_LEFT:
			LEFT_PRESSED = 0;
			break;
		// case 't':
		// 	break;
	}
	if(!(UP_PRESSED || DOWN_PRESSED || RIGHT_PRESSED || LEFT_PRESSED))
		glutIdleFunc(NULL);
}

void reshape(int w, int h){
	float a = (float)windowWidth/windowHeight;
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-40.0, 40.0, -40/a, 40/a, -100.0, 100.0);
	glFrustum (-1.0, 1.0, -1.0/a, 1.0/a, 1.5, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardUpFunc(keyboardUp); 
	glutKeyboardFunc(keyboardDown);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}