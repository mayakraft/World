#ifdef OS_WINDOWS
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/wglew.c"
#else
#  include "../lib/glew-2.0.0/include/GL/glew.h"
#  include "../lib/glew-2.0.0/src/glew.c"
#endif

#include "../world.h"

#define NUM_SHADERS 7
char* fragmentList[NUM_SHADERS] = {
	"../examples/shaders/fog.frag",
	"../examples/shaders/smpte.frag",
	"../examples/shaders/metaballs.frag",
	"../examples/shaders/cave.frag",
	"../examples/shaders/warp.frag",
	"../examples/shaders/water.frag",
	"../examples/shaders/fractal.frag"
};
int selection = 0;

char *vertexPath = "../examples/shaders/simple.vert";
GLuint shader = 0;
unsigned char autoReload = 1; // if true, program will auto-reload the shader every second

void setup() {
	hideHelpfulOrientation();
	shader = loadShader( vertexPath, fragmentList[selection] );
}
void update() {
	if(frame%60 == 0){ if(autoReload) shader = loadShader( vertexPath, fragmentList[selection] ); }
	float resolution[2] = {WIDTH, HEIGHT};
	setShaderUniformVec2f(shader, "u_resolution", resolution);
	setShaderUniform1f(shader, "u_time", frame/60.0);
}
void draw3D() { }
void draw2D() {
	glUseProgram(shader);
	drawRect(0,0,0, WIDTH, HEIGHT);
	glUseProgram(0);
}
void keyDown(unsigned int key) {
	if(key == ' '){
		selection = (selection+1)%NUM_SHADERS;
		// fragmentPath = fragmentList[selection];
		shader = loadShader( vertexPath, fragmentList[selection] );
	}
}
void keyUp(unsigned int key) { }
void mouseDown(unsigned int button) { }
void mouseUp(unsigned int button) { }
void mouseMoved(int x, int y) { }