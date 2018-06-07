// example 4
//
// flying, repeating content

#include "../world.h"

typedef struct{
	int polyNum;
	float x, y, z;  // fraction of OBJ_DIST
	float scale;  //  1 to 4
	int randInt;  // 0 maxInt
	float zFighting;  // 0 and .1
}worldObjects;

const int OBJ_DIST = 200;
const int NUM_OBJ = 200;
const int NUM_BLDG = 200;

worldObjects obj[NUM_OBJ];
worldObjects building[NUM_BLDG];

GLfloat green[] = {0.16, 0.41, 0.29, 1.0};
GLfloat darkGreen[] = { 0.10, 0.26, 0.18, 1.00 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat bright_white[] = { 2.0, 2.0, 2.0, 1.0 };

void drawDotsPlane(int dotRows){
	int space = 5;
	float walkX = modulusContext(-ORIGIN[0], space);
	float walkY = modulusContext(-ORIGIN[1], space);
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	glTranslatef(walkX, walkY, -ORIGIN[2]);
	for(int i = -dotRows*.5; i < dotRows*.5; i++){
		for(int j = -dotRows*.5; j < dotRows*.5; j++){
			drawPoint((i-XOffset)*space, (j-YOffset)*space, 0);
		}
	}
}

void setup(){
	HANDED = RIGHT;
	// glShadeModel(GL_FLAT); //GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_FUNCTIONS;
	ORIGIN[2] = 4;
	// glClearColor(0.32, 0.63, 0.75, 1.0);
	glClearColor(0.19, 0.45, 0.68, 1.00);
	for(int i = 0; i < NUM_OBJ; i++){
		do{
			obj[i].polyNum = random()%2 + 1;
			obj[i].x = random()%OBJ_DIST-(OBJ_DIST*0.5);
			obj[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
			obj[i].z = random()%10 + 1;
			obj[i].scale = 1.0 + random()%100/100.0 * 3;
			obj[i].zFighting = random()%100/1000.0;
			obj[i].randInt = random();
		}while(fabs(obj[i].y) < 4.0);
	}
	for(int i = 0; i < NUM_BLDG; i++){
		do{
			building[i].polyNum = 2;
			building[i].x = random()%OBJ_DIST-(OBJ_DIST*0.5);
			building[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
			building[i].scale = 10.0 + random()%100/100.0 * 10;
			building[i].zFighting = 1.0 + random()%100/100.0 * 6;
		}while(fabs(building[i].y) < 8.0);
	}

	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	GLfloat spot_position[] = { 0.0, 0.0, 50.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, spot_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glEnable(GL_LIGHT0);

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient_position[] = { 0.0, 0.0, 100.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_POSITION, ambient_position);
}
void update(){
	ORIGIN[0] += 0.2;
	for(int i = 0; i < NUM_OBJ; i++){
		if(obj[i].x < ORIGIN[0]-OBJ_DIST*0.5){
			do{
				obj[i].x = ORIGIN[0] + OBJ_DIST*0.25 + random()%OBJ_DIST; 
				obj[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
				obj[i].z = random()%10 + 1;
			}while(fabs(obj[i].y) < 4.0);
		};
	}
	for(int i = 0; i < NUM_BLDG; i++){
		if(building[i].x < ORIGIN[0]-OBJ_DIST*0.5){
			do{
				building[i].x = ORIGIN[0] + OBJ_DIST*0.25 + random()%OBJ_DIST; 
				building[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
			}while(fabs(building[i].y) < 8.0);
		};
	}
}
void draw3D(){
	// grassy ground
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_AMBIENT, darkGreen);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	glPushMatrix();
		for(int i = 0; i < 100; i++){
			for(int j = 0; j < 100; j++){
				drawRect(-1000 + 20*i, -1000 + 20*j, -ORIGIN[2], 20, 20);
			}
		}
	glPopMatrix();

	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	// moving ground dots
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		drawDotsPlane(32);
	glPopMatrix();

	// draw the floating world objects
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bright_white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);

	glEnable(GL_LIGHTING);
	for(int i = 0; i < NUM_BLDG; i++){
		glPushMatrix();
			// buildings
			glTranslatef(-ORIGIN[0], -ORIGIN[1], -ORIGIN[2]);
			glTranslatef(building[i].x, building[i].y, building[i].scale*0.5);
			glPushMatrix();
				glScalef(7.0, building[i].zFighting, building[i].scale);
				drawPlatonicSolidFaces( building[i].polyNum );
			glPopMatrix();
		glPopMatrix();
	}
	

	for(int i = 0; i < NUM_OBJ; i++){
		glPushMatrix();
			// objects

		glEnable(GL_LIGHTING);

			glTranslatef(-ORIGIN[0], -ORIGIN[1], -ORIGIN[2]);
			glTranslatef(obj[i].x, obj[i].y, obj[i].z);
			glPushMatrix();
				// make the cubes move up and down
				if(obj[i].polyNum == 2){
					glTranslatef(0, 0, obj[i].z);
					float zPos = 0;
					int intSec = ELAPSED;
					if((intSec+obj[i].randInt)%7 == 0) zPos = (-cosf((ELAPSED-intSec)*M_PI)+1)*0.5;
					if((intSec+obj[i].randInt)%7 == 1) zPos = 1.0;
					if((intSec+obj[i].randInt)%7 == 2) zPos = 1.0 - ((-cosf((ELAPSED-intSec)*M_PI)+1)*0.5);
					glTranslatef(0, 0, -zPos * obj[i].z * 2);
					glShadeModel(GL_SMOOTH);
				} else{
					glShadeModel(GL_FLAT); //GL_SMOOTH);
				}
				glScalef(obj[i].scale, obj[i].scale, obj[i].scale);
				drawPlatonicSolidFaces( obj[i].polyNum );
			glPopMatrix();
			glShadeModel(GL_SMOOTH);
			// object shadows

			glDisable(GL_LIGHTING);
			glPushMatrix();
				glColor4f(0.0, 0.0, 0.0, 0.2);
				glRotatef(45 * obj[i].polyNum,0,0,1);
				float sc = obj[i].scale*1.414;
				drawRect(-sc*0.5, -sc*0.5, -obj[i].z + 0.01 + obj[i].zFighting, sc, sc);
			glPopMatrix();
		glPopMatrix();
	}
	// my shadow
	glPushMatrix();
		glColor4f(0.0, 0.0, 0.0, 0.2);
		glScalef(1.0, 3.0, 0.5);
		glRotatef(45,0,0,1);
		drawRect(-1, -1, -ORIGIN[2]*2 + 0.1, 2, 2);
	glPopMatrix();
	// me, a octahedron
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glScalef(1.0, 3.0, 0.5);
		glTranslatef(0, 0, 1);
		drawOctahedron(1);
	glPopMatrix();
	glDisable(GL_LIGHTING);
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }