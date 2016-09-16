# Linux (default)
EXE = world
CFLAGS = -std=gnu99
LDFLAGS = -lGL -lGLU -lglut -lm

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
	# LDFLAGS="-L /usr/local/Cellar/glew/1.12.0/include/GL -lglew" gcc -o world world.c -std=gnu99 -framework Carbon -framework OpenGL -framework GLUT -Wno-deprecated
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -Wno-deprecated -L/usr/local/lib -lGLEW
endif

$(EXE) : world.c
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)

run :
	./$(EXE) $(ARGS)