# OpenGL basic world + interface

a hyper-minimalist (1 file) framework to bring the Processing design paradigm to a simple low-dependency C environment (Mac, Linux, Windows-Cygwin)

* 2D and 3D graphics, drawing primitives
* user input (mouse, keyboard)
* textures, shaders
* easy built-in perspective frustums

> ### Try it out
>
> 1. clone this repostory
> 2. in terminal type `make` then `make run`
> 
> or try the examples
>
> 1. navigate to the /examples folder `cd examples`
> 2. type `make` then `make run1` or `make run2` ...

# Introduction

```c
setup(); // runs once at the beginning
update(); // runs every frame, first thing to run
// draw() is split out into 2 functions
draw3D(); // runs every frame, called first
draw2D(); // 2D in pixel coordinates, called after draw3D
```

## Make your own sketch

create a .c file with the following:

```c
#include "world.h"

void setup(){ }
void update(){ }
void draw3D(){ }
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }
```

Compile your sketch with the following build command (MacOS), or use the makefile `make`. The following is an example for a file named **world.c**:

```
gcc -o world world.c -std=gnu99 -framework Carbon -framework OpenGL -framework GLUT
```

Done! To run: `./world` or `make run`

---

# Documentation

## Variables

* `elapsed` time in seconds
* `keyboard[256]` array of keyboard ASCII state: T/F if pressed
* `mouseX` `mouseY` mouse location in pixels
* `mouseDragX` `mouseDragY` mouse drag between click and release
* `WIDTH` `HEIGHT` READ ONLY window dimensions

## Funtions

### General

```c
toggleFullscreen();
loadTexture(filename, width, height);
```

### Drawing

```c
text("text", x, y, z);
drawPoint(x, y, z);
drawLine(x1, y1, z1, x2, y2, z2);
drawUnitOriginSquare();
drawRect(x, y, z, width, height);
drawUnitSquare(x, y, z);
drawUnitOriginSphere();
drawUnitSphere(x, y, z);
drawSphere(x, y, z, radius);
drawUnitOriginCircle();
drawUnitCircle(x, y, z);
drawCircle(x, y, z, radius);
draw3DAxesLines(x, y, z, scale);
drawUVSphereLines();
// solid: 0=tetra, 1=octa... 4=dodeca
drawPlatonicSolidFaces(solid);
drawPlatonicSolidLines(solid);
drawPlatonicSolidPoints(solid);
```

### Shaders

```c
loadShader(vertex_path, fragment_path);
setShaderUniform1f(shader, uniform, value);
setShaderUniformVec2f(shader, uniform, array);
setShaderUniformVec3f(shader, uniform, array);
setShaderUniformVec4f(shader, uniform, array);
```

## Perspective

It's easy to get your bearings. Camera orientation is measured using horizontal coordinate system used in astronomy: altitude and azimuth.

### Change Perspective

* `firstPersonPerspective()`: 3D look at horizon
* `polarPerspective()`: 3D look at origin
* `orthoPerspective(x, y, width, height)`: 2D x,y

### 3D Properties

* `origin[3]` (x, y, z) the center of the world
* `horizon[3]` (azimuth, altitude, zoom) point on celestial sphere

### Polar Perspective

![example](https://68.media.tumblr.com/62fe5fd43d7390d15ff228595090e6dd/tumblr_odgrd3iDGu1vfq168o2_500.gif)

### First Person Perspective

![example](https://66.media.tumblr.com/a54164176d27d4c5d888c5ee58f52318/tumblr_odgrd3iDGu1vfq168o1_500.gif)

## Beginner / Advanced Mode

*TO MAKE 3D EASY* the framework comes with keyboard and mouse handling and coordinate space visualizations. You must **opt-out** if you don't want these features. 

default is `BEGINNER`

```c
OPTIONS = ADVANCED;
```

* `BEGINNER`
* `ADVANCED`

Beginner and Advanced turn **all the flags** on or off

### Options Flags

Flip individual flags for a unique combination of built-in features

* `SET_MOUSE_LOOK` *click* mouse move looks around
* `SET_KEYBOARD_MOVE` arrow keys, `W` `A` `S` `D` move view
* `SET_KEYBOARD_FUNCTIONS` *see below*
* `SET_SHOW_GROUND` checkerboard 2D surface
* `SET_SHOW_GRID` repeating 3D grid axes lines

*Example, set new features:*

```c
OPTIONS = SET_MOUSE_LOOK | SET_KEYBOARD_FUNCTIONS | SET_SHOW_GRID;
```

*toggle features:*

```c
OPTIONS ^= SET_SHOW_GROUND;
```


### keyboard functions include

* `ESC` close application
* `F` toggle fullscreen
* `+` `-` zoom
* `,` `.` change field of view
* `P` switch between perspectives
    * first person
    * polar
    * orthographic
* `G` show/hide 2D ground (2D infinitely-repeating scenery)
* `X` show/hide 3D axes (3D infinitely-repeating scenery)

# OpenGL

Whenever possible, this framework uses standard OpenGL built-in functions. Here is an abbreviated list of important calls you may want to know.

### Useful commands

* `glClearColor(0.0, 0.5, 1.0, 1.0)` set RGBA background color
* `glColor4f(1.0, 0.0, 0.0, 1.0)` set RGBA draw color

### Transforms

* `	glTranslatef(x, y, z)`
* `	glRotatef(angle, 1, 0, 0)`
* `glScalef(10, 10, 10)`
* `glMultMatrixf(m)`
* `glPushMatrix()` `glPopMatrix()`

### Lighting

* `	glMaterialfv(GL_FRONT, GL_DIFFUSE, white)` set draw color
* `glEnable(GL_LIGHTING)`, `glEnable(GL_LIGHT0)` enable light
* `glLightfv(GL_LIGHT0, GL_DIFFUSE, white)` set light color
* `glLightfv(GL_LIGHT0, GL_POSITION, pos)` set light position

These are just a few useful functions. A classic resource for more information is the OpenGL Programming Guide ("the OpenGL red book"), look into:

* lighting effects, shadows, fog
* matrix stack
* depth buffer
* state machine

# license
MIT