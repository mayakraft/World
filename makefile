# Linux (default)
EXE = world
CFLAGS = -std=gnu99
LDFLAGS = -I/opt/vc/include -L/opt/vc/lib -lGL -lm -lglfw -lGLEW

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE = world.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X, OSTYPE not being declared
ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif
ifeq ($(OSTYPE),darwin)
	LDFLAGS = -I./lib/include/ -L./lib/glfw-macos/ -lGLEW -lglfw3 -framework Carbon -framework Cocoa -framework OpenGL -framework IOKit -Wno-deprecated
endif
# ifeq use local src of lGLEW
$(EXE): $(EXE).c
	true > world.h
	cat ./src/header.c >> world.h
	cat ./src/globals.c >> world.h
	cat ./src/math.c >> world.h
	cat ./src/events.c >> world.h
	cat ./src/projection.c >> world.h
	cat ./src/shapes.c >> world.h
	cat ./src/texture.c >> world.h
	cat ./src/helpful.c >> world.h
	cat ./src/loop.c >> world.h
	cat ./src/main.c >> world.h
	cat ./src/footer.c >> world.h
	@mkdir -p bin
	gcc -o bin/$@ $< $(CFLAGS) $(LDFLAGS) -Os -ffunction-sections -fdata-sections

run:
	./bin/$(EXE) $(ARGS)
