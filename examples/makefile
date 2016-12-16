# Linux (default)
EXE1 = simple
EXE2 = shaders
CFLAGS = -std=gnu99
LDFLAGS = -lGL -lGLU -lglut -lm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE1 = simple.exe
	EXE2 = shaders.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X, OSTYPE not being declared
ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif
ifeq ($(OSTYPE),darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT  -Wno-deprecated
endif

all: simple shaders

$(EXE1): $(EXE1).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE1) $< $(CFLAGS) $(LDFLAGS)

$(EXE2): $(EXE2).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE2) $< $(CFLAGS) $(LDFLAGS)

run1:
	./../bin/$(EXE1) $(ARGS)

run2:
	./../bin/$(EXE2) $(ARGS)
