// example 7
//
// OpenGL lighting, 1000 particles

#include "../world.h"

// polyhedra into particles
typedef struct{
	unsigned int type;  // 0-4 which platonic solid
	float pos[3];
	float vel[3];
	float spin[3];
	float scale;
} Particle;

// particles
const unsigned int numPolyhedra = 1000;
Particle poly[numPolyhedra];
// +/- X,Y,Z boundary for the particles
static float BOUNDS[3] = {5.0, 5.0, 20.0};
// move the center of the bounding box up in the z
static float B_OFFSET[3] = {0.0, 0.0, 19.0};  

void setupLighting(){
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 0.0f};
	GLfloat red[] =   {1.0f, 0.2f, 0.0f, 0.0f};
	GLfloat green[] = {0.3f, 0.9f, 0.3f, 0.0f};
	GLfloat blue[] =  {0.0f, 0.2f, 1.0f, 0.0f};
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	GLfloat light_position0[] = { 0.0, 0.0, 10.0, 1.0 };
	GLfloat light_position1[] = { 8.0, 0.0, -5.0, 0.0 };
	GLfloat light_position2[] = { 5.0, 5.0, 5.0, 0.0 };
	GLfloat light_position3[] = { -5.0, 5.0, -5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 40.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT2, GL_SPECULAR, green);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT3, GL_SPECULAR, blue);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

	glEnable(GL_LIGHTING);
}

void setup() {
	firstPersonPerspective();
	setupLighting();
	// glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_MOVE | SET_KEYBOARD_FUNCTIONS | SET_SHOW_GROUND;

	for (int i = 0; i < numPolyhedra; i++){
		poly[i].type = random() % 5;
		for(int j = 0; j < 3; j++){
			poly[i].pos[j] = ((random()%1000)*0.001-0.5)*2.0 * (BOUNDS[j]) + B_OFFSET[j];
			poly[i].vel[j] = ((random()%1000)*0.001-0.5)*2.0 * 0.01;
			poly[i].spin[j] = ((random()%1000)*0.001-0.5)*2.0 * 200.0;
		}
		poly[i].scale = 0.01 + 0.05 * (random()%100)*0.01;
		poly[i].vel[2] = -0.03;  // z velocity (falling) is fixed
	}
}
void update() { 
	// update particle positions
	for (int i = 0; i < numPolyhedra; i++){
		for(int j = 0; j < 3; j++){
			poly[i].pos[j] += poly[i].vel[j];
			if(poly[i].pos[j] < origin[j]-BOUNDS[j]+B_OFFSET[j]){ poly[i].pos[j] = origin[j]+BOUNDS[j]+B_OFFSET[j]; }
			if(poly[i].pos[j] > origin[j]+BOUNDS[j]+B_OFFSET[j]){ poly[i].pos[j] = origin[j]-BOUNDS[j]+B_OFFSET[j]; }
		}
	}
}
void draw3D() {
	// color lights only
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	// material
	GLfloat mat_white[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
	// draw particles
	for (int i = 0; i < numPolyhedra; i++){
		glPushMatrix();
			glTranslatef(poly[i].pos[0], poly[i].pos[1], poly[i].pos[2]);
			glRotatef(poly[i].spin[0]*elapsed, 1, 0, 0);
			glRotatef(poly[i].spin[1]*elapsed, 0, 1, 0);
			glRotatef(poly[i].spin[2]*elapsed, 0, 0, 1);
			glPushMatrix();
				glScalef(poly[i].scale, poly[i].scale, poly[i].scale);
				drawPlatonicSolidFaces(poly[i].type);
			glPopMatrix();
		glPopMatrix();
	}
	// reset: white light only
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
}
void draw2D() { }
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }