# OpenGL basic world + interface

a hyper-minimalist (1 file) framework to bring the Processing coding design paradigm to simple, low-dependency C (Mac, Linux, Windows-Cygwin)

* 3D graphics
* user input (mouse, keyboard)
* 2D and 3D drawing primitives
* textures, shaders
* simple 3D perspectives setup
    * horizontal coordinates: altitude + azimuth

# Introduction

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

compile your sketch in terminal by typing `make`, which runs the following build command:

```
gcc -o world world.c -std=gnu99 -framework Carbon -framework OpenGL -framework GLUT
```

Done! Run with `make run`

## Processing-style design

```c
setup(); // runs once at the beginning
update(); // runs every frame, first thing to run
// draw() is split out into 2 functions
draw3D(); // runs every frame, called first
draw2D(); // 2D in pixel coordinates, called after draw3D
```

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

### Change Perspective

* `firstPersonPerspective()`: 3D look at horizon
* `polarPerspective()`: 3D look at origin
* `orthoPerspective(x, y, width, height)`: 2D x,y

### 3D Properties

* `origin[3]` (x, y, z) the center of the world
* `horizon[3]` (azimuth, altitude, zoom) point on celestial sphere

# Easy / Advanced Mode

some presets baked into the keyboard and mouse, because setting up 3D perspectives is busywork.

* `P` switch perspectives (first person, polar, and orthographic perspectives)
* `G` 2D ground (2D infinitely-repeating scenery)
* `X` 3D axes (3D infinitely-repeating scenery)
* mouse to look, move with arrow keys or `W` `A` `S` `D`
* `F` fullscreen
* `+` `-` zoom
* `,` `.` change field of view

### Polar Perspective

![example](https://68.media.tumblr.com/62fe5fd43d7390d15ff228595090e6dd/tumblr_odgrd3iDGu1vfq168o2_500.gif)

### First Person Perspective

![example](https://66.media.tumblr.com/a54164176d27d4c5d888c5ee58f52318/tumblr_odgrd3iDGu1vfq168o1_500.gif)

# license
MIT