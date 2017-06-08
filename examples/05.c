#include "../world.h"

typedef struct{
	unsigned int type;
	float position[3];
	float attr1[3];
	float attr2[3];
	float a;
} Poly;

const unsigned int numPolyhedra = 15;
Poly poly[numPolyhedra];

void setupLighting(){
	GLfloat light_position0[] = { 0.0, 0.0, 10.0, 1.0 };
	GLfloat light_position1[] = { 3.0, 3.0, -3.0, 0.0 };
	GLfloat light_position2[] = { -3.0, 3.0, 1.0, 0.0 };
	GLfloat light_position3[] = { -1.0, -3.0, -1.0, 0.0 };
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 0.0f};
	GLfloat red[] = {1.0f, 0.2f, 0.0f, 0.0f};
	GLfloat green[] = {0.3f, 0.9f, 0.3f, 0.0f};
	GLfloat blue[] = {0.0f, 0.2f, 1.0f, 0.0f};
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 40.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT2, GL_SPECULAR, green);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT3, GL_SPECULAR, blue);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

	glEnable(GL_LIGHTING);
}
void setup() {
	// glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	setupLighting();
	firstPersonPerspective();
	GRID = 0;
	GROUND = 0;
	for (int i = 0; i < numPolyhedra; i++){
		poly[i].type = random() % 5;
		poly[i].position[0] += (random() % 100 - 50) * 0.05;
		poly[i].position[1] += (random() % 100 - 50) * 0.05;
		poly[i].position[2] += (random() % 100 - 50) * 0.05;
		poly[i].a = random() % 100 * 0.1;
		for(int j = 0; j < 3; j++){
			poly[i].attr1[j] = (random() % 100 - 50) * 0.005;
			poly[i].attr2[j] = (random() % 100) * 0.01;  // 0 and 1
		}
		// printf("%d: %d, %f -- %f, %f, %f\n", i, poly[i].type, poly[i].a, poly[i].position[0], poly[i].attr1[0], poly[i].attr2[0]);
	}
}
void update() { 
	// for (int i = 0; i < numPolyhedra; i++){
	// 	poly[i].position[0] += attr1[0];
	// 	poly[i].position[1] += attr1[1];
	// 	poly[i].position[2] += attr1[2];
	// }
}
void draw3D() {
	glTranslatef(-origin[0], -origin[1], -origin[2]);
	glEnable(GL_LIGHTING);
	GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glColor3f(1.0, 1.0, 1.0); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
	for (int i = 0; i < numPolyhedra; i++){
		glPushMatrix();
			glTranslatef(poly[i].position[0] + 2*sinf(elapsed*poly[i].attr1[0] +
			                                          poly[i].attr2[0]*2*M_PI), 
			             poly[i].position[1] + 2*sinf(elapsed*poly[i].attr1[1] + 
			                                          poly[i].attr2[1]*2*M_PI), 
			             poly[i].position[2] + 2*sinf(elapsed*poly[i].attr1[2] + 
			                                          poly[i].attr2[2]*2*M_PI) );
			drawPlatonicSolidFaces(poly[i].type);
		glPopMatrix();
	}
}
void draw2D() { }
void keyDown(unsigned int key) { }
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }