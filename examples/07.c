#include "../world.h"

double KeplersEquation(double E, double M, double e){
	double deltaM = M - ( E - (e*180.0/M_PI) * sin(E*M_PI/180.0) );
	double deltaE = deltaM / (1.0 - e*cos(E*M_PI/180.0));
	return E + deltaE;
}
void calculateLocationOfPlanet(int planet, double time, double *planet_x, double *planet_y, double *planet_z){
//                              AU           rad          deg          deg          deg          deg
static double _elements[]= {0.38709927,  0.20563593,  7.00497902, 252.25032350, 77.45779628, 48.33076593,
                            0.72333566,  0.00677672,  3.39467605, 181.97909950,131.60246718, 76.67984255,
                            1.00000261,  0.01671123, -0.00001531, 100.46457166,102.93768193,  0.0,
                            1.52371034,  0.09339410,  1.84969142,  -4.55343205,-23.94362959, 49.55953891,
                            5.20288700,  0.04838624,  1.30439695,  34.39644051, 14.72847983,100.47390909,
                            9.53667594,  0.05386179,  2.48599187,  49.95424423, 92.59887831,113.66242448,
                            19.18916464, 0.04725744,  0.77263783, 313.23810451,170.95427630, 74.01692503,
                            30.06992276, 0.00859048,  1.77004347, -55.12002969, 44.96476227,131.78422574,
                            39.48211675, 0.24882730, 17.14001206, 238.92903833,224.06891629,110.30393684 };
//                         AU/Cy        rad/Cy       deg/Cy       deg/Cy          deg/Cy      deg/Cy
static double _rates[]= {0.00000037,  0.00001906, -0.00594749,149472.67411175,  0.16047689, -0.12534081,
                         0.00000390, -0.00004107, -0.00078890, 58517.81538729,  0.00268329, -0.27769418,
                         0.00000562, -0.00004392, -0.01294668, 35999.37244981,  0.32327364,  0.0,
                         0.00001847,  0.00007882, -0.00813131, 19140.30268499,  0.44441088, -0.29257343,
                        -0.00011607, -0.00013253, -0.00183714,  3034.74612775,  0.21252668,  0.20469106,
                        -0.00125060, -0.00050991,  0.00193609,  1222.49362201, -0.41897216, -0.28867794,
                        -0.00196176, -0.00004397, -0.00242939,   428.48202785,  0.40805281,  0.04240589,
                         0.00026291,  0.00005105,  0.00035372,   218.45945325, -0.32241464, -0.00508664,
                        -0.00031596,  0.00005170,  0.00004818,   145.20780515, -0.04062942, -0.01183482 };
	double a = _elements[6*planet+0] + _rates[6*planet+0]*time;	// (au) semi_major_axis
	double e = _elements[6*planet+1] + _rates[6*planet+1]*time;	//  ( ) eccentricity
	double I = _elements[6*planet+2] + _rates[6*planet+2]*time;	//  (°) inclination
	double L = _elements[6*planet+3] + _rates[6*planet+3]*time;	//  (°) mean_longitude
	double omega_bar = _elements[6*planet+4] + _rates[6*planet+4]*time;	//  (°) longitude_of_periapsis
	double OMEGA = _elements[6*planet+5] + _rates[6*planet+5]*time;	//  (°) longitude_of_the_ascending_node
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
	*planet_z = (			sin(omega)*sin(I)			 )*x0 + (			 cos(omega)*sin(I)			 )*y0;
}

static float colors[] = {192/256.0,192/256.0,192/256.0,206/256.0,172/256.0,113/256.0,0/256.0,19/256.0,174/256.0,172/256.0,81/256.0,40/256.0,186/256.0,130/256.0,83/256.0,253/256.0,196/256.0,126/256.0,149/256.0,188/256.0,198/256.0,98/256.0,119/256.0,226/256.0,192/256.0,192/256.0,192/256.0};

char zodiacs[][50] = {"Aries","Taurus","Gemini","Cancer","Leo","Virgo","Libra","Scorpio","Saggitarius","Capricorn","Aquarius","Pisces"};
char planets[][50] = {"Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto"};
char monthStrings[][50] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
// this approximates what day of the year we are in. not for precise calculation!
unsigned int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
unsigned int monthStartDay[12];
double planetsX[9];
double planetsY[9];
double planetsZ[9];
int year = 0; 
int month = 0;
int day = 0;
int zodiac = 0;
// double clockTime = .138767; // mid November 2013
double clockTime = .130767;
	static float lastAngle;

GLuint dot;


typedef enum{
	user,
	follow
}ModeState;

ModeState MODE = user;

void setup(){
	OPTIONS ^= SET_SHOW_GROUND;
	OPTIONS ^= SET_SHOW_GRID;

	polarPerspective();
	horizon[0] = -95;
	horizon[1] = 15;
	horizon[2] = 16;

	for(int i = 0; i < 12; i++){ 
		int totalDays = 0;
		for(int j = i; j >= 0; j--){
			totalDays += monthDays[j];
		}
		monthStartDay[i] = totalDays;
	}
	dot = loadTexture("../examples/data/dot-black.raw", 64, 64);
}

void update(){
	for(int i = 0; i < 9; i++){
		calculateLocationOfPlanet(i, clockTime, &planetsX[i], &planetsY[i], &planetsZ[i]);
	}
	clockTime += .00001;
	if(MODE == follow){
		float newAngle = atan2(-planetsY[2], planetsX[2]);
		newAngle += cosf(elapsed)*0.1;
		horizon[0] += 180 / M_PI * (newAngle - lastAngle);
		lastAngle = newAngle;

		// origin[0] = planetsX[2] * 10;
		// origin[1] = planetsY[2] * 10;
		// origin[2] = planetsZ[2] * 10;
	}

	year = (int)(clockTime*100.0+2000);
	float yearPct = clockTime*100.0+2000 - year;
	for(int i = 0; i < 12; i++){
		if(yearPct*365 < monthStartDay[i]){
			month = i;
			break;
		}
	}
	float sunAngle = atan2(-planetsY[2], -planetsX[2]);
	if(sunAngle < 0){ sunAngle += M_PI*2; }
	zodiac = sunAngle * 180 / M_PI / 30;
}

void draw3D(){
	glScalef(10, 10, 10);

	fill();
	glBindTexture(GL_TEXTURE_2D, dot);
	glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
	glBlendEquation(GL_FUNC_ADD);
	glPushMatrix();
		glTranslatef(0,0,-150);
		glScalef(600, 600, 600);
		glTranslatef(-0.5,-0.5,0);
		drawUnitSquare(0,0,0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0,150);
		glScalef(600, 600, 600);
		glTranslatef(-0.5,-0.5,0);
		drawUnitSquare(0,0,0);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);


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
			glTranslatef(planetsX[i], planetsY[i], planetsZ[i]);
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

	// 12 zodiac divisions
	glColor4f(1.0, 0.1, 0.1, 0.3);
	glPushMatrix();
	glRotatef(90,1,0,0);
	for(int i = 0; i < 12; i++){
		drawCircle(0,0,0,1000);
		glRotatef(30,0,1,0);
	}
	glPopMatrix();

	// earth celestial sphere
	glColor4f(colors[3*2+0],colors[3*2+1],colors[3*2+2], 1.0);
	noFill();
	glPushMatrix();
		glTranslatef(planetsX[2], planetsY[2], planetsZ[2]);
		glRotatef(-23.4,1,0,0);
		drawCircle(0,0,0,1000);
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


	// lines from Earth to each planet
	fill();
	float lineLen = 800;
	for(int i = 0; i < 9; i++){
		if(i != 2){
			glColor4f(colors[3*i+0], colors[3*i+1], colors[3*i+2], 0.5);
			// drawLine(planetsX[2], planetsY[2], planetsZ[2], planetsX[i], planetsY[i], planetsZ[i]);
			float dX = planetsX[i] - planetsX[2];
			float dY = planetsY[i] - planetsY[2];
			float dZ = planetsZ[i] - planetsZ[2];
			float angle = atan2(dY, dX);
			float mag = sqrt( powf(dX,2) + powf(dY,2) + powf(dZ,2) );
			dX = dX / mag * lineLen;
			dY = dY / mag * lineLen;
			dZ = dZ / mag * lineLen;
			drawLine(planetsX[2], planetsY[2], planetsZ[2], planetsX[2]+dX, planetsY[2]+dY, planetsZ[2]+dZ );
			glPushMatrix();
				glTranslatef(planetsX[2]+dX*(1+(i+1)*0.02), planetsY[2]+dY*(1+(i+1)*0.02), planetsZ[2]+dZ*(1+(i+1)*0.02));
				glRotatef(90,1,0,0);
				glRotatef(90+angle/M_PI*180.0,0,1,0);
				glColor4f(colors[3*i+0], colors[3*i+1], colors[3*i+2], 1.0);
				drawCircle(0, 0, 0, 40);
			glPopMatrix();
		}
	}

	// to sun
	{
		glColor3f(242/256.0,229/256.0,129/256.0);
		float angle = atan2(-planetsY[2], -planetsX[2]);
		float mag = sqrt( powf(planetsX[2],2) + powf(planetsY[2],2) + powf(planetsZ[2],2) ) * lineLen;
		drawLine(planetsX[2], planetsY[2], planetsZ[2], -planetsX[2]*mag, -planetsY[2]*mag, -planetsZ[2]*mag );
		glPushMatrix();
			glTranslatef(-planetsX[2]*mag, -planetsY[2]*mag, -planetsZ[2]*mag);
			glRotatef(90,1,0,0);
			glRotatef(90+angle/M_PI*180.0,0,1,0);
			drawCircle(0, 0, 0, 40);
			// drawSphere(planetsX[2]+dX, planetsY[2]+dY, planetsZ[2]+dZ, 4);
		glPopMatrix();

	}

	// planet names
	glColor4f(1.0, 1.0, 1.0, 1.0);
	for(int i = 0; i < 9; i++){
		if(i != 2)
		text(planets[i], planetsX[i], planetsY[i], planetsZ[i]+.2);
	}

	// zodiac names
	for(int i = 0; i < 12; i++){
		float angle = i/12.0*2.0*M_PI + 0.36;
		glColor4f(1.0, 0.1, 0.1, 1.0);
		if(i == zodiac){ glColor4f(1.0, 1.0, 0.3, 1.0); }
		text(zodiacs[i], 900*cosf(angle), 900*sinf(angle), 100.0);
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
	char yearString[50];
	sprintf(yearString, "%d", year);
	text(yearString, 10, 10, 0);
	text(monthStrings[month], 60, 10, 0);
}
void keyDown(unsigned int key){
	if (key == ' '){
		if(MODE == follow){
			MODE = user;
		}
		else if(MODE == user){ 
			lastAngle = atan2(-planetsY[2], planetsX[2]);
			MODE = follow;
		}
	}
}
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }