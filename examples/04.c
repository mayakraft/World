// example 4
//
// flying, repeating content

#include "../world.h"

typedef struct{
	int polyNum;
	float x, y, z;
	float scale;
	float zFighting;
}worldObjects;

const int NUM_OBJ = 200;
const int OBJ_DIST = 200;

worldObjects obj[NUM_OBJ];

void drawDots(){
	int dotRows = 32;
	int space = 5;
	float walkX = modulusContext(-origin[0], space);
	float walkY = modulusContext(-origin[1], space);
	int XOffset = ceil(walkX);
	int YOffset = ceil(walkY);
	glTranslatef(walkX, walkY, -origin[2]);
	for(int i = -dotRows*.5; i < dotRows*.5; i++){
		for(int j = -dotRows*.5; j < dotRows*.5; j++){
			drawPoint((i-XOffset)*space, (j-YOffset)*space, 0);
		}
	}
}

void setup(){
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_FUNCTIONS;
	origin[2] = 4;
	glClearColor(0.32, 0.63, 0.75, 1.0);
	for(int i = 0; i < NUM_OBJ; i++){
		do{
			obj[i].polyNum = random()%2 + 1;
			obj[i].x = random()%OBJ_DIST-(OBJ_DIST*0.5);
			obj[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
			obj[i].z = random()%10 + 1;
			obj[i].scale = 1.0 + random()%100/100.0 * 3;
			obj[i].zFighting = random()%100/1000.0;
		}while(fabs(obj[i].y) < 4.0);
	}
	simpleGrayLights();
}
void update(){
	origin[0] += 0.2;
	for(int i = 0; i < NUM_OBJ; i++){
		if(obj[i].x < origin[0]-OBJ_DIST*0.5){
			do{
				obj[i].x = origin[0] + OBJ_DIST*0.25 + random()%OBJ_DIST; 
				obj[i].y = random()%OBJ_DIST-(OBJ_DIST*0.5);
				obj[i].z = random()%10 + 1;
			}while(fabs(obj[i].y) < 4.0);
		};
	}
}
void draw3D(){
	// grassy ground
	glPushMatrix();
		glColor4f(0.16, 0.41, 0.29, 1.0);
		drawRect(-1000, -1000, -origin[2], 2000, 2000);
	glPopMatrix();
	// moving ground dots
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		drawDots();
	glPopMatrix();
	// draw the floating world objects
	for(int i = 0; i < NUM_OBJ; i++){
		glPushMatrix();
			// objects
			glEnable(GL_LIGHTING);
			glTranslatef(-origin[0], -origin[1], -origin[2]);
			glTranslatef(obj[i].x, obj[i].y, obj[i].z);
			glPushMatrix();
				glScalef(obj[i].scale, obj[i].scale, obj[i].scale);
				drawPlatonicSolidFaces( obj[i].polyNum );
			glPopMatrix();
			// object shadows
			glDisable(GL_LIGHTING);
			glPushMatrix();
				glColor4f(0.0, 0.0, 0.0, 0.2);
				glRotatef(45 * obj[i].polyNum,0,0,1);
				drawRect(-obj[i].scale*0.5, -obj[i].scale*0.5, -obj[i].z + 0.01 + obj[i].zFighting, obj[i].scale, obj[i].scale);
			glPopMatrix();
		glPopMatrix();
	}
	// my shadow
	glPushMatrix();
		glColor4f(0.0, 0.0, 0.0, 0.2);
		glRotatef(45,0,0,1);
		drawRect(-1, -1, -origin[2] + 0.1, 2, 2);
	glPopMatrix();
	// me, a octahedron
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(0, 0, 1);
		drawOctahedron();
	glPopMatrix();
	glDisable(GL_LIGHTING);
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }