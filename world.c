#include "world.h"

const unsigned int NUM_CONSTELLATIONS = 46;
const unsigned int NUM_WARP_LINES = 80;

// LOAD STARS
// #include "1619stars.c"
// unsigned int NUM_STARS = 1619;
#include "31608stars.c"
unsigned int NUM_STARS = 31608;
// #include "518stars.c"
// unsigned int NUM_STARS = 518;


GLuint texture;
GLuint texture_lines;
GLuint constellationTex[NUM_CONSTELLATIONS];

unsigned int visibleConstellations[10];
unsigned int visibleWarpLines = 15;

float latitude = 0;
float longitude = 0;

unsigned char fixedStars = 1;
unsigned char showConstellations = 0;
unsigned char showTarget = 0;
unsigned char targetAcquired = 0;
unsigned char interplanetaryTravel = 0;

float matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

float constellationMatrix[NUM_CONSTELLATIONS][16];

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

void fillConstellationArray(unsigned int *a){
	int count = 0;
	for(int i = 0; i < 10; i++){
		a[i] = 99999;
	}
	while(count < 10){
		int try = arc4random()%NUM_CONSTELLATIONS;
		unsigned char exists = 0;
		for(int i = 0; i < count; i++){
			if(a[i] == try){
				exists = 1;
			}
		}
		if(!exists){
			a[count] = try;
			count++;
		}
	}
}

float warpAngles[NUM_WARP_LINES];
float warpDistances[NUM_WARP_LINES];

