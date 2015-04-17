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


#define STEP 1.0f
#define M_PI_HALF 1.5708f


static int windowWidth = 800;
static int windowHeight = 400;

static float POVX = 0.0f;
static float POVY = 0.0f;
static unsigned int UP_PRESSED = 0;
static unsigned int DOWN_PRESSED = 0;
static unsigned int RIGHT_PRESSED = 0;
static unsigned int LEFT_PRESSED = 0;
static int mouseDownX = 0;
static int mouseDownY = 0;
static float mouseRotationX = 180.0f;
static float mouseRotationY = 0.0f;
static float startRotationX = 0.0f;
static float startRotationY = 0.0f;


static GLfloat spin = 0.0f;
static float *_points;
static uint32_t *_indices;
static float *_colors;
static unsigned int _numPoints;
static unsigned int _numIndices;

void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
		// glTranslatef(0, 0, -30);
		// glTranslatef(0, 0, -50);
		// glScalef(.1, .1, -.1);
   		glRotatef(mouseRotationY, -1, 0, 0);
	    glRotatef(mouseRotationX, 0, -1, 0);

		glTranslatef(POVY, 0, -POVX);
		glPushMatrix();

		// glRotatef(-90, 1, 0, 0);
		glTranslatef(0,0,-30);

	glPushMatrix();
	glRotatef(spin, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0, 1.0, 1.0);
		glRectf(-25.0, -25.0, 25.0, 25.0);
	glPopMatrix();

		// glRotatef(180, -1, 0, 0);  // ORTHO 1
		// glRotatef(120+cos(spin*.0015)*30, -1, 0, 0);  // PERSPECTIVE 1

		// glRotatef(sin(spin*.004)*90, 0.0f, 0.0f, 1.0f);  // PERSPECTIVE 2
		glPushMatrix();
		glScalef(-1.0f, 1.0f, .10f);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(0.5f, 1.0f, 0.5f);
		glColorPointer(3, GL_FLOAT, 0, _colors);
		glVertexPointer(3, GL_FLOAT, 0, _points);
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
	glPopMatrix();
	glutSwapBuffers();
	// glFlush();
}

void spinDisplay(void){
	spin = spin - 2.0;
	if(spin > 360.0){
		spin = spin - 360.0;
	}
	glutPostRedisplay();
}

void update(){

    float lookAzimuth = 0;
    float mouseAzimuth = mouseRotationX/180.*3.14159f-M_PI_HALF;    
    lookAzimuth += mouseAzimuth;
    
    if(UP_PRESSED){
        float x = sinf(lookAzimuth);
        float y = cosf(lookAzimuth);
        POVX += x;
        POVY += y;
    }
    if(DOWN_PRESSED){
        float x = sinf(lookAzimuth);
        float y = cosf(lookAzimuth);
        POVX -= x;
        POVY -= y;
    }
    if(LEFT_PRESSED){
        float x = sinf(lookAzimuth+M_PI_HALF);
        float y = cosf(lookAzimuth+M_PI_HALF);
        POVX -= x;
        POVY -= y;
    }
    if(RIGHT_PRESSED){
        float x = sinf(lookAzimuth+M_PI_HALF);
        float y = cosf(lookAzimuth+M_PI_HALF);
        POVX += x;
        POVY += y;
    }
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
				// glutIdleFunc(spinDisplay);
				mouseDownX = x;
				mouseDownY = y;
				startRotationX = mouseRotationX;
				startRotationY = mouseRotationY;
			break;
		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN)
				// glutIdleFunc(NULL);
				mouseDownX = x;
				mouseDownY = y;
				startRotationX = mouseRotationX;
				startRotationY = mouseRotationY;
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
	// printf("%f, %f\n", mouseRotationX, mouseRotationY);
  // If button1 pressed, zoom in/out if mouse is moved up/down.
  // if (g_bButton1Down)
  //   {
  //     g_fViewDistance = (y - g_yClick) / 3.0;
  //     if (g_fViewDistance < VIEWING_DISTANCE_MIN)
  //        g_fViewDistance = VIEWING_DISTANCE_MIN;
  //     glutPostRedisplay();
  //   }
}
void keyboard(unsigned char key, int x, int y){
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
	glutKeyboardFunc(keyboard);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}