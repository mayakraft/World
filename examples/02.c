// example 2
//
// Perlin noise landscape

#include "../world.h"
#include "noise.c"

static int LAND_WIDTH = 400;
static int LAND_HEIGHT = 400;
#define ZSCALE 10.0

static float *_points;
static float *_normals;
static uint32_t *_indices;
static float *_colors;

static unsigned int _numPoints;
static unsigned int _numIndices;

void buildWorld();

void drawLandscape(){
	glPushMatrix();
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, _colors);
		glVertexPointer(3, GL_FLOAT, 0, _points);
		glNormalPointer(GL_FLOAT, 0, _normals);
		// glDrawArrays(GL_POINTS, 0, _numPoints);
		glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, _indices);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	glPopMatrix();
}

void setup(){ 
	_points = (float*)malloc(sizeof(float) * LAND_WIDTH*LAND_HEIGHT * 3);
	_normals = (float*)malloc(sizeof(float) * LAND_WIDTH*LAND_HEIGHT * 3);
	_indices = (uint32_t*)malloc(sizeof(uint32_t) * 2*(LAND_WIDTH-1)*(LAND_HEIGHT-1) * 3);
	_colors = (float*)malloc(sizeof(float) * LAND_WIDTH*LAND_HEIGHT * 3);
	buildWorld();
	SETTINGS = SET_MOUSE_LOOK | SET_KEYBOARD_MOVE | SET_KEYBOARD_FUNCTIONS;

	GLfloat white_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	GLfloat spot_position[] = { 0.0, 0.0, 5.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_color);
	glLightfv(GL_LIGHT0, GL_POSITION, spot_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void update(){ 
	int w = ORIGIN[0] * 10 + LAND_WIDTH*.5 ;
	int h = ORIGIN[1] * 10 +  LAND_HEIGHT*.5 ;
	if(w < 0) w = 0;
	if(h < 0) h = 0;
	if(w >= LAND_WIDTH) w = LAND_WIDTH-1;
	if(h >= LAND_HEIGHT) h = LAND_HEIGHT-1;

	float target = ORIGIN[2];
	if(_numPoints > h*LAND_WIDTH+w){
		if(PERSPECTIVE == FPP){
			target = _points[(h*LAND_WIDTH+w)*3+2];
		}
	}
	ORIGIN[2] = ORIGIN[2]*0.5 + target*0.5;
}
void draw3D(){ 
	drawLandscape();
}
void draw2D(){ }
void keyDown(unsigned int key){ 
	if(key == ' ') buildWorld();
}
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }

void buildWorld(){
	#define START_FREQ .002
	float freq = START_FREQ;

	// reset data
	memset(_points, 0, sizeof(float)*LAND_HEIGHT*LAND_WIDTH*3);
	// set X Y to the grid
	for(int h = 0; h < LAND_HEIGHT; h++){
		for(int w = 0; w < LAND_WIDTH; w++){
			_points[(h*LAND_WIDTH+w)*3+0] = (w - LAND_WIDTH*.5) * .1;    // x
			_points[(h*LAND_WIDTH+w)*3+1] = (h - LAND_HEIGHT*.5) * .1;   // y
		}
	}

	for(int loop = 0; loop < 20; loop++){
		freq *= 2;
		float RX = rand()%1000/500.0 - 1.0;
		float RY = rand()%1000/500.0 - 1.0;
		for(int h = 0; h < LAND_HEIGHT; h++){
			for(int w = 0; w < LAND_WIDTH; w++){
				float vec[2] = {w*freq + RX, h*freq + RY};
				_points[(h*LAND_WIDTH+w)*3+2] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ);
			}
		}
	}

// these next 2 blocks distort along X and Y,
// potential to make the land non-continuous, may create bad geometry
	//////////////////////////////////////////////////////
	freq = START_FREQ*.5;
	for(int loop = 0; loop < 4; loop++){
		freq *= 2;
		float RX = rand()%1000/500.0 - 1.0;
		float RY = rand()%1000/500.0 - 1.0;
		for(int h = 0; h < LAND_HEIGHT; h++){
			for(int w = 0; w < LAND_WIDTH; w++){
				float vec[2] = {w*freq + RX, h*freq + RY};
				_points[(h*LAND_WIDTH+w)*3+0] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ*12);
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
				_points[(h*LAND_WIDTH+w)*3+1] += noise2(vec) * 1.5*ZSCALE/(freq/START_FREQ*12);
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

	// GENERATE COLORS
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
			_colors[i*3+0] = orange * .66;
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

	// crop ocean
	for(int i = 0; i < LAND_WIDTH * LAND_HEIGHT; i++){
		if(_points[i*3+2] < -.33 * ZSCALE*.5){
			_points[i*3+2] = -.33 * ZSCALE*.5;
		}
	}

	// generate normals
	for(int h = 0; h < LAND_HEIGHT; h++){
		for(int w = 0; w < LAND_WIDTH; w++){
			int i =        ((h)*LAND_WIDTH+(w))*3;
			int nextOver = ((h)*LAND_WIDTH+(w+1))*3;
			int downOne =  ((h+1)*LAND_WIDTH+(w))*3;
			if(w == LAND_WIDTH-1)  nextOver = ((h)*LAND_WIDTH+(w-1))*3;
			if(h == LAND_HEIGHT-1) downOne =  ((h-1)*LAND_WIDTH+(w))*3;
			float a[3] = {_points[nextOver+0]-_points[i+0], _points[nextOver+1]-_points[i+1], _points[nextOver+2]-_points[i+2]};
			float b[3] = {_points[downOne+0]-_points[i+0], _points[downOne+1]-_points[i+1], _points[downOne+2]-_points[i+2]};
			vec3Cross(a, b, &_normals[i]);
			vec3Normalize(&_normals[i]);
		}
	}

	_numPoints = LAND_HEIGHT * LAND_WIDTH;
	_numIndices = 2*(LAND_WIDTH-1)*(LAND_HEIGHT-1)*3;
}
