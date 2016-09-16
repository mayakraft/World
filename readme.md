# OpenGL basic world + interface

![example](https://66.media.tumblr.com/a54164176d27d4c5d888c5ee58f52318/tumblr_odgrd3iDGu1vfq168o1_500.gif)

this is a hyper minimalist (1 file) framework for graphics and user input (keyboard, mouse) following the OpenFrameworks / Processing design paradigm

### Cross platform: Mac, Linux, Windows (cygwin)

Mac / Linux tested. Windows machines might need to install GLUT.

# try it out
type `make` then `make run`

# setup

create an empty .c file containing:

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

# features

```c
setup(){
	// runs once at the beginning
}
```

```c
update(){
	// runs every frame, at the beginning
}
```

```c
draw3D(){
	// runs every frame, after update()
}
draw2D(){
	// a 2D overlay, the last thing to be drawn
}
```

* `P` switch perspectives (first person, polar, and orthographic perspectives)
* `G` 2D ground (2D infinitely-repeating scenery)
* `X` 3D axes (3D infinitely-repeating scenery)
* mouse to look, move with arrow keys or `W` `A` `S` `D`
* `F` fullscreen
* `+` `-` zoom
* `,` `.` change field of view

![example](https://67.media.tumblr.com/62fe5fd43d7390d15ff228595090e6dd/tumblr_odgrd3iDGu1vfq168o2_500.gif)

# license
MIT