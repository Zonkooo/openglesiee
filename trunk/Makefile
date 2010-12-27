#Projet OpenGL
CXX=g++
CC=gcc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDFLAGS= -lglut -lGLU -lGLEW
CFLAGS= -Wall -I. -DLINUX
endif

ifeq ($(UNAME), Darwin)
LDFLAGS= -framework glew -framework GLUT -framework OpenGL -framework Cocoa
CFLAGS= -Wall -I. -DMAC
endif

EXE=projet

$(EXE) :  terrain.o tga.o  tg.o 
	$(CXX) $^  $(LDFLAGS) -o $(EXE)


terrain.o : terrain.cpp terrain.h
	$(CXX) -c  $(CFLAGS) $<
	
	
tga.o : tga.cpp tga.h
	$(CXX) -c  $(CFLAGS) $<
	
tg.o : tg.cpp
	$(CXX) -c  $(CFLAGS) $<

.PHONY : clean ultraclean

clean::
	rm -f *.o *~

ultraclean : clean
	rm -f $(EXE)

