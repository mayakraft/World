# Linux (default)
EXE1 = 01screen
EXE2 = 02shaders
EXE3 = 03simple3D
EXE4 = 04shaders3D
EXE5 = 05fps
CFLAGS = -std=gnu99
LDFLAGS = -lGL -lGLU -lglut -lm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE1 = screen.exe
	EXE2 = shaders.exe
	EXE3 = simple3D.exe
	EXE4 = shaders3D.exe
	EXE5 = fps.exe
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

all: $(EXE1) $(EXE2) $(EXE3) $(EXE4) $(EXE5)

$(EXE1): $(EXE1).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE1) $< $(CFLAGS) $(LDFLAGS)

$(EXE2): $(EXE2).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE2) $< $(CFLAGS) $(LDFLAGS)

$(EXE3): $(EXE3).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE3) $< $(CFLAGS) $(LDFLAGS)

$(EXE4): $(EXE4).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE4) $< $(CFLAGS) $(LDFLAGS)

$(EXE5): $(EXE5).c
	@mkdir -p ../bin
	gcc -o ../bin/$(EXE5) $< $(CFLAGS) $(LDFLAGS)

run1:
	./../bin/$(EXE1) $(ARGS)

run2:
	./../bin/$(EXE2) $(ARGS)

run3:
	./../bin/$(EXE3) $(ARGS)

run4:
	./../bin/$(EXE4) $(ARGS)

run5:
	./../bin/$(EXE5) $(ARGS)