
#include "../world.h"
#include "../examples/data/518stars.c"
#include "../examples/data/1619stars.c"

double KeplersEquation(double E, double M, double e){
	double deltaM = M - ( E - (e*180.0/M_PI) * sin(E*M_PI/180.0) );
	double deltaE = deltaM / (1.0 - e*cos(E*M_PI/180.0));
	return E + deltaE;
}
void calculateLocationOfPlanet(int planet, double time, double *planet_x, double *planet_y, double *planet_z){
//                          a           e           I           L           w         omega 
//                         AU          rad         deg         deg         deg         deg
static double elm[]={0.38709927, 0.20563593, 7.00497902,252.25032350, 77.45779628, 48.33076593,
                     0.72333566, 0.00677672, 3.39467605,181.97909950,131.60246718, 76.67984255,
                     1.00000261, 0.01671123,-0.00001531,100.46457166,102.93768193,  0.0,
                     1.52371034, 0.09339410, 1.84969142, -4.55343205,-23.94362959, 49.55953891,
                     5.20288700, 0.04838624, 1.30439695, 34.39644051, 14.72847983,100.47390909,
                     9.53667594, 0.05386179, 2.48599187, 49.95424423, 92.59887831,113.66242448,
                     19.18916464,0.04725744, 0.77263783,313.23810451,170.95427630, 74.01692503,
                     30.06992276,0.00859048, 1.77004347,-55.12002969, 44.96476227,131.78422574,
                     39.48211675,0.24882730,17.14001206,238.92903833,224.06891629,110.30393684 };
//                         AU/Cy       rad/Cy      deg/Cy      deg/Cy         deg/Cy     deg/Cy
static double rate[]={0.00000037, 0.00001906,-0.00594749,149472.67411175, 0.16047689,-0.12534081,
                      0.00000390,-0.00004107,-0.00078890, 58517.81538729, 0.00268329,-0.27769418,
                      0.00000562,-0.00004392,-0.01294668, 35999.37244981, 0.32327364, 0.0,
                      0.00001847, 0.00007882,-0.00813131, 19140.30268499, 0.44441088,-0.29257343,
                     -0.00011607,-0.00013253,-0.00183714,  3034.74612775, 0.21252668, 0.20469106,
                     -0.00125060,-0.00050991, 0.00193609,  1222.49362201,-0.41897216,-0.28867794,
                     -0.00196176,-0.00004397,-0.00242939,   428.48202785, 0.40805281, 0.04240589,
                      0.00026291, 0.00005105, 0.00035372,   218.45945325,-0.32241464,-0.00508664,
                     -0.00031596, 0.00005170, 0.00004818,   145.20780515,-0.04062942,-0.01183482 };
	double a = elm[6*planet+0] + rate[6*planet+0]*time;	// (au) semi_major_axis
	double e = elm[6*planet+1] + rate[6*planet+1]*time;	//  ( ) eccentricity
	double I = elm[6*planet+2] + rate[6*planet+2]*time;	//  (°) inclination
	double L = elm[6*planet+3] + rate[6*planet+3]*time;	//  (°) mean_longitude
	double omega_bar = elm[6*planet+4] + rate[6*planet+4]*time;	//  (°) longitude_of_periapsis
	double OMEGA = elm[6*planet+5] + rate[6*planet+5]*time;	//  (°) longitude_of_the_ascending_node
	double omega = omega_bar - OMEGA;
	double M = L - omega_bar;
	while(M > 180) M-=360;  // in degrees
	double E = M + (e*180.0/M_PI) * sin(M*M_PI/180.0);  // E0
	for(int i = 0; i < 5; i++){
		E = KeplersEquation(E, M, e);
	}
	omega = omega * M_PI/180.0;
	E = E * M_PI/180.0;
	I = I * M_PI/180.0;
	OMEGA = OMEGA * M_PI/180.0;
	double x0 = a*(cos(E)-e);
	double y0 = a*sqrt((1.0-e*e))*sin(E);
	*planet_x = ( cos(omega)*cos(OMEGA) - sin(omega)*sin(OMEGA)*cos(I) )*x0 + ( -sin(omega)*cos(OMEGA) - cos(omega)*sin(OMEGA)*cos(I) )*y0;
	*planet_y = ( cos(omega)*sin(OMEGA) + sin(omega)*cos(OMEGA)*cos(I) )*x0 + ( -sin(omega)*sin(OMEGA) + cos(omega)*cos(OMEGA)*cos(I) )*y0;
	*planet_z = (            sin(omega)*sin(I)                         )*x0 + (                          cos(omega)*sin(I)            )*y0;
}
void calculateLocationOfMoon(double time, double *moon_x, double *moon_y, double *moon_z){
	//    a             e      w      M      i       node       n         P      Pw     Pnode     Ref.
	//  (km)                 (deg)   (deg)  (deg)    (deg)   (deg/day)  (days)   (yr)    (yr)
	// 384400.     0.0554   318.15   135.27   5.16   125.08   13.176358   27.322   5.997   18.600   1
	// moon calculation http://njsas.org/projects/tidal_forces/altaz/pausch/ppcomp.html#6

	// i dunno why. sorry.
	// time = time + 1.42;
	// time = time + M_PI*0.5;
	// time = time + 1.5;
	time = time + 1.6666666;

	double N = (125.1228 - 0.0529538083 * time) / 180.0 * M_PI;
	double i = 5.1454 / 180.0 * M_PI;
	double w = (318.0634 + 0.1643573223 * time) / 180.0 * M_PI;
	double e = 0.054900; //eccentricity
	double M = (115.3654 + 13.0649929509 * time) / 180.0 * M_PI ; //mean anomaly
	double E = M + e * sin(M) * ( 1.0 + e * cos(M) );
	// for(int i = 0; i < 5; i++){
	// 	E = E - ( E - e * sin(E) - M ) / ( 1 - e * cos(E) );
	// }
	// double a = 60.2666; // earth radius
	// double a = 385000; // km
	double a = 0.00257356604; // au
	double xv = a * ( cos(E) - e );
	double yv = a * ( sqrt(1.0 - e*e) * sin(E) );
	double v = atan2(yv, xv);
	double r = sqrt(xv*xv + yv*yv);
	*moon_x = r * ( cos(N) * cos(v+w) - sin(N) * sin(v+w) * cos(i) );
	*moon_y = r * ( sin(N) * cos(v+w) + cos(N) * sin(v+w) * cos(i) );
	*moon_z = r * ( sin(v+w) * sin(i) );
}


typedef enum { user, follow } ModeState;
ModeState MODE = user;

FILE *file;
unsigned char isFileOpen = 0;

char zodiacs[][50] = {"Aries","Taurus","Gemini","Cancer","Leo","Virgo","Libra","Scorpio","Saggitarius","Capricorn","Aquarius","Pisces"};
char planetNames[][50] = {"Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto"};
char monthStrings[][50] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
static float colors[] = {0.75,0.75,0.75,0.8,0.67,113/256.0,0/256.0,19/256.0,174/256.0,0.67,81/256.0,40/256.0,186/256.0,130/256.0,83/256.0,253/256.0,196/256.0,126/256.0,149/256.0,188/256.0,198/256.0,98/256.0,119/256.0,226/256.0,169/256.0,149/256.0,146/256.0};

float planetSiderealDays[] = {
	1.0/58.65,
	1.0/243,
	24/23.9416666,
	24/24.6233333,
	0.4135416666,
	24/10.656,
	-24/17.24,
	24/16.11,
	-24/153.2928
};

char moonEventDescriptions[][50] = {
	"New",
	"Waxing Crescent",
	"First Quarter",
	"Waxing Gibbous",
	"Full",
	"Waning Gibbous",
	"Third Quarter",
	"Waning Crescent"
};

int year = 2017;
int month = 12;
int day = 1;
int hour = 0;
int minute = 0;
int second = 0;

float universeScale = 10;
float coordinateScale = 500;

// heliocentric coordinates
double planets[9][3];
double moonPosition[3];

// geocentric observation data
double planetProjections[9][3];
double moonProjection[3];
double sunProjection[3];
double planetLongitude[9];
double planetLatitude[9];
double planetDistance[9];
double moonLongitude;
double moonLatitude;
double moonDistance;
double sunLongitude;
double sunDistance;
double moonPhase; // angle, 0 and 2PI are 0 sunlight. PI is full sunlight
double daylightHours;

int zodiac = 0;

int clockSpeed = 10;

unsigned char showText = 1;
unsigned char showCoordinates = 1;
unsigned char autoRotate = 1;
unsigned char showPlanetLabels = 1;

static float lastAngle;

GLuint dot;
GLuint constellationTexture = 0;
GLuint planetTextures[9];
GLuint moonTexture;

static float sunRadius = 15000.0; //696342.;
static float planetRadiuses[] = {2439.7, 6051.8, 6371.00, 3389.5, 69911., 58232., 25362., 24622., 1151.};

char planetTexturePath[][100] = {
	"../examples/data/mercury_map.raw",
	"../examples/data/venus_map.raw",
	"../examples/data/earth_map.raw",
	"../examples/data/mars_map.raw",
	"../examples/data/jupiter_map.raw",
	"../examples/data/saturn_map.raw",
	"../examples/data/uranus_map.raw",
	"../examples/data/neptune_map.raw",
	"../examples/data/pluto_map.raw",
};