void setup() {

	time_t t;
	srand((unsigned) time(&t));

	GRID = 0;
	// texture = loadTexture("texture.raw", 32, 32);
	// texture = loadTexture("spectrum.raw", 512, 256);
	// texture = loadTexture("stars.raw", 2048, 1024);
	// texture = loadTexture("constellations.raw", 1024, 512);
	// texture_lines = loadTexture("lines.raw", 512, 256);

	setMat4Identity(matrix);

	for(int i = 0; i < NUM_WARP_LINES; i++){
		warpDistances[i] = arc4random()%400;
		warpAngles[i] = arc4random()%628 / 100.0;
	}

	// constellations
	const char *constellationFiles[NUM_CONSTELLATIONS];
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
	constellationFiles[20] = "constellations/The-Hare.raw";
	constellationFiles[21] = "constellations/The-Hen.raw";
	constellationFiles[22] = "constellations/The-Howler.raw";
	constellationFiles[23] = "constellations/The-Kneeling-Man.raw";
	constellationFiles[24] = "constellations/The-Lesser-Bear.raw";
	constellationFiles[25] = "constellations/The-Lesser-Dog.raw";
	constellationFiles[26] = "constellations/The-Lion.raw";
	constellationFiles[27] = "constellations/The-Maiden.raw";
	constellationFiles[28] = "constellations/The-Northern-Crown.raw";
	constellationFiles[29] = "constellations/The-Ram.raw";
	constellationFiles[30] = "constellations/The-Raven.raw";
	constellationFiles[31] = "constellations/The-Reins-holder.raw";
	constellationFiles[32] = "constellations/The-River.raw";
	constellationFiles[33] = "constellations/The-Scorpion.raw";
	constellationFiles[34] = "constellations/The-Shackled-Woman.raw";
	constellationFiles[35] = "constellations/The-Ship.raw";
	constellationFiles[36] = "constellations/The-Snake-Charmer.raw";
	constellationFiles[37] = "constellations/The-Southern-Crown.raw";
	constellationFiles[38] = "constellations/The-Southern-Whale.raw";
	constellationFiles[39] = "constellations/The-Triangle.raw";
	constellationFiles[40] = "constellations/The-Turtle.raw";
	constellationFiles[41] = "constellations/The-Twins.raw";
	constellationFiles[42] = "constellations/The-Well-bucket.raw";
	constellationFiles[43] = "constellations/The-Whale.raw";
	constellationFiles[44] = "constellations/The-Young-Goat.raw";
	constellationFiles[45] = "constellations/Who-Lady-Has-Chair.raw";

	for(int i = 0; i < NUM_CONSTELLATIONS; i++){
		constellationTex[i] = loadTexture(constellationFiles[i], 256, 256);
		float angle1 = i * 10;
		float angle2 = i * M_PI / NUM_CONSTELLATIONS - M_PI*0.5;
		float rot1[16];
		float rot2[16];
		makeMat4XRot(rot1, angle1);
		makeMat4YRot(rot2, angle2);
		mat4x4Mult(rot1, rot2, constellationMatrix[i]);
	}

	fillConstellationArray(visibleConstellations);

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

void logMat4(const float *m){
	printf("%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n", 
	m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

unsigned char mat4IdentitySimilarity(const float *m){
	float epsilon = 0.92; // how close to 1 do we need to be?
	if(m[10] > epsilon)
		return 1;
	return 0;
	// float epsilon = 0.9; // how close to 1 do we need to be?
	// if(m[0] > epsilon && m[5] > epsilon && m[10] > epsilon)
	// 	return 1;
	// return 0;
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

	for(int i = 0; i < NUM_WARP_LINES; i++){
		warpDistances[i] += 10 + arc4random()%3;
		if(warpDistances[i] > 500){
			warpDistances[i] = 3 + arc4random()%7;
		}
	}

	float rot1[16];
	float rot2[16];
	float rot[16];
	float newMatrix[16];	
	makeMat4XRot(rot1, changeY);
	makeMat4YRot(rot2, changeX);
	mat4x4Mult(rot1, rot2, rot);
	mat4x4Mult(matrix, rot, newMatrix);
	memcpy(matrix, newMatrix, sizeof(float)*16);


	if(interplanetaryTravel){
		originZ -= 5;
	}
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

		glColor3f(0.8, 0.8, 0.8);
		static const GLfloat _unit_line[] = { 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 10.0f };
		for(int i = 0; i < visibleWarpLines; i++){

			if(targetAcquired){
				float r = arc4random()%66 / 100.0;
				float g = arc4random()%66 / 100.0;
				float b = arc4random()%66 / 100.0;
				glColor3f(0.33 + r, 0.33 + g, 0.33 + b);
			}

			glPushMatrix();
				glTranslatef(5 * cosf(warpAngles[i]), 5 * sinf(warpAngles[i]), warpDistances[i]);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, _unit_line);
				glDrawArrays(GL_LINES, 0, 2);
				glDisableClientState(GL_VERTEX_ARRAY);
			glPopMatrix();
		}
		glColor3f(1.0, 1.0, 1.0);

		if(fixedStars){
			glColor3f(0.66, 0.66, 0.66);
			glPushMatrix();
				// glRotatef(-90, 1, 0, 0);
				glPushMatrix();
					glScalef(100, 100, 100);
					glMultMatrixf(matrix);
					renderStars();
				glPopMatrix();
			glPopMatrix();
			glColor3f(1.0, 1.0, 1.0);
		}




		if(showConstellations){
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			for(int i = 0; i < 10; i++){
				glPushMatrix();
					glMultMatrixf(matrix);
					glMultMatrixf(constellationMatrix[ visibleConstellations[i] ]);
					glTranslatef(0, 0, 450);
					glScalef(300, 300, 300);
					glBindTexture(GL_TEXTURE_2D, constellationTex[ visibleConstellations[i] ]);
					drawUnitSquare(-0.5, -0.5);
					glBindTexture (GL_TEXTURE_2D, 0);
				glPopMatrix();
			}
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}


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
			glColor3f(0.66, 0.66, 0.66);
			glPushMatrix();
				// glRotatef(-90, 1, 0, 0);
				glPushMatrix();
					glScalef(10, 10, 10);
					glMultMatrixf(matrix);
					renderStars();
				glPopMatrix();
			glPopMatrix();
			glColor3f(1.0, 1.0, 1.0);
		}


	glPopMatrix();

	static int count = 0;
	count++;
	if(count % 60 == 0){
		float m[16];
		mat4x4Mult(constellationMatrix[ visibleConstellations[0] ], matrix, m);
		if(mat4IdentitySimilarity(m)){
			targetAcquired = 1;
			visibleWarpLines = NUM_WARP_LINES;
		} else{
			targetAcquired = 0;
			visibleWarpLines = 15;
		}
	}

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



	if(showTarget){

		orthoPerspective(0, 0, WIDTH, HEIGHT);
		glPushMatrix();

		glDisable(GL_LIGHTING);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

		glBindTexture(GL_TEXTURE_2D, constellationTex[ visibleConstellations[0] ]);
		glScalef(HEIGHT, HEIGHT, HEIGHT);
		drawUnitSquare((WIDTH*0.5 - HEIGHT*0.5)/((float)HEIGHT), 0);
		glBindTexture (GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LIGHTING);

		glPopMatrix();

		firstPersonPerspective();

	}


}
void keyDown(unsigned int key) {
	// if(key == ' '){
	// 	fixedStars = !fixedStars;
	// 	launchBeginX = originX;
	// 	launchBeginY = originY;
	// }
	if(key == ' '){
		if(targetAcquired){
			fixedStars = 0;
			interplanetaryTravel = 1;
		} else{
			showTarget = !showTarget;
			launchBeginX = originX;
			launchBeginY = originY;			
		}
	}
	if(key == 'c' || key == 'C'){
		showConstellations = !showConstellations;
	}

}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }