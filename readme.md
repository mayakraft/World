# OpenGL basic world + interface

### Mac, Linux, Windows

this is a hyper minimalist (1 file) framework for graphics (OpenGL) and user input (keyboard, mouse) following the OpenFrameworks / Processing design paradigm

# try it out
type `make` then `make run`

# setup

1. create a blank .c file
2. `#include "world.c"`
3. implement a few functions including:

```
setup(){
	// runs once at the beginning
}
```

```
update(){
	// runs every frame, at the beginning
}
```

```
draw(){
	// runs every frame, after update()
}
```

#features

## perspectives `SPACE BAR`
* first person: looking, walking
* polar: rotating around origin, zooming
* orthographic: panning, zooming

## landscapes
* 'G' 2D ground (2D infinitely-repeating scenery)
* 'X' 3D grid (3D infinitely-repeating scenery)

## more input
* mouse look
* fullscreen `F`
* zoom `+` `-`
* arrow keys and `W` `A` `S` `D`

# license
MIT