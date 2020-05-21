void mouseUpdatePerspective(int dx, int dy);
void drawUnitCircle(float x, float y, float z);
void UPDATE();
void DRAW(GLFWwindow* window);
void reshapeWindow(int windowWidth, int windowHeight);
void rebuildProjection();
void toggleFullscreen();
void firstPersonPerspective();
void polarPerspective();
void orthoPerspective(float x, float y, float width, float height);

///////////////////////////////////////////////////////////////////////////////
//      WORLD is a hyper minimalist framework for graphics (OpenGL) 
//   and user input (keyboard, mouse) in the Processing design paradigm
///////////////////////////////////////////////////////////////////////////////
//
//   HOW TO USE
//
//   1) make an empty .c file
//   2) #include "world.h"
//   3) implement the following functions below.
//      you're done! build with makefile: 'make', 'make run'
void setup();
void update();
void draw3D();
void draw2D();
void keyDown(unsigned int key);
void keyUp(unsigned int key);
void mouseDown(unsigned int button);
void mouseUp(unsigned int button);
void mouseMoved(int x, int y);
void worldDelegate(unsigned int code);


///////////////////////////////////////////////////////////////////////////////
static float _invert_y_m[16] = {1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1};
static unsigned char SHAPE_FILL = 1;
enum{ 
  SET_MOUSE_LOOK = 1 << 0,
  SET_KEYBOARD_MOVE = 1 << 1,
  SET_KEYBOARD_ZOOM = 1 << 2,
  SET_KEYBOARD_FUNCTIONS = 1 << 3,
  SET_SHOW_GROUND = 1 << 4,  // a 2D orientation grid
  SET_SHOW_GRID = 1 << 5    // a 3D orientation grid
};
enum { BIT_MOUSE_LOOK, BIT_KEYBOARD_MOVE, BIT_KEYBOARD_ZOOM, BIT_KEYBOARD_FUNCTIONS, BIT_SHOW_GROUND, BIT_SHOW_GRID };

GLFWwindow *WINDOW = NULL;
GLFWmonitor *MONITOR = NULL;

#define UP_KEY 265
#define DOWN_KEY 264
#define LEFT_KEY 263
#define RIGHT_KEY 262

#define ESCAPE_KEY 256
#define SPACE_BAR 32
#define RETURN_KEY 257
#define DELETE_KEY 259
#define EQUAL_KEY 61
#define PLUS_KEY 61
#define MINUS_KEY 45
#define UNDERBAR_KEY 45
#define PERIOD_KEY 46
#define GREATER_THAN_KEY 46
#define COMMA_KEY 44
#define LESS_THAN_KEY 44

///////////////////////////////////////////////////////////////////////////////
//  SETTINGS
const char *APP_TITLE = "World";
enum { LEFT, RIGHT };
unsigned char HANDED = LEFT; // 0:left, 1:right. flip coordinate axes orientation.
#define CONTINUOUS_REFRESH 1  // set 0 for maximum battery efficiency, only redraws screen upon input
static unsigned char SETTINGS = 0b11111111; // flip bits to turn on and off features. see documentation.
static unsigned char SIMPLE_SETTINGS = 255;  // simple mode (default) hooks helpful keyboard and visual feedback
static unsigned char ADVANCED_SETTINGS = 0;
// WINDOW
// 1280, 800
static int WIDTH = 800;  // (readonly) adjust window dimensions. not able to be set at runtime
static int HEIGHT = 600; // (readonly)
static unsigned char FULLSCREEN = 0;  // fullscreen:1   window:0
static float ASPECT;  // (readonly) aspect ratio of window dimensions
// PROJECTION
static float NEAR_CLIP = 0.1;
static float FAR_CLIP = 10000.0;
static float FOV = 0.1;
// PERSPECTIVE
enum { FPP,  POLAR,  ORTHO }; // first persion, polar, orthographic
static unsigned char PERSPECTIVE = FPP;  // initialize point of view in this state
// CAMERA ORIENTATION AND POSITION
float ORIGIN[3] = {0.0f, 0.0f, 0.0f};  // x, y, z, location of the eye
float HORIZON[3] = {0.0f, 0.0f, 7.0f};   // azimuth, altitude, zoom (log)
float FRAME[4]; // x, y, width, height
static float EYE_HEIGHT = 1.0;  // camera offset above the origin ground plane in first-person perspective mode.
// INPUT
static int mouseX = 0;  // get mouse location, units in pixels
static int mouseY = 0;
static unsigned char KEYBOARD[348];  // get current pressed-state of each keyboard key (0:up, 1:pressed)
// mouse drag
static int mouseDownX, mouseDownY;  // mouse down drag distance
static int mouseDownStartX, mouseDownStartY;  // mouse down initial location
static float MOUSE_SENSITIVITY = 0.333f;
static float WALK_INTERVAL = 0.077f;  // first person walking speed. @ 60 UPS (updates/sec), walk speed (units/sec) = INTERVAL * UPS
static float ZOOM_SPEED = 0.1f;
// TIME
// static unsigned long FRAME;  // # times the screen has drawn 
struct timespec START, CURRENT;
static float ELAPSED;  // elapsed time in seconds, includes fractional part
int YEAR, MONTH, DAY, HOUR, MINUTE, SECOND;
