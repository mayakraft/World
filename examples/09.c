// example 9
//
// HSL

#include "../world.h"

typedef struct {
	double r;       // between 0 and 1
	double g;       // between 0 and 1
	double b;       // between 0 and 1
} RGB;

typedef struct {
	double h;       // between 0 and 360
	double s;       // between 0 and 1
	double v;       // between 0 and 1
} HSV;

static HSV RGB2HSV(RGB in);
static RGB HSV2RGB(HSV in);

HSV RGB2HSV(RGB in){
	HSV out;
	double min, max, delta;
	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min  : in.b;
	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max  : in.b;
	out.v = max;
	delta = max - min;
	if (delta < 0.00001) {
		out.s = out.h = 0;
		return out;
	}
	if( max > 0.0 ){ out.s = (delta / max); } 
	else {
		out.s = 0.0;
		out.h = NAN;
		return out;
	}
	if( in.r >= max ){ out.h = ( in.g - in.b ) / delta; }
	else
	if( in.g >= max ){ out.h = 2.0 + ( in.b - in.r ) / delta; }
	else{ out.h = 4.0 + ( in.r - in.g ) / delta; }
	out.h *= 60.0;
	if( out.h < 0.0 ){ out.h += 360.0; }
	return out;
}

RGB HSV2RGB(HSV in){
	double hh, p, q, t, ff;
	long i;
	RGB out;
	if(in.s <= 0.0) { out.r = out.g = out.b = in.v; return out; }
	hh = in.h;
	while(hh >= 360.0) hh -= 360.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));
	switch(i) {
		case 0: out.r = in.v;  out.g = t;     out.b = p;    break;
		case 1: out.r = q;     out.g = in.v;  out.b = p;    break;
		case 2: out.r = p;     out.g = in.v;  out.b = t;    break;
		case 3: out.r = p;     out.g = q;     out.b = in.v; break;
		case 4: out.r = t;     out.g = p;     out.b = in.v; break;
		case 5:
		default: out.r = in.v; out.g = p;     out.b = q;    break;
	}
	return out;     
}

void setup(){ 
	SETTINGS = SET_MOUSE_LOOK | SET_KEYBOARD_MOVE | SET_KEYBOARD_FUNCTIONS;
	HORIZON[2] = 50;
	polarPerspective();
	HORIZON[1] = 90;
}
void update(){ }
void draw3D(){
	HSV groundHSV;
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
			groundHSV.h = 360*j*0.01;
			groundHSV.s = 1.0;
			groundHSV.v = 1-i*0.01;
			RGB ground = HSV2RGB(groundHSV);
			glColor4f(ground.r, ground.g, ground.b, 1.0);
			drawRect(-50 + i, -50 + j, 0, 1, 1);
		}
	}
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }