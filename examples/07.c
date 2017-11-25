
#include "../world.h"

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
	// time = time + 1.42;
	// time = time + M_PI*0.5; // i dunno why. sorry.
	time = time + 1.5;
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

char zodiacs[][50] = {"Aries","Taurus","Gemini","Cancer","Leo","Virgo","Libra","Scorpio","Saggitarius","Capricorn","Aquarius","Pisces"};
char planetNames[][50] = {"Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto"};
char monthStrings[][50] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
static float colors[] = {0.75,0.75,0.75,0.8,0.67,113/256.0,0/256.0,19/256.0,174/256.0,0.67,81/256.0,40/256.0,186/256.0,130/256.0,83/256.0,253/256.0,196/256.0,126/256.0,149/256.0,188/256.0,198/256.0,98/256.0,119/256.0,226/256.0,0.75,0.75,0.75};

int year = 2016;
int month = 12;
int day = 21;
int hour = 0;
int minute = 0;
int second = 0;

double planets[9][3];
double moonPosition[3];

double planetProjections[9][3];
double moonProjection[3];
double sunProjection[3];
double planetAngles[9];
double moonAngle;
double sunAngle;

int zodiac = 0;

int clockSpeed = 5;

static float lastAngle;

GLuint dot;
GLuint constellationTexture = 0;

typedef enum { user, follow } ModeState;
ModeState MODE = user;

unsigned char isFileOpen = 0;


FILE *file;
void openFile(){
	char filename[128] = "calendar-data.csv\0";
	char path[128] = "../examples/data/\0";
	strcat(path, filename);
	file = fopen(path, "w");
	fprintf(file, "Year, Month, Day, Hour, Minute, Second, Sun, Moon, ");
	for(int i = 0; i < 9; i++){ 
		if(i != 2){
			fprintf(file, "%s, ", planetNames[i]);
		}
	}
	fprintf(file, "Zodiac\n");
}
void closeFile(){
	fclose(file);
}


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

void setup(){
	OPTIONS ^= SET_SHOW_GROUND;
	OPTIONS ^= SET_SHOW_GRID;
	OPTIONS ^= SET_KEYBOARD_MOVE;

	polarPerspective();
	HORIZON[0] = -95;
	HORIZON[1] = 15;
	HORIZON[2] = 16;
	// zoom orthographic projection
	WINDOW[0] = (-17000*0.5) * ASPECT;
	WINDOW[1] = -17000*0.5;
	WINDOW[2] = 17000 * ASPECT;
	WINDOW[3] = 17000;

	dot = loadTexture("../examples/data/dot-black.raw", 64, 64);
	// constellationTexture = loadTexture("../examples/data/constellations.raw", 1024, 512);

	ZOOM_SPEED = 10.0f;

	HANDED = RIGHT;

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
	double clockTime = j2000Centuries(year, month, day, hour, minute, second);

	for(int i = 0; i < 9; i++){
		calculateLocationOfPlanet(i, clockTime, &planets[i][0], &planets[i][1], &planets[i][2]);
	}
	// calculateLocationOfMoon(clockTime*36525.0+1.625, &moonPosition[0], &moonPosition[1], &moonPosition[2] );
	calculateLocationOfMoon(clockTime*36525.0, &moonPosition[0], &moonPosition[1], &moonPosition[2] );

	float lineLen = 800;
	// planets
	for(int i = 0; i < 9; i++){
		if(i != 2){
			float dX = planets[i][0] - planets[2][0];
			float dY = planets[i][1] - planets[2][1];
			float dZ = planets[i][2] - planets[2][2];
			float mag = sqrt( powf(dX,2) + powf(dY,2) + powf(dZ,2) );
			dX = dX / mag * lineLen;
			dY = dY / mag * lineLen;
			dZ = dZ / mag * lineLen;
			planetProjections[i][0] = planets[2][0]+dX;
			planetProjections[i][1] = planets[2][1]+dY;
			planetProjections[i][2] = planets[2][2]+dZ;
			planetAngles[i] = atan2(dY, dX);
			if(planetAngles[i] < 0) planetAngles[i] += M_PI*2;
		}
		else{
			planetProjections[i][0] = 0.0;
			planetProjections[i][1] = 0.0;
			planetProjections[i][2] = 0.0;
			planetAngles[i] = 0.0;
		}
	}
	// moon
	{
		float dX = moonPosition[0];
		float dY = moonPosition[1];
		float dZ = moonPosition[2];
		float mag = sqrt( powf(dX,2) + powf(dY,2) + powf(dZ,2) );
		dX = dX / mag * lineLen;
		dY = dY / mag * lineLen;
		dZ = dZ / mag * lineLen;
		moonProjection[0] = planets[2][0]+dX;
		moonProjection[1] = planets[2][1]+dY;
		moonProjection[2] = planets[2][2]+dZ;
		moonAngle = atan2(dY, dX);
		if(moonAngle < 0) moonAngle += M_PI*2;
	}
	// sun
	{
		float mag = sqrt( powf(planets[2][0],2) + powf(planets[2][1],2) + powf(planets[2][2],2) ) * lineLen;
		sunProjection[0] = -planets[2][0]*mag;
		sunProjection[1] = -planets[2][1]*mag;
		sunProjection[2] = -planets[2][2]*mag;
		sunAngle = atan2(-planets[2][1], -planets[2][0]);
		if(sunAngle < 0) sunAngle += M_PI*2;
	}


	if(MODE == follow){
		float newAngle = atan2(-planets[2][1], planets[2][0]);
		// newAngle += cosf(ELAPSED)*0.1;
		HORIZON[0] += 180 / M_PI * (newAngle - lastAngle);
		lastAngle = newAngle;
	}

	float sunAngle = atan2(-planets[2][1], -planets[2][0]);
	if(sunAngle < 0){ sunAngle += M_PI*2; }
	zodiac = sunAngle * 180 / M_PI / 30;

	switch(clockSpeed){
		case 0: day-=50; break;
		case 1: day-=5; break;
		case 2: day--; break;
		case 3: hour--; break;
		case 4: minute--; break;
		case 5:           break;
		case 6: minute++; break;
		case 7: hour++; break;
		case 8: day++; break;
		case 9: day+=5; break;
		case 10: day+=50; break;
	}
	if(year < 1800){ year = 2050; }
	if(year > 2050){ year = 1800; }

	correctDates();

	if(isFileOpen){
		fprintf(file, "%d, %d, %d, %d, %d, %d, ", year, month, day, hour, minute, second);
		fprintf(file, "%f, ", sunAngle * 180 / M_PI);
		fprintf(file, "%f, ", moonAngle * 180 / M_PI);
		for(int i = 0; i < 9; i++){
			if(i != 2){
				fprintf(file, "%f, ", planetAngles[i] * 180 / M_PI);
			}
		}
		// fprintf(file, "%d", zodiac+1);
		fprintf(file, "%s", zodiacs[zodiac]);
		fprintf(file, "\n");
	}
}

