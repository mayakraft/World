#include "world.h"

const unsigned int NUM_CONSTELLATIONS = 20;

// LOAD STARS
// #include "1619stars.c"
// unsigned int NUM_STARS = 1619;
#include "31608stars.c"
unsigned int NUM_STARS = 31608;


GLuint texture;
GLuint texture_lines;
GLuint constellationTex[NUM_CONSTELLATIONS];

float latitude = 0;
float longitude = 0;

unsigned char fixedStars = 1;

float matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

float constellationMatrix[NUM_CONSTELLATIONS][16];

void mat4x4Mult(const float *a, const float *b, float *c){
	c[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12]; 
	c[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13]; 
	c[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14]; 
	c[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15]; 
	c[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12]; 
	c[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13]; 
	c[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14]; 
	c[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15]; 
	c[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12]; 
	c[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13]; 
	c[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14]; 
	c[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15]; 
	c[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12]; 
	c[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13]; 
	c[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14]; 
	c[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]; 
}
// multiply two 3x3 matrices, a and b, store result into c
void mat3x3Mult(const float* a, const float* b, float* c) {
	c[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
	c[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
	c[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];
	c[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	c[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	c[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
	c[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	c[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	c[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
}
void makeMat3XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;
	m[3] = 0;	m[4] = cos(angle);	m[5] = -sin(angle);
	m[6] = 0;	m[7] = sin(angle);	m[8] = cos(angle);
}
void makeMat3YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);
	m[3] = 0;			m[4] = 1;	m[5] = 0;
	m[6] = sin(angle);	m[7] = 0;	m[8] = cos(angle);
}
void makeMat3ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;
	m[3] = sin(angle);	m[4] = cos(angle);	m[5] = 0;
	m[6] = 0;			m[7] = 0;			m[8] = 1;
}
void makeMat4XRot(float *m, float angle){
	m[0] = 1;	m[1] = 0;			m[2] = 0;			m[3] = 0;
	m[4] = 0;	m[5] = cos(angle);	m[6] = -sin(angle);	m[7] = 0;
	m[8] = 0;	m[9] = sin(angle);	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;	m[13] = 0;			m[14] = 0;			m[15] = 1;
}
void makeMat4YRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = 0;	m[2] = -sin(angle);	m[3] = 0;
	m[4] = 0;			m[5] = 1;	m[6] = 0;			m[7] = 0;
	m[8] = sin(angle);	m[9] = 0;	m[10] = cos(angle);	m[11] = 0;
	m[12] = 0;			m[13] = 0;	m[14] = 0;			m[15] = 1;
}
void makeMat4ZRot(float *m, float angle){
	m[0] = cos(angle);	m[1] = -sin(angle);	m[2] = 0;	m[3] = 0;
	m[4] = sin(angle);	m[5] = cos(angle);	m[6] = 0;	m[7] = 0;
	m[8] = 0;			m[9] = 0;			m[10] = 1;	m[11] = 0;
	m[12] = 0;			m[13] = 0;			m[14] = 0;	m[15] = 1;
}
void setMat3FromMat4(float *m3, float *m4){
	m3[0] = m4[0]; m3[1] = m4[1]; m3[2] = m4[2];
	m3[3] = m4[4]; m3[4] = m4[5]; m3[5] = m4[6];
	m3[6] = m4[8]; m3[7] = m4[9]; m3[8] = m4[10];
}
void setMat4FromMat3(float *m4, float *m3){
	m4[0] = m3[0]; m4[1] = m3[1]; m4[2] = m3[2]; m4[3] = 0;
	m4[4] = m3[3]; m4[5] = m3[4]; m4[6] = m3[5]; m4[7] = 0;
	m4[8] = m3[6]; m4[9] = m3[7]; m4[10] = m3[8]; m4[11] = 0;
	m4[12] = 0; m4[13] = 0; m4[14] = 0; m4[15] = 1;
}
void setMat4Identity(float *m){
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

float launchBeginX = 0;
float launchBeginY = 0;

void renderStars(){
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _star_vertices);
	glDrawArrays(GL_POINTS, 0, NUM_STARS);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

// void spherePoints(int samples){
//     int rnd = arc4random()%samples;
//     float points[samples];
//     float offset = 2.0/samples;
//     float increment = M_PI * (3.0 - sqrt(5.0));

//     for(int i = 0; i < samples; i++){
//         y = ((i * offset) - 1) + (offset / 2);
//         r = math.sqrt(1 - pow(y,2))

//         phi = ((i + rnd) % samples) * increment

//         x = math.cos(phi) * r
//         z = math.sin(phi) * r

//         points.append([x,y,z])
// 	}
//     return points

// }

void setup() {
	GRID = 0;
	// texture = loadTexture("texture.raw", 32, 32);
	// texture = loadTexture("spectrum.raw", 512, 256);
	texture = loadTexture("stars.raw", 2048, 1024);
	// texture = loadTexture("constellations.raw", 1024, 512);
	texture_lines = loadTexture("lines.raw", 512, 256);

	// constellations
	const char *constellationFiles[20];
	constellationFiles[0] = "constellations/Bearer-of-the-Demons-Head.raw";
	constellationFiles[1] = "constellations/CentaurAndThe-Beast-of-Prey.raw";
	constellationFiles[2] = "constellations/Cetus.raw";
	constellationFiles[3] = "constellations/Inflammatus.raw";
	constellationFiles[4] = "constellations/Part-of-the-Horse.raw";
	constellationFiles[5] = "constellations/The-Archer.raw";
	constellationFiles[6] = "constellations/The-Arrow.raw";
	constellationFiles[7] = "constellations/The-Balance.raw";
	constellationFiles[8] = "constellations/The-Brave.raw";
	constellationFiles[9] = "constellations/The-Bull.raw";
	constellationFiles[10] = "constellations/The-Censer.raw";
	constellationFiles[11] = "constellations/The-Crab.raw";
	constellationFiles[12] = "constellations/The-Dolphin.raw";
	constellationFiles[13] = "constellations/The-Dragon.raw";
	constellationFiles[14] = "constellations/The-Eagle.raw";
	constellationFiles[15] = "constellations/The-Giant.raw";
	constellationFiles[16] = "constellations/The-Great-Cup.raw";
	constellationFiles[17] = "constellations/The-Great-Horse.raw";
	constellationFiles[18] = "constellations/The-Greater-Bear.raw";
	constellationFiles[19] = "constellations/The-Greater-Dog.raw";

	for(int i = 0; i < NUM_CONSTELLATIONS; i++){
		// setMat4Identity(constellationMatrix[i]);
		constellationTex[i] = loadTexture(constellationFiles[i], 256, 256);
		// constellationTex[i] = loadTexture(constellationFiles[i], 512, 512);
		// float angle1 = arc4random()%1000;
		// float angle2 = arc4random()%1000;
		float angle1 = i * 10;
		float angle2 = i * M_PI / NUM_CONSTELLATIONS - M_PI*0.5;
		float rot1[16];
		float rot2[16];
		makeMat4XRot(rot1, angle1);
		makeMat4YRot(rot2, angle2);
		mat4x4Mult(rot1, rot2, constellationMatrix[i]);
	}

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat light_position1[] = { 0.0, 0.0, 5.0, 1.0 };
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glPointSize(1);
}

void update() { 
	static float SKY_INTERVAL = 0.0015;
	// static float SKY_INTERVAL = 0.1;
	float changeX = 0;
	float changeY = 0;
	float lookAzimuth = lookOrientation[0]/180.0*M_PI;
	if(keyboard[UP_KEY] || keyboard['W'] || keyboard['w']){
		changeX += SKY_INTERVAL * sinf(lookAzimuth);
		changeY += SKY_INTERVAL * -cosf(lookAzimuth);
	}
	if(keyboard[DOWN_KEY] || keyboard['S'] || keyboard['s']){
		changeX -= SKY_INTERVAL * sinf(lookAzimuth);
		changeY -= SKY_INTERVAL * -cosf(lookAzimuth);
	}
	if(keyboard[LEFT_KEY] || keyboard['A'] || keyboard['a']){
		changeX += SKY_INTERVAL * sinf(lookAzimuth+M_PI_2);
		changeY += SKY_INTERVAL * -cosf(lookAzimuth+M_PI_2);
	}
	if(keyboard[RIGHT_KEY] || keyboard['D'] || keyboard['d']){
		changeX -= SKY_INTERVAL * sinf(lookAzimuth+M_PI_2);
		changeY -= SKY_INTERVAL * -cosf(lookAzimuth+M_PI_2);
	}
	// latitude = changeX;
	// longitude = originY;

	float rot1[16];
	float rot2[16];
	float rot[16];
	float newMatrix[16];	
	makeMat4XRot(rot1, changeY);
	makeMat4YRot(rot2, changeX);
	mat4x4Mult(rot1, rot2, rot);
	mat4x4Mult(matrix, rot, newMatrix);
	memcpy(matrix, newMatrix, sizeof(float)*16);
}
void draw() {
	glDisable(GL_LIGHTING);

	// GLfloat light_position1[] = { originX, originY, 5.0 + originZ, 1.0 };
	// glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

	float brightness = 1.0/powf(2,fabs(originZ));
	GLfloat white[] = {brightness, brightness, brightness, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

	glPushMatrix();
	// world is fixed

		if(fixedStars){
			glPushMatrix();
				// glRotatef(-90, 1, 0, 0);
				glPushMatrix();
					glScalef(100, 100, 100);
					glMultMatrixf(matrix);
					renderStars();
				glPopMatrix();
			glPopMatrix();
		}



		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

		for(int i = 0; i < NUM_CONSTELLATIONS; i++){
			glPushMatrix();
				glMultMatrixf(matrix);
				glMultMatrixf(constellationMatrix[i]);
				glTranslatef(0, 0, 450);
				glScalef(300, 300, 300);
				glBindTexture(GL_TEXTURE_2D, constellationTex[i]);
				drawUnitSquare(-0.5, -0.5);
				glBindTexture (GL_TEXTURE_2D, 0);
			glPopMatrix();
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

	glPopMatrix();

	glPushMatrix();
	// world moves with person
		glTranslatef(originX - launchBeginX, originY - launchBeginY, originZ);
		if(GROUND){
			glPushMatrix();
				glTranslatef(0.0, 0.0, -1.0);
				glScalef(500, 500, 500);
				glColor3f(0.0, 0.0, 0.0);
				drawUnitSquare(-0.5, -0.5);
			glPopMatrix();
		}
		
		glColor3f(1.0, 1.0, 1.0);

		if(!fixedStars){
			glPushMatrix();
				// glRotatef(-90, 1, 0, 0);
				glPushMatrix();
					glScalef(10, 10, 10);
					glMultMatrixf(matrix);
					renderStars();
				glPopMatrix();
			glPopMatrix();
		}


	glPopMatrix();

	// glPushMatrix();
	// 	glScalef(100, 100, 100);
	// 	glRotatef(-90, 1, 0, 0);
	// 	glPushMatrix();
	// 			glRotatef(latitude*3, 0, 1, 0);
	// 		glPushMatrix();
	// 		glRotatef(-longitude*3, 1, 0, 0);
	// 			glBindTexture(GL_TEXTURE_2D, texture);
	// 			drawUnitSphere();
	// 			glBindTexture(GL_TEXTURE_2D, 0);
	// 		glPopMatrix();
	// 	glPopMatrix();

	// glPopMatrix();


	// glPushMatrix();
	// 	glScalef(90, 90, 90);
	// 	glRotatef(-90, 1, 0, 0);
	// 	glPushMatrix();
	// 		glRotatef(latitude*3, 0, 1, 0);
	// 		glRotatef(-longitude*3, 1, 0, 0);
	// 		glBindTexture(GL_TEXTURE_2D, texture_lines);
	// 		drawUnitSphere();
	// 		glBindTexture(GL_TEXTURE_2D, 0);
	// 	glPopMatrix();

	// glPopMatrix();

	
	glEnable(GL_LIGHTING);


	if(GROUND){
		glPushMatrix();
		float newX = modulusContext(originX, 2);
		float newY = modulusContext(originY, 2);
		glTranslatef(newX, newY, originZ);
		drawCheckerboard(newX, newY, 12);
		glPopMatrix();
	}

}
void keyDown(unsigned int key) {
	if(key == ' '){
		fixedStars = !fixedStars;
		launchBeginX = originX;
		launchBeginY = originY;
	}
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }