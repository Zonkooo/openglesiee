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

#dependences des .h
terrain.o : terrain.h tga.h	
tga.o : tga.h

#cible générique
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@
	
.PHONY : clean ultraclean

clean::
	rm -f *.o *~

ultraclean : clean
	rm -f $(EXE)