void correctDates(){
	static unsigned int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if(year%4 == 0) monthDays[1] = 29;
	else monthDays[1] = 28;
	// time. simple
	if(minute < 0)       { hour--; minute += 60; }
	else if(minute > 60) { hour++; minute -= 60; }
	if(hour < 0)         { day--; hour += 24; }
	else if(hour > 23)   { day++; hour -= 24; }
	if(day < 1) {
		month--; 
		if(month < 1){ month += 12; year--; }
		day = monthDays[month-1];
	} else if( day > monthDays[month-1]) {
		month++;
		if(month > 12){ month -= 12; year++; }
		day = 1;
	}
}

void setupLights(){
	GLfloat sunlight[] =   {4.0f, 4.0f, 4.0f, 0.0f};
	GLfloat sunlight_position[] = { 0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunlight);
	glLightfv(GL_LIGHT0, GL_POSITION, sunlight_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void renderStars(){
	glPushMatrix();
		glRotatef(-23.4, 1, 0, 0);
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

void drawPlanet(int planetNumber, float x, float y, float z){
	static float PLANET_SCALE = 0.0015;
	fill();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, planetTextures[planetNumber]);
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(j2000Days(year, month, day, hour, minute, second) * 360 * planetSiderealDays[planetNumber], 0, 0, 1);
		drawSphere(0, 0, 0, sqrt(planetRadiuses[planetNumber]) * PLANET_SCALE );
		glBindTexture(GL_TEXTURE_2D, 0);
		// saturns rings
		if(planetNumber == 5){
			noFill();
			glDisable(GL_LIGHTING);
			float r1 = sqrt(planetRadiuses[planetNumber]) * PLANET_SCALE * 1.3;
			float r1_2 = sqrt(planetRadiuses[planetNumber]) * PLANET_SCALE * 1.9;
			float r2 = sqrt(planetRadiuses[planetNumber]) * PLANET_SCALE * 2.2;
			glColor4f(0.13, 0.13, 0.13, 1.0);
			for(int i = 0; i < 60; i++){
				float rr = (i/60.0) * (r1_2-r1) + r1;
				drawCircle(0, 0, 0, rr);
			}
			glColor4f(0.1, 0.1, 0.1, 1.0);
			for(int i = 0; i < 15; i++){
				float rr = (i/15.0) * (r2-r1_2) + r1_2;
				drawCircle(0, 0, 0, rr);
			}
			glEnable(GL_LIGHTING);
		}
	glPopMatrix();
}
void drawMoon(){
	fill();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
		// translate to earth's center
		glTranslatef(planets[2][0], planets[2][1], planets[2][2]);
		// translate to moon position
		glTranslatef(moonPosition[0]*70, moonPosition[1]*70, moonPosition[2]*70);
		glRotatef(moonLongitude*180/M_PI - 90, 0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, moonTexture);
		drawSphere(0, 0, 0, .05);
		glBindTexture(GL_TEXTURE_2D, 0);

		// glDisable(GL_LIGHTING);
		// if(moonPosition[2] < 0) { glColor4f(1.0, 0.1, 0.1, 1.0); }
		// else  { glColor4f(0.3, 0.3, 1.0, 1.0); }
		// noFill();
		// drawIcosahedron(.15);
		// glEnable(GL_LIGHTING);
	glPopMatrix();
}


void setup(){
	dot = loadTexture("../examples/data/dot-black-on-white.raw", 64, 64);
	// constellationTexture = loadTexture("../examples/data/constellations.raw", 1024, 512);
	for(int i = 0; i < 8; i++){
		switch(i){
			case 0: planetTextures[0] = loadTextureBGR("../examples/data/mercury_map.raw", 128, 64);
			case 1: planetTextures[1] = loadTextureBGR("../examples/data/venus_map.raw", 128, 64);
			case 2: planetTextures[2] = loadTextureBGR("../examples/data/earth_map.raw", 128, 64);
			case 3: planetTextures[3] = loadTextureBGR("../examples/data/mars_map.raw", 128, 64);
			case 4: planetTextures[4] = loadTextureBGR("../examples/data/jupiter_map.raw", 128, 64);
			case 5: planetTextures[5] = loadTextureBGR("../examples/data/saturn_map.raw", 128, 64);
			case 6: planetTextures[6] = loadTextureBGR("../examples/data/uranus_map.raw", 128, 64);
			case 7: planetTextures[7] = loadTextureBGR("../examples/data/neptune_map.raw", 128, 64);
			case 8: planetTextures[8] = loadTextureBGR("../examples/data/pluto_map.raw", 128, 64);
		}
		// planetTextures[i] = loadTexture(planetTexturePath[i], 128, 64);
	}
	moonTexture = loadTextureBGR("../examples/data/moon_map.raw", 128, 64);


	SETTINGS ^= SET_SHOW_GROUND;
	SETTINGS ^= SET_SHOW_GRID;
	SETTINGS ^= SET_KEYBOARD_MOVE;

	polarPerspective();
	HORIZON[0] = -95;
	HORIZON[1] = 15;
	HORIZON[2] = 16;
	// zoom orthographic projection
	FRAME[0] = (-1700*0.5) * ASPECT;
	FRAME[1] = -1700*0.5;
	FRAME[2] = 1700 * ASPECT;
	FRAME[3] = 1700;

	setupLights();

	ZOOM_SPEED = 1.0f;

	HANDED = RIGHT;

	// HORIZON[2] = coordinateScale * 1.2; 
	HORIZON[2] = universeScale * 1.5; 


	FAR_CLIP = 10000000.0;

	// // total solar eclipse, 2019 Chile
	// year = 2019;
	// month = 7;
	// day = 2;
	// hour = 19;
	// minute = 22;
	// second = 57;

	// // total solar eclipse, North America
	// year = 2017;
	// month = 8;
	// day = 21;
	// hour = 18;
	// minute = 25;
	// second = 35;

	// // total solar eclipse, 2001
	// year = 2001;
	// month = 6;
	// day = 21;
	// hour = 12;
	// minute = 04;
	// second = 46;

	// // total solar eclipse, 2009
	// year = 2009;
	// month = 7;
	// day = 22;
	// hour = 02;
	// minute = 36;
	// second = 25;

	// // total solar eclipse, 2026
	// year = 2026;
	// month = 8;
	// day = 12;
	// hour = 17;
	// minute = 47;
	// second = 06;

	// // transit of venus 2012
	// year = 2012;
	// month = 6;
	// day = 6;
	// hour = 1;
	// minute = 13;
	// second = 00;
}

void update(){
	if(keyboard['+'] || keyboard['-']){
		ZOOM_SPEED += 0.02;
	}

	double clockTime = j2000Centuries(year, month, day, hour, minute, second);

	for(int i = 0; i < 9; i++){
		calculateLocationOfPlanet(i, clockTime, &planets[i][0], &planets[i][1], &planets[i][2]);
	}
	// calculateLocationOfMoon(clockTime*36525.0+1.625, &moonPosition[0], &moonPosition[1], &moonPosition[2] );
	calculateLocationOfMoon(clockTime*36525.0, &moonPosition[0], &moonPosition[1], &moonPosition[2] );

	// planets
	for(int i = 0; i < 9; i++){
		if(i != 2){
			float dX = planets[i][0] - planets[2][0];
			float dY = planets[i][1] - planets[2][1];
			float dZ = planets[i][2] - planets[2][2];
			float mag = sqrt( powf(dX,2) + powf(dY,2) + powf(dZ,2) );
			dX = dX / mag;
			dY = dY / mag;
			dZ = dZ / mag;
			planetProjections[i][0] = dX;
			planetProjections[i][1] = dY;
			planetProjections[i][2] = dZ;
			planetLongitude[i] = atan2(dY, dX);
			planetLatitude[i] = M_PI*0.5 - acos( dZ );
			if(planetLongitude[i] < 0) planetLongitude[i] += M_PI*2;
			planetDistance[i] = mag;
		}
		else{
			// this is earth. the distance of earth from earth
			planetProjections[i][0] = 0.0;
			planetProjections[i][1] = 0.0;
			planetProjections[i][2] = 0.0;
			planetLongitude[i] = 0.0;
			planetDistance[i] = 0.0;
		}
	}
	// moon
	{
		float dX = moonPosition[0];
		float dY = moonPosition[1];
		float dZ = moonPosition[2];
		float mag = sqrt( powf(dX,2) + powf(dY,2) + powf(dZ,2) );
		dX = dX / mag;
		dY = dY / mag;
		dZ = dZ / mag;
		moonProjection[0] = dX;
		moonProjection[1] = dY;
		moonProjection[2] = dZ;
		moonLongitude = atan2(dY, dX);
		moonLatitude = M_PI*0.5 - acos(dZ );
		if(moonLongitude < 0) moonLongitude += M_PI*2;
		moonDistance = mag;
	}
	// sun
	{
		// float mag = sqrt( powf(planets[2][0],2) + powf(planets[2][1],2) + powf(planets[2][2],2) );
		sunProjection[0] = -planets[2][0];
		sunProjection[1] = -planets[2][1];
		sunProjection[2] = -planets[2][2];
		sunLongitude = atan2(-planets[2][1], -planets[2][0]);
		// sunLatitude = acos(sunProjection[2] / sqrt( powf(sunProjection[0],2) + powf(sunProjection[1],2) + powf(sunProjection[2],2) ) );
		if(sunLongitude < 0) sunLongitude += M_PI*2;
		sunDistance = sqrt( powf(planets[2][0],2) + powf(planets[2][1],2) + powf(planets[2][2],2) );
	}


	if(MODE == follow){
		float newAngle = atan2(-planets[2][1], planets[2][0]);
		// newAngle += cosf(ELAPSED)*0.1;
		HORIZON[0] += 180 / M_PI * (newAngle - lastAngle);
		lastAngle = newAngle;
	}

	float sunLongitude = atan2(-planets[2][1], -planets[2][0]);
	if(sunLongitude < 0){ sunLongitude += M_PI*2; }
	zodiac = sunLongitude * 180 / M_PI / 30;

	// this is for new york latitude. ~40 N
	daylightHours = 9.25 + 2.92 + 2.92*sin(sunLongitude);
	moonPhase = moonLongitude - sunLongitude;
	if(moonPhase < 0) { moonPhase += M_PI*2; }

	switch(clockSpeed){
		case 0: day-=50; break;
		case 1: day-=5; break;
		case 2: hour-=6; break;
		case 3: hour--; break;
		case 4: minute-=10; break;
		case 5: minute--; break;
		case 6:           break;
		case 7: minute++; break;
		case 8: minute+=10; break;
		case 9: hour++; break;
		case 10: hour+=6; break;
		case 11: day+=5; break;
		case 12: day+=50; break;
	}
	if(year < 1800){ year = 2050; }
	if(year > 2050){ year = 1800; }

	correctDates();


	// console log
	// {
	// 	printf("sun:%.3f %s ", sunLongitude * 180 / M_PI, zodiacs[zodiac]);
	// 	printf("%fhrs ", daylightHours);
	// 	for(int i = 0; i < 9; i++){
	// 		if(i != 2){
	// 			printf("%s ", planetNames[i]);
	// 			printf("%.3f ", planetLongitude[i] * 180 / M_PI);
	// 			printf("%.3f ", planetLatitude[i] * 180 / M_PI);
	// 		}
	// 	}
	// 	printf("moon %.3f ", moonLongitude * 180 / M_PI);
	// 	printf("%.3f ", moonLatitude * 180 / M_PI);
	// 	printf("%.3f\n", moonPhase * 180 / M_PI);
	// }


	if(isFileOpen){
		static float lastMoonAngle = -1;
		fprintf(file, "%d,%d,%d,%d,%d,%d,", year, month, day, hour, minute, second);
		fprintf(file, "%f,", sunLongitude * 180 / M_PI);
		fprintf(file, "%f,", sunDistance);
		fprintf(file, "%s,", zodiacs[zodiac]);
		fprintf(file, "%f,", daylightHours);
		for(int i = 0; i < 9; i++){
			if(i != 2){
				fprintf(file, "%f,", planetLongitude[i] * 180 / M_PI);
				fprintf(file, "%f,", planetLatitude[i] * 180 / M_PI);
				fprintf(file, "%f,", planetDistance[i]);
			}
		}
		fprintf(file, "%f,", moonLongitude * 180 / M_PI);
		fprintf(file, "%f,", moonLatitude * 180 / M_PI);
		fprintf(file, "%f,", moonDistance);
		fprintf(file, "%f", moonPhase * 180 / M_PI);
		fprintf(file, "\n");
	}

	if(autoRotate){
		HORIZON[0] += 0.15;
	}
}

void openFile(){
	char filename[128] = "calendar-data.csv\0";
	char path[128] = "../examples/data/\0";
	strcat(path, filename);
	file = fopen(path, "w");
	fprintf(file, "Year,Month,Day,Hour,Minute,Second,SunLongitude,SunDistance,Zodiac,Daylight,");
	for(int i = 0; i < 9; i++){
		if(i != 2){
			fprintf(file, "%sLongitude,", planetNames[i]);
			fprintf(file, "%sLatitude,", planetNames[i]);
			fprintf(file, "%sDistance,", planetNames[i]);
		}
	}
	fprintf(file, "MoonLongitude,MoonLatitude,MoonDistance,MoonPhase\n");
}
void closeFile(){
	fclose(file);
}


void draw3D(){
	glEnable(GL_CULL_FACE);

	// glCullFace(GL_BACK);
	// constellation sphere
	// fill();
	// glColor4f(1.0, 1.0, 1.0, 0.2);
	// glBindTexture(GL_TEXTURE_2D, constellationTexture);
	// glPushMatrix();
	// 	glRotatef(-23.4,1,0,0);
	// 	glRotatef(90,0,0,1);
	// 	glScalef(1,-1,1);
	// 	drawSphere(0,0,0,1000);
	// glPopMatrix();
	// glBindTexture(GL_TEXTURE_2D, 0);
	// noFill();

	glPushMatrix();
	glScalef(universeScale, universeScale, universeScale);

	glPushMatrix();
		glScalef(100, 100, 100);
		renderStars();
	glPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);

	// sun
	glDisable(GL_LIGHTING);
	glCullFace(GL_BACK);
	fill();
	glColor3f(247/256.0, 248/256.0, 243/256.0);
	drawSphere(0,0,0,0.11);
	glEnable(GL_LIGHTING);
	// planets
	for(int i = 0; i < 9; i++){
		if(i != 2){
			drawPlanet(i, planets[i][0], planets[i][1], planets[i][2]);
		}
	}
	// earth 
	glPushMatrix();
		glTranslatef(planets[2][0], planets[2][1], planets[2][2]);
		glRotatef(-23.4,1,0,0);
		drawPlanet(2, 0, 0, 0);
	glPopMatrix();
	// moon
	drawMoon();

	if(showPlanetLabels){
		glDisable(GL_LIGHTING);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		for(int i = 0; i < 9; i++){
			text(planetNames[i], planets[i][0], 
			                     planets[i][1], 
			                     planets[i][2] + sqrt(planetRadiuses[i])*0.002 );
		}
		text("Moon", planets[2][0] + moonPosition[0]*70, 
		             planets[2][1] + moonPosition[1]*70, 
		             planets[2][2] + moonPosition[2]*70 + 0.075 );
		text("Sun", 0,0,0 + 0.175 );
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();  // universe scale


	///////////////////////////////////////////////////////
	// PROJECTION LINES FROM EARTH
	if(showCoordinates){
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	fill();
	glLineWidth(1);
	glCullFace(GL_FRONT);
	// to planets
	for(int i = 0; i < 9; i++){
		if(i != 2){
			glPushMatrix();
			glColor4f(colors[3*i+0], colors[3*i+1], colors[3*i+2], 0.5);
			// glTranslatef(universeScale * planets[2][0], 
			             // universeScale * planets[2][1], 
			             // universeScale * planets[2][2] );
			drawLine(universeScale * planets[2][0], 
			         universeScale * planets[2][1],
			         universeScale * planets[2][2],
			         universeScale * planets[2][0] + coordinateScale * planetProjections[i][0],
			         universeScale * planets[2][1] + coordinateScale * planetProjections[i][1],
			         universeScale * planets[2][2] + coordinateScale * planetProjections[i][2]);
			glPushMatrix();
				// circle on celestial sphere
				glTranslatef(universeScale * planets[2][0] + coordinateScale * planetProjections[i][0],
				             universeScale * planets[2][1] + coordinateScale * planetProjections[i][1],
				             universeScale * planets[2][2] + coordinateScale * planetProjections[i][2]);
				glRotatef(90,1,0,0);
				glRotatef(90+planetLongitude[i]/M_PI*180.0,0,1,0);
				glColor4f(1.0, 1.0, 1.0, 1.0);
				drawCircle(0, 0, 0, 1);
			glPopMatrix();
			drawLine(universeScale * planets[2][0] + coordinateScale * planetProjections[i][0], 
					 universeScale * planets[2][1] + coordinateScale * planetProjections[i][1], 
					 universeScale * planets[2][2] + coordinateScale * planetProjections[i][2],
					 universeScale * planets[2][0] + coordinateScale * planetProjections[i][0], 
					 universeScale * planets[2][1] + coordinateScale * planetProjections[i][1], 
					 universeScale * planets[2][2] + 0 );
			glPopMatrix();
		}
	}

	// to sun
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawLine(universeScale * planets[2][0], 
	         universeScale * planets[2][1], 
	         universeScale * planets[2][2], 
	         universeScale * planets[2][0] + coordinateScale * sunProjection[0],
	         universeScale * planets[2][1] + coordinateScale * sunProjection[1],
	         universeScale * planets[2][2] + coordinateScale * sunProjection[2] );
	glPushMatrix();
		glTranslatef(universeScale * planets[2][0] + coordinateScale * sunProjection[0], 
		             universeScale * planets[2][1] + coordinateScale * sunProjection[1],
		             universeScale * planets[2][2] + coordinateScale * sunProjection[2]);
		glRotatef(90,1,0,0);
		glRotatef(90+sunLongitude/M_PI*180.0,0,1,0);
		drawCircle(0, 0, 0, 2);
	glPopMatrix();

	// to moon
	glColor4f(0.6, 0.6, 0.6, 1.0);
	drawLine(universeScale * planets[2][0], 
	         universeScale * planets[2][1], 
	         universeScale * planets[2][2], 
	         universeScale * planets[2][0] + coordinateScale * moonProjection[0],
	         universeScale * planets[2][1] + coordinateScale * moonProjection[1],
	         universeScale * planets[2][2] + coordinateScale * moonProjection[2] );
	glPushMatrix();
		glTranslatef(universeScale * planets[2][0] + coordinateScale * moonProjection[0],
		             universeScale * planets[2][1] + coordinateScale * moonProjection[1],
		             universeScale * planets[2][2] + coordinateScale * moonProjection[2]);
		glRotatef(90,1,0,0);
		glRotatef(90+moonLongitude/M_PI*180.0,0,1,0);
		glColor3f(1.0, 1.0, 1.0);
		drawCircle(0, 0, 0, 1);
	glPopMatrix();
	drawLine(universeScale * planets[2][0] + coordinateScale * moonProjection[0], 
	         universeScale * planets[2][1] + coordinateScale * moonProjection[1], 
	         universeScale * planets[2][2] + coordinateScale * moonProjection[2],
			 universeScale * planets[2][0] + coordinateScale * moonProjection[0], 
			 universeScale * planets[2][1] + coordinateScale * moonProjection[1], 
			 universeScale * planets[2][2] + 0 );
	}
	glEnable(GL_CULL_FACE);


	glPushMatrix();
	glScalef(coordinateScale, coordinateScale, coordinateScale);

	///////////////////////////////////////////////////////
	// TEXT
	if(showText){

	// planet names
	// glColor4f(1.0, 1.0, 1.0, 1.0);
	// for(int i = 0; i < 9; i++){
	// 	if(i != 2)
	// 	text(planetNames[i], planets[i][0], planets[i][1], planets[i][2]+.2);
	// }

	// sun text
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	text("Sun", planets[2][0]/coordinateScale*universeScale + sunProjection[0], 
	            planets[2][1]/coordinateScale*universeScale + sunProjection[1],
	            planets[2][2]/coordinateScale*universeScale + sunProjection[2] + .15);
	glColor4f(0.6, 0.6, 0.6, 1.0);
	char sunDetailString[50];
	sprintf(sunDetailString, "%0.2f lo", sunLongitude*180/M_PI);
	text(sunDetailString, planets[2][0]/coordinateScale*universeScale + sunProjection[0], 
	                      planets[2][1]/coordinateScale*universeScale + sunProjection[1],
	                      planets[2][2]/coordinateScale*universeScale + sunProjection[2] + .12);
	// moon text
	glColor4f(1.0, 1.0, 1.0, 1.0);
	text("Earth's Moon", planets[2][0]/coordinateScale*universeScale + moonProjection[0], 
	                     planets[2][1]/coordinateScale*universeScale + moonProjection[1], 
	                     planets[2][2]/coordinateScale*universeScale + moonProjection[2] + .05);
	glColor4f(0.6, 0.6, 0.6, 1.0);
	char moonDetailString[50];
	sprintf(moonDetailString, "%0.2f lo", moonLongitude*180/M_PI);
	text(moonDetailString, planets[2][0]/coordinateScale*universeScale + moonProjection[0], 
	                       planets[2][1]/coordinateScale*universeScale + moonProjection[1], 
	                       planets[2][2]/coordinateScale*universeScale + moonProjection[2] + .03);
	char moonDetailString2[50];
	sprintf(moonDetailString2, "   %0.2f la", moonLatitude*180/M_PI);
	text(moonDetailString2, planets[2][0]/coordinateScale*universeScale + moonProjection[0],
	                        planets[2][1]/coordinateScale*universeScale + moonProjection[1],
	                        planets[2][2]/coordinateScale*universeScale + moonProjection[2] - .01);
	// planet text
	for(int i = 0; i < 9; i++){
		if(i != 2){
			glColor4f(1.0, 1.0, 1.0, 1.0);
			text(planetNames[i], planets[2][0]/coordinateScale*universeScale + planetProjections[i][0], 
			                     planets[2][1]/coordinateScale*universeScale + planetProjections[i][1], 
			                     planets[2][2]/coordinateScale*universeScale + planetProjections[i][2] + .09);
			glColor4f(0.6, 0.6, 0.6, 1.0);
			char planetDetailString[50];
			sprintf(planetDetailString, "%0.2f lo", planetLongitude[i]*180/M_PI);
			text(planetDetailString, planets[2][0]/coordinateScale*universeScale + planetProjections[i][0], 
			                         planets[2][1]/coordinateScale*universeScale + planetProjections[i][1], 
			                         planets[2][2]/coordinateScale*universeScale + planetProjections[i][2] + .06);
			char planetDetailString2[50];
			sprintf(planetDetailString2, "   %0.2f la", planetLatitude[i]*180/M_PI);
			text(planetDetailString2, planets[2][0]/coordinateScale*universeScale + planetProjections[i][0], 
			                          planets[2][1]/coordinateScale*universeScale + planetProjections[i][1], 
			                          planets[2][2]/coordinateScale*universeScale + planetProjections[i][2] - .01);
		}
	}
	// zodiac names
	glPushMatrix();
		glTranslatef(planets[2][0]/coordinateScale*universeScale, 
	             planets[2][1]/coordinateScale*universeScale,
	             planets[2][2]/coordinateScale*universeScale );


		for(int i = 0; i < 12; i++){
			float angle = i/12.0*2.0*M_PI + 0.36;
			glColor4f(1.0, 0.1, 0.1, 1.0);
			if(i == zodiac){ glColor4f(1.0, 1.0, 0.3, 1.0); }
			text(zodiacs[i], cosf(angle), sinf(angle), 0.2);
		}
	glPopMatrix();
	}

	///////////////////////////////////////////////////////
	// COORDINATE LINES
	if(showCoordinates){
	glDisable(GL_LIGHTING);
	noFill();

	glPushMatrix(); // coordinate matrix

		glTranslatef(planets[2][0]/coordinateScale*universeScale, 
		             planets[2][1]/coordinateScale*universeScale,
		             planets[2][2]/coordinateScale*universeScale );


		//////////////////////////////////////////////////////////
		// draw ecliptic planes, AU units
		glColor4f(1.0, 1.0, 1.0, 0.3);

		// ecliptic equator
		drawCircle(0,0,0,1);
		// ecliptic node intersection
		drawLine(1,0,0,-1,0,0);
		// drawLine(0,100,0,0,-100,0);
		glPushMatrix();
			glRotatef(-23.4, 1, 0, 0);
			// celestial equator
			drawCircle(0,0,0,1);
		glPopMatrix();

		// solar system rings
		// glColor4f(1.0, 1.0, 1.0, 0.15);
		// for(int i = 0; i < 20; i++){
		// 	drawCircle(0, 0, 0, powf(2,i));
		// }

		// 12 zodiac divisions
		if(PERSPECTIVE != ORTHO){
		glLineWidth(2);
		// glColor4f(0.6, 0.06, 0.06, 1.0);  // red
		glColor4f(1.0, 1.0, 1.0, 0.1);
		glPushMatrix();
			glRotatef(90,1,0,0);
			for(int i = 0; i < 12; i++){
				drawCircle(0,0,0,1);
				glRotatef(30,0,1,0);
			}
		glPopMatrix();
		glLineWidth(1);

		// earth celestial sphere
		// glColor4f(.04, .07, .3, 1.0); // blue
		glColor4f(1.0, 1.0, 1.0, 0.07);
		glPushMatrix();
			glRotatef(-23.4,1,0,0);
			drawCircle(0,0,0,1);
			drawSphere(0,0,0,1);
		glPopMatrix();

		// upper and lower black fades
		glDisable(GL_CULL_FACE);
		fill();
		glColor4f(0.0, 0.0, 0.0, 0.2);
		float NUM_DARK_PANELS = 7.0;
		glPushMatrix();
			for(int i = NUM_DARK_PANELS; i >= 0; i--){
				float z1 =  0.5 + 0.5/NUM_DARK_PANELS*i;  // 0 to 1
				float z2 = -0.5 - 0.5/NUM_DARK_PANELS*i;  // 0 to 1
				// float r1 = sin((1-z1)*M_PI*0.5);
				// float r2 = sin((-1-z2)*M_PI*0.5);
				float r1 = sqrt(1-powf(z1,2));
				float r2 = sqrt(1-powf(z2,2));
				glPushMatrix();
					glScalef(r1, r1, 1);
					drawUnitCircle(0,0, z1);
				glPopMatrix();
				glPushMatrix();
					glScalef(r1, r1, 1);
					drawUnitCircle(0,0, z2);
				glPopMatrix();
			}
		glPopMatrix();
		noFill();
		}


	glPopMatrix(); // coordinate matrix
	}


	glPopMatrix(); // coordinate scale

	///////////////////////
	// reset
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
}
void draw2D(){
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	char dateString[50];
	sprintf(dateString, "%d %s %d  %02d:%02d:%02d UTC", year, monthStrings[month-1], day, hour, minute, second);
	text(dateString, WIDTH*0.5 - 120, HEIGHT - 30, 0);
}
void keyDown(unsigned int key){
	if(key == 'l' || key == 'L'){ showPlanetLabels = !showPlanetLabels; }
	if(key == 'r' || key == 'R'){ autoRotate = !autoRotate; }
	if (key == 't' || key == 'T'){ showText = !showText; }
	if (key == 'c' || key == 'C'){ showCoordinates = !showCoordinates; }
	if (key == ' '){
		if(MODE == follow){
			MODE = user;
		}
		else if(MODE == user){ 
			lastAngle = atan2(-planets[2][1], planets[2][0]);
			MODE = follow;
		}
		printf("MOON:  %f, %f, %f:  %f\n", moonPosition[0], moonPosition[1], moonPosition[2], atan2(moonPosition[1], moonPosition[0]));
	}
	if(key == ']'){
		clockSpeed++;
		if(clockSpeed > 12){ clockSpeed = 12; }
	}
	if(key == '['){
		clockSpeed--;
		if(clockSpeed < 0){ clockSpeed = 0; }
	}
	if(key == 'n'){
		year = YEAR;
		month = MONTH;
		day = DAY;
		hour = HOUR;
		minute = MINUTE;
		second = SECOND;
	}
	if(key == 'w' || key == 'W'){
		if(isFileOpen){
			closeFile();
			isFileOpen = 0;
		} else{
			openFile();
			isFileOpen = 1;
		}
	}
}
void keyUp(unsigned int key){
	if(key == '+' || key == '-'){
		ZOOM_SPEED = 0.1f;
	}
}
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }
void worldDelegate(unsigned int code){ }

