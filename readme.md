# OpenGL basic world + interface

this is a hyper minimalist (1 file) framework to bring the Processing coding design paradigm to ANSI C (Mac, Linux, Windows-Cygwin) including 3D graphics, textures, shaders, and user input.

# introduction

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

compile your sketch in terminal

```
gcc -o world world.c -std=gnu99 -framework Carbon -framework OpenGL -framework GLUT
```
*MacOS above. See makefile for other operating systems*

Done! You have a compiled application.

```c
setup() // runs once at the beginning
update() // runs every frame, first thing to run
draw3D() // runs every frame, called just after update()
draw2D() // 2D in pixel coordinate space, run after draw3D()
```

# documentation

## variables

* `keyboard[256]` array of keyboard ASCII state: T/F pressed
* `mouseX` `mouseY` mouse location in pixels
* `mouseDragX` `mouseDragY` mouse drag between click and release

## perspective

### variables

* `origin[3]` (x, y, z): the center of the world
* `horizon[3]` (az, alt, zoom): the point on the celestial sphere

### change perspective

* `firstPersonPerspective();`
 * look at: horizon
 * eye at: origin
* `polarPerspective();`
 * look at: origin
 * eye at: horizon
* `orthoPerspective(float x, float y, float width, float height);`

**In Both cases:**

* arrows move origin
* mouse moves horizon

# Easy / Advanced Mode

* `P` switch perspectives (first person, polar, and orthographic perspectives)
* `G` 2D ground (2D infinitely-repeating scenery)
* `X` 3D axes (3D infinitely-repeating scenery)
* mouse to look, move with arrow keys or `W` `A` `S` `D`
* `F` fullscreen
* `+` `-` zoom
* `,` `.` change field of view

![example](https://68.media.tumblr.com/62fe5fd43d7390d15ff228595090e6dd/tumblr_odgrd3iDGu1vfq168o2_500.gif)


![example](https://66.media.tumblr.com/a54164176d27d4c5d888c5ee58f52318/tumblr_odgrd3iDGu1vfq168o1_500.gif)

# license
MIT