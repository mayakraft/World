# Linux (default)
objects = 01 02 03 04 05 06 07 08
CFLAGS = -std=gnu99
LDFLAGS = -lGL -lGLU -lglut -lm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
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

all: $(objects)

%: %.c
	gcc $< -o ../bin/$@ $(CFLAGS) $(LDFLAGS)

run1:
	./../bin/01 $(ARGS)

run2:
	./../bin/02 $(ARGS)

run3:
	./../bin/03 $(ARGS)

run4:
	./../bin/04 $(ARGS)

run5:
	./../bin/05 $(ARGS)

run6:
	./../bin/06 $(ARGS)

run7:
	./../bin/07 $(ARGS)

run8:
	./../bin/08 $(ARGS)