void draw3D(){
	glEnable(GL_CULL_FACE);
	glScalef(8, 8, 8);

	glColor4f(1.0, 1.0, 1.0, 0.75);
	drawLine(1000,0,0,-1000,0,0);
	// glColor4f(1.0, 1.0, 1.0, 0.25);
	// drawLine(0,1000,0,0,-1000,0);

	glCullFace(GL_BACK);

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

	glCullFace(GL_FRONT);

	// sun
	glPushMatrix();
		fill();
		glColor3f(242/256.0,229/256.0,129/256.0);
		glRotatef(90,0,1,0);
		drawIcosahedron(0.2);
		noFill();
		glColor3f(0, 0, 0);
		drawIcosahedron(.201);
	glPopMatrix();

	// planets
	for(int i = 0; i < 9; i++){
		if(i == 2){ i++; }
		glPushMatrix();
			glTranslatef(planets[i][0], planets[i][1], planets[i][2]);
			fill();
			glColor3f(colors[3*i+0],colors[3*i+1],colors[3*i+2]);
			glRotatef(90,0,1,0);
			// glRotatef(23.4,0,0,1);
			drawIcosahedron(.1);
			noFill();
			glColor3f(0, 0, 0);
			drawIcosahedron(.101);
		glPopMatrix();
	}

	glLineWidth(4);
	// 12 zodiac divisions
	glColor4f(1.0, 0.1, 0.1, 0.2);
	glPushMatrix();
	glRotatef(90,1,0,0);
	for(int i = 0; i < 12; i++){
		drawCircle(0,0,0,950);
		glRotatef(30,0,1,0);
	}
	glPopMatrix();
	glLineWidth(1);

	// earth celestial sphere
	noFill();
	glPushMatrix();
		// earth position
		glTranslatef(planets[2][0], planets[2][1], planets[2][2]);
		glPushMatrix();
		// 	// moon
			glTranslatef(moonPosition[0]*50, moonPosition[1]*50, moonPosition[2]*50);
			if(moonPosition[2] < 0) { glColor4f(1.0, 0.6, 0.6, 1.0); }
			else  { glColor4f(0.6, 0.6, 1.0, 1.0); }
			// glColor4f(0.6, 0.6, 0.6, 1.0);
			fill();
			drawIcosahedron(.1);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			noFill();
			drawIcosahedron(.101);
		glPopMatrix();

		glRotatef(-23.4,1,0,0);
	glLineWidth(4);
	glColor4f(.2, .3, .9, 1.0);
	drawCircle(0,0,0,950);
	glLineWidth(1);
		glColor4f(colors[3*2+0]*1.6,colors[3*2+1]*1.6,colors[3*2+2]*1.6, 1.0);
		drawSphere(0,0,0,.1);
		glPushMatrix();
			fill();
			glColor4f(colors[3*2+0],colors[3*2+1],colors[3*2+2], 1.0);
			glRotatef(90,0,1,0);
			drawIcosahedron(.1);
			noFill();
			glColor3f(0, 0, 0);
			drawIcosahedron(.101);
		glPopMatrix();
		glColor4f(1.0, 1.0, 1.0, 0.8);
		drawCircle(0,0,0,.101);
	glPopMatrix();


	if(PERSPECTIVE != ORTHO){
		fill();
		glBindTexture(GL_TEXTURE_2D, dot);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);
		glCullFace(GL_FRONT);
		glPushMatrix();
			glTranslatef(0,0,-50);
			glScalef(600, 600, 600);
			glTranslatef(-0.5,-0.5,0);
			drawUnitSquare(0,0,0);
		glPopMatrix();
		glCullFace(GL_BACK);
		glPushMatrix();
			glTranslatef(0,0,50);
			glScalef(600, 600, 600);
			glTranslatef(-0.5,-0.5,0);
			drawUnitSquare(0,0,0);
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		glCullFace(GL_FRONT);
	}


	// lines from Earth to each planet
	fill();

	for(int i = 0; i < 9; i++){
		if(i != 2){
			glColor4f(colors[3*i+0], colors[3*i+1], colors[3*i+2], 0.5);
			// drawLine(planets[2][0], planets[2][1], planets[2][2], planets[i][0], planets[i][1], planets[i][2]);
			drawLine(planets[2][0], planets[2][1], planets[2][2],
			         planetProjections[i][0], planetProjections[i][1], planetProjections[i][2]);
			glPushMatrix();
				glTranslatef(planetProjections[i][0]*(1+(i+1)*0.02),
				             planetProjections[i][1]*(1+(i+1)*0.02),
				             planetProjections[i][2]*(1+(i+1)*0.02));
				glRotatef(90,1,0,0);
				glRotatef(90+planetAngles[i]/M_PI*180.0,0,1,0);
				glColor4f(colors[3*i+0], colors[3*i+1], colors[3*i+2], 1.0);
				drawCircle(0, 0, 0, 40);
			glPopMatrix();
			// planet names
			glColor4f(1.0, 1.0, 1.0, 1.0);
			text(planetNames[i], planetProjections[i][0], planetProjections[i][1], planetProjections[i][2] + 50);
		}
	}

	// to sun
	{
		glColor3f(242/256.0,229/256.0,129/256.0);
		drawLine(planets[2][0], planets[2][1], planets[2][2], sunProjection[0], sunProjection[1], sunProjection[2] );
		glPushMatrix();
			glTranslatef(sunProjection[0], sunProjection[1], sunProjection[2]);
			glRotatef(90,1,0,0);
			glRotatef(90+sunAngle/M_PI*180.0,0,1,0);
			drawCircle(0, 0, 0, 40);
			// drawSphere(planets[2][0]+dX, planets[2][1]+dY, planets[2][2]+dZ, 4);
		glPopMatrix();
		// sun name
		glColor4f(1.0, 1.0, 1.0, 1.0);
		text("Sun", sunProjection[0], sunProjection[1], sunProjection[2] + 50);
	}
	// to moon
	{
		glColor3f(0.6, 0.6, 0.6);
		drawLine(planets[2][0], planets[2][1], planets[2][2], moonProjection[0], moonProjection[1], moonProjection[2] );
		glPushMatrix();
			glTranslatef(moonProjection[0], moonProjection[1], moonProjection[2]);
			glRotatef(90,1,0,0);
			glRotatef(90+moonAngle/M_PI*180.0,0,1,0);
			drawCircle(0, 0, 0, 40);
		glPopMatrix();
		// moon names
		glColor4f(1.0, 1.0, 1.0, 1.0);
		text("Earth's Moon", moonProjection[0], moonProjection[1], moonProjection[2] + 50);
	}

	// planet names
	glColor4f(1.0, 1.0, 1.0, 1.0);
	for(int i = 0; i < 9; i++){
		if(i != 2)
		text(planetNames[i], planets[i][0], planets[i][1], planets[i][2]+.2);
	}

	// zodiac names
	for(int i = 0; i < 12; i++){
		float angle = i/12.0*2.0*M_PI + 0.36;
		glColor4f(1.0, 0.1, 0.1, 1.0);
		if(i == zodiac){ glColor4f(1.0, 1.0, 0.3, 1.0); }
		text(zodiacs[i], 900*cosf(angle), 900*sinf(angle), 85.0);
	}

	// draw ecliptic planes, AU units
	noFill();
	glColor4f(1.0, 1.0, 1.0, 0.5);
	drawCircle(0,0,0,1000);
	glColor4f(1.0, 1.0, 1.0, 0.15);
	for(int i = 0; i < 20; i++){
		drawCircle(0, 0, 0, powf(2,i));
	}
	glColor4f(1.0, 1.0, 1.0, 1.0);
}
void draw2D(){
	glColor3f(1.0, 1.0, 1.0);
	char dateString[50];
	sprintf(dateString, "%d %s %d", year, monthStrings[month-1], day);
	text(dateString, 10, 10, 0);
	char timeString[50];
	sprintf(timeString, "%02d:%02d:%02d", hour, minute, second);
	text(timeString, 200, 10, 0);
}
void keyDown(unsigned int key){
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
		if(clockSpeed > 10){ clockSpeed = 10; }
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
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }
