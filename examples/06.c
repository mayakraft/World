// example 6
//
// rotation matrices
// geocentric astronomy model

#include "../world.h"
#include "../examples/data/518stars.c"
#include "../examples/data/1619stars.c"

void renderStars(){
	glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		// bright stars
		glVertexPointer(3, GL_FLOAT, 0, _518_stars);
		glColor3f(1.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 518);
		// dim stars
		glVertexPointer(3, GL_FLOAT, 0, _1619_stars);
		glColor3f(0.5, 0.5, 0.5);
		glDrawArrays(GL_POINTS, 0, 1619);
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

// Matrix B, in the book
// void generateSiderealTimeMatrix(float *m, float localSiderealTime){
// // for multiplying against one and getting the other. works either way.
// // (a): altitude and declination
// // (b): hour angle and declination
// 	// localSiderealTime must be in degrees, 0deg to 360deg
// 	// if it's in hours: multiply by 15. example: 18hrs * 15 = 270deg
// 	float angle = localSiderealTime * D2R; // convert to radians
// 	m[0] = cos(angle); m[1] = sin(angle);  m[2] = 0;   m[3] = 0;
// 	m[4] = sin(angle); m[5] = -cos(angle); m[6] = 0;   m[7] = 0; 
// 	m[8] = 0;          m[9] = 0;           m[10] = 1;  m[11] = 0;
// 	m[12] = 0;         m[13] = 0;          m[14] = 0;  m[15] = 1;
// }
void generateSiderealTimeMatrix(float *m, float localSiderealTime){
// for multiplying against one and getting the other. works either way.
// (a): altitude and declination
// (b): hour angle and declination
	// localSiderealTime must be in degrees, 0deg to 360deg
	// if it's in hours: multiply by 15. example: 18hrs * 15 = 270deg
	float angle = localSiderealTime * D2R; // convert to radians
	m[0] = cos(angle); m[1] = sin(angle);  m[2] = 0;   m[3] = 0;
	m[4] = -sin(angle); m[5] = cos(angle); m[6] = 0;   m[7] = 0; 
	m[8] = 0;          m[9] = 0;           m[10] = 1;  m[11] = 0;
	m[12] = 0;         m[13] = 0;          m[14] = 0;  m[15] = 1;
}

// Matrix A, in the book
// void generateGeographicLatitudeMatrix(float *m, float latitude){
// // for multiplying against one and getting the other. works either way.
// // (a): azimuth and altitude
// // (b): hour angle and declination
// 	float angle = latitude * D2R;  // radians
// 	m[0] = -sin(angle); m[1] = 0;   m[2] = cos(angle);  m[3] = 0;
// 	m[4] = 0;           m[5] = -1;  m[8] = 0;           m[9] = 0; 
// 	m[8] = cos(angle);  m[9] = 0;   m[10] = sin(angle); m[11] = 0;
// 	m[12] = 0;          m[13] = 0;  m[14] = 0;          m[15] = 1;
// }
void generateGeographicLatitudeMatrix(float *m, float latitude){
// for multiplying against one and getting the other. works either way.
// (a): azimuth and altitude
// (b): hour angle and declination
	float angle = latitude * D2R;  // radians
	m[0] = cos(angle);  m[1] = 0;   m[2] = sin(angle);  m[3] = 0;
	m[4] = 0;           m[5] = 1;   m[8] = 0;           m[9] = 0; 
	m[8] = -sin(angle); m[9] = 0;   m[10] = cos(angle); m[11] = 0;
	m[12] = 0;          m[13] = 0;  m[14] = 0;          m[15] = 1;
}

int zoom = 1;

float identity[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

float m[16] = {
	0.0, 0.0, 1.0, 0.0,
	0.0, -1.0, 0.0, 0.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};


float siderealMatrix[16];
float latitudeMatrix[16];

float latitude;

// sin and cos of 23.446236
float eclipticToEquatorial[16] = {
	1.0, 0.000000, 0.000000, 0.0,
	0.0, 0.917498,-0.397741, 0.0,
	0.0, 0.397741, 0.917498, 0.0,
	0.0, 0.000000, 0.000000, 1.0
};
float equatorialToEcliptic[16] = {
	1.0, 0.000000, 0.000000, 0.0,
	0.0, 0.917498, 0.397741, 0.0,
	0.0,-0.397741, 0.917498, 0.0,
	0.0, 0.000000, 0.000000, 1.0
};

void drawUnitCubeWireframe(){
	static float l = 0.66;
	drawLine(1.0, 1.0, 1.0, 1.0, 1.0, l);
	drawLine(1.0, 1.0, 1.0, 1.0, l, 1.0);
	drawLine(1.0, 1.0, 1.0, l, 1.0, 1.0);
	drawLine(-1.0, 1.0, 1.0, -1.0, 1.0, l);
	drawLine(-1.0, 1.0, 1.0, -1.0, l, 1.0);
	drawLine(-1.0, 1.0, 1.0, -l, 1.0, 1.0);
	drawLine(1.0, -1.0, 1.0, 1.0, -1.0, l);
	drawLine(1.0, -1.0, 1.0, 1.0, -l, 1.0);
	drawLine(1.0, -1.0, 1.0, l, -1.0, 1.0);
	drawLine(1.0, 1.0, -1.0, 1.0, 1.0, -l);
	drawLine(1.0, 1.0, -1.0, 1.0, l, -1.0);
	drawLine(1.0, 1.0, -1.0, l, 1.0, -1.0);
	drawLine(-1.0, -1.0, 1.0, -1.0, -1.0, l);
	drawLine(-1.0, -1.0, 1.0, -1.0, -l, 1.0);
	drawLine(-1.0, -1.0, 1.0, -l, -1.0, 1.0);
	drawLine(-1.0, 1.0, -1.0, -1.0, 1.0, -l);
	drawLine(-1.0, 1.0, -1.0, -1.0, l, -1.0);
	drawLine(-1.0, 1.0, -1.0, -l, 1.0, -1.0);
	drawLine(1.0, -1.0, -1.0, 1.0, -1.0, -l);
	drawLine(1.0, -1.0, -1.0, 1.0, -l, -1.0);
	drawLine(1.0, -1.0, -1.0, l, -1.0, -1.0);
	drawLine(-1.0, -1.0, -1.0, -1.0, -1.0, -l);
	drawLine(-1.0, -1.0, -1.0, -1.0, -l, -1.0);
	drawLine(-1.0, -1.0, -1.0, -l, -1.0, -1.0);
}

void drawUnitCubeLabels(){
	text("1 1 1", 1, 1, 1);
	text("-1 1 1", -1, 1, 1);
	text("1 -1 1", 1, -1, 1);
	text("1 1 -1", 1, 1, -1);
	text("-1 -1 1", -1, -1, 1);
	text("-1 1 -1", -1, 1, -1);
	text("1 -1 -1", 1, -1, -1);
	text("-1 -1 -1", -1, -1, -1);
}

void setup() {
	HANDED = RIGHT;
	noFill();
	// glutReshapeWindow(400, 400);
	// hideHelpfulOrientation();
	polarPerspective();
	OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_FUNCTIONS;
	HORIZON[0] = 0;
	HORIZON[1] = 0;
	HORIZON[2] = sqrt( 1 + powf(4,zoom));
}
void update() { 
	generateSiderealTimeMatrix(siderealMatrix, ELAPSED);
	latitude = 30*sin(ELAPSED*0.1);
	generateGeographicLatitudeMatrix(latitudeMatrix, latitude);
}
void draw3D() {
	renderStars();

	glPushMatrix(); // ecliptic to equatorial
		glMultMatrixf(eclipticToEquatorial);

		glPushMatrix(); // sidereal time
			glMultMatrixf(siderealMatrix);

			glPushMatrix();
				glMultMatrixf(latitudeMatrix);

				// horizon
				glPushMatrix();
					glMultMatrixf(m);
					glTranslatef(0.0, 0.0, 1.0);
					glScalef(0.15, 0.15, 0.15);
					glColor3f(1.0, 1.0, 1.0);
					drawAxesLabels(1);
					drawUnitCubeWireframe();
					glColor3f(1.0, 1.0, 0.0);
					// drawUnitCubeLabels();
				glPopMatrix();

			glPopMatrix();

			// Earth
			glPushMatrix();
				glColor3f(0.3, 0.6, 1.0);
				drawUnitOriginSphereWireframe(6);
				glColor3f(1.0, 1.0, 1.0);
				drawAxesLabels(1);
				drawUnitCubeWireframe();
				glColor3f(0.3, 0.6, 1.0);
				// drawUnitCubeLabels();
			glPopMatrix();

			// latitude line
			// for(float pos = 1.0/3; pos < 1.0; pos += 1.0/3){
			glPushMatrix();
				float pos = sin(latitude*D2R);
				float r = cosf(pos*M_PI*0.5);
				r = sqrt(1 - powf(pos,2));
				glScalef(r, r, 1.0);
				glColor3f(0.2, 0.8, 0.2);
				drawUnitCircle(0, 0, pos);
			glPopMatrix();

		glPopMatrix(); // sidereal time

		// celestial equator
		glColor3f(0.4, 0.4, 0.4);
		drawCircle(0, 0, 0, 480);

	glPopMatrix(); // ecliptic to equatorial

	glColor3f(0.5, 0.5, 0.5);
	drawAxesLabels(1);
	drawUnitCubeWireframe();

	// ecliptic equator
	glColor3f(0.4, 0.4, 0.4);
	drawCircle(0, 0, 0, 490);
	// vernal equinox. Right Ascention 0
	glColor3f(1.0, 0.0, 0.0);
	drawLine(500, 0, 0, 1, 0, 0);
	text("vernal equinox", 450, 0, 0);

	// celestial sphere
	glPushMatrix();
		glScalef(500, 500, 500);
		glColor3f(0.2, 0.2, 0.2);
		drawUnitOriginSphereWireframe(6);
	glPopMatrix();
}
void draw2D() { }
void keyDown(unsigned int key) {
	if(key == ' '){
		zoom = (zoom+1)%4;
		HORIZON[2] = sqrt( 1 + powf(4,zoom));
	}
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }